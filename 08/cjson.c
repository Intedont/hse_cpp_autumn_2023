#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define START_SIZE 100 // Начальный размер вектора

static PyObject* cjson_loads(PyObject* self, PyObject* args){
    char* json_string;
    size_t string_len;

    // Парсинг строки из аргументов
    if (!PyArg_ParseTuple(args, "s#", &json_string, &string_len))
    {
        printf("ERROR: Failed to initialize dict\n");
        return NULL;  
    }

    // Инициализация словаря
    PyObject* py_dict = NULL;
    if (!(py_dict = PyDict_New())) {
        printf("ERROR: Failed to create Dict Object\n");
        return NULL;
    }

    char state = '0'; // Переменная для хранения состояний
    /*
        Программа проходит состояния 0-1-2-3-(4)-0 при парсинге каждой пары ключ/значение
        0 - Состояние перед парсингом ключа
        1 - Состояние внутри кавычек при парсинге ключа
        2 - Состояние после окончания парсинга ключа. Поиск запятой.
        3 - Состояние парсинга интового значения
        4 - Состояние парсинга строкового значения
    */
    char* key = NULL; // Переменная для хранения спаршенного ключа
    size_t key_size = 0;

    char* value = NULL; // Переменная для хранения спаршенного значения
    size_t value_size = 0;

    bool is_value_integer = true;

    for(size_t i = 0; i < string_len; ++i){
        char current = json_string[i];

        // Переход по состояниям
        if(current == '"'){
            if(state == '0'){
                state = '1';
            } else if(state == '1'){
                state = '2';
            } else if(state == '3'){
                state = '4';
                is_value_integer = false;
            } else if(state == '4'){
                state = '3';
            }
        } else if ((current == ':') && (state == '2')){ 
            state = '3';
        } else if ((current == ',' || current == '}') && (state == '3')){ 
            // Парсинг пары окончен. Запись в словарь
            state = '0';
            
            PyObject *py_key = NULL;
            PyObject *py_value = NULL;
            // Преобразования ключа py объект
            if (!(py_key = Py_BuildValue("s#", key, key_size))) {
                printf("ERROR: Failed to build string value\n");
                return NULL;
            }

            // Преобразования значения в py объект
            if(is_value_integer){
                if (!(py_value = Py_BuildValue("i", atoi(value)))) {
                    printf("ERROR: Failed to build integer value\n");
                    return NULL;
                }
            } else {
                is_value_integer = true;
                if (!(py_value = Py_BuildValue("s#", value, value_size))) {
                    printf("ERROR: Failed to build string value\n");
                    return NULL;
                }
            }

            // Запись пары в словарь
            if (PyDict_SetItem(py_dict, py_key, py_value) < 0) {
                printf("ERROR: Failed to set item\n");
                return NULL;
            }

            value = NULL;
            key = NULL;
            key_size = 0;
            value_size = 0;

        } else {
            // Парсинг символов из строки
            if(state == '4'){
                if (value == NULL)
                {
                    value = malloc(sizeof(char));
                    *value = current;
                    value_size = 1;
                } 
                else
                {
                    value = realloc(value, value_size + 1);
                    value[value_size] = current;
                    value_size = value_size + 1;
                }
            } else if(state == '1'){
                if (key == NULL) 
                {
                    key = malloc(sizeof(char));
                    *key = current;
                    key_size = 1;
                } 
                else 
                {
                    key = realloc(key, key_size + 1);
                    key[key_size] = current;
                    key_size = key_size + 1;
                }
            } else if(state == '3'){
                if(current != ' '){
                    if (value == NULL)
                    {
                        value = malloc(sizeof(char));
                        *value = current;
                        value_size = 1;
                    } 
                    else
                    {
                        value = realloc(value, value_size + 1);
                        value[value_size] = current;
                        value_size = value_size + 1;

                    }
                }
            }
        }
    }
    return py_dict;
}

// Самодельный динамически расширяемый вектор для функции dumps
struct StringVector{
    char* string;
    size_t size;
    size_t pos;
};

void push_back(struct StringVector* string_vector, char to_push){
    if(string_vector->pos < string_vector->size){        
        string_vector->string[string_vector->pos] = to_push;
    } else {
        string_vector->size = (string_vector->size) * 2;
        string_vector->string = (char*)realloc(string_vector->string, string_vector->size * sizeof(char));
        string_vector->string[string_vector->pos] = to_push;
    }

    string_vector->pos += 1;
}

static PyObject* cjson_dumps(PyObject* self, PyObject* args){
    PyObject* py_dict;
    // Проверки, что словарь загрузился нормально
    if (!PyArg_ParseTuple(args, "O", &py_dict)) {
        return NULL;
    }
    if (!PyDict_Check(py_dict)) {
        printf("ERROR: Not a dictionary\n");
        return NULL;
    }

    PyObject *key, *value;

    
    // Инициализация вектора
    struct StringVector* result_string = malloc(sizeof(struct StringVector));
    result_string->string = (char*) malloc(START_SIZE * sizeof(char));
    result_string->size = START_SIZE;
    result_string->pos = 0;
    
    char* s; // Буферная переменная для итерирования по спаршенным стрингам
    size_t s_pos = 0; //Текущая позиция буферной переменной

    Py_ssize_t len = PyDict_Size(py_dict);
    Py_ssize_t pos = 0;
    
    push_back(result_string, '{');
    
    // Итерация по парам ключ/значение словаря
    while (PyDict_Next(py_dict, &pos, &key, &value)) {
        push_back(result_string, '"');

        // Преобразование ключа в строку
        if(PyUnicode_Check(key)){
            s = PyBytes_AsString(PyUnicode_AsUTF8String(key));
        } else {
            printf("ERROR: Not supported key type\n");
            return NULL;
        }

        // Заполнение вектора ключом
		s_pos = 0;
		while(s[s_pos] != '\0')
		{
            push_back(result_string, s[s_pos++]);
		}

        push_back(result_string, '"');
        push_back(result_string, ':');
        push_back(result_string, ' ');
        
        // Преобразование значения 
        // Проверка, что значение - число
        if(PyLong_Check(value)){
            char str[256];
            sprintf(str, "%ld", PyLong_AsLong(value));
            
            s_pos = 0;
            while((str[s_pos] != '\0') && (s_pos < 256))
            {
                push_back(result_string, str[s_pos++]);
            }
            
        // Проверка, что значение - строка
        } else if (PyUnicode_Check(value)){
            push_back(result_string, '"');

            s = PyBytes_AsString(PyUnicode_AsUTF8String(value));
            s_pos = 0;
            while((s[s_pos] != '\0'))
            {
                push_back(result_string, s[s_pos++]);
            }

            push_back(result_string, '"');

        } else {
            printf("ERROR: Not supported value type\n");
        }
        
        // Если вставляемая пара не последняя в словаре, то пишем запятую
        if(pos < len){
            push_back(result_string, ',');
            push_back(result_string, ' ');
        }
        
    }

    push_back(result_string, '}');
    push_back(result_string, '\0');

    
    return Py_BuildValue("s", result_string->string);
    return NULL;
}


//************************************************************************
// Предыдущая версия dumps. Работает очень медленно, но есть полезный код. 
// Оставил на память.
//************************************************************************

/*
PyObject* append(PyObject* left, PyObject* right){
    PyObject* tmp = PyUnicode_Concat(left, right);
    Py_DECREF(left);
    return tmp;
}

static PyObject* cjson_dumps(PyObject* self, PyObject* args){
    PyObject* py_dict;
    if (!PyArg_ParseTuple(args, "O", &py_dict)) {
        return NULL;
    }
    if (!PyDict_Check(py_dict)) {
        printf("ERROR: Not a dictionary\n");
        return NULL;
    }

    PyObject *key, *value;

    PyObject* bracket = PyUnicode_FromString("\"");
    PyObject* key_ending = PyUnicode_FromString("\": ");
    PyObject* comma = PyUnicode_FromString(", ");

    PyObject* result_string = PyUnicode_FromString("{");
    Py_ssize_t pos = 0;
    bool is_first = true;

    while (PyDict_Next(py_dict, &pos, &key, &value)) {
        
        if(!is_first){
            result_string = append(result_string, comma);
        }
        is_first = false;
        
        result_string = append(result_string, bracket);
        
        
        if (PyUnicode_Check(key)){
            result_string = append(result_string, key);

        } else {
            printf("ERROR: Not supported key type\n");
        }
        
        result_string = append(result_string, key_ending);
        
        
        if(PyLong_Check(value)){
            char str[256];
            sprintf(str, "%ld", PyLong_AsLong(value));
            result_string = append(result_string, PyUnicode_FromString(str));
        } else if (PyUnicode_Check(value)){
            result_string = append(result_string, bracket);
            result_string = append(result_string, value);
            result_string = append(result_string, bracket);
        } else {
            printf("ERROR: Not supported value type\n");
        }
        
    }
    result_string = append(result_string, PyUnicode_FromString("}"));

    return result_string;
}*/



struct PyMethodDef cjson_method_def[] = {
    {"loads", cjson_loads, METH_VARARGS, "parse string to dict"},
    {"dumps", cjson_dumps, METH_VARARGS, "parse dict to string"},
    {NULL, NULL, 0, NULL}
};

struct PyModuleDef cjson_module_def = {
    PyModuleDef_HEAD_INIT,
    "cjson",
    "Module provides useful functions",
    -1,
    cjson_method_def
};


PyMODINIT_FUNC PyInit_cjson(void)
{
    Py_Initialize();
    return PyModule_Create(&cjson_module_def);
}

