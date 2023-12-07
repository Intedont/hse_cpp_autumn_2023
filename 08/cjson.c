#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


static PyObject* cjson_loads(PyObject* self, PyObject* args){
    char* json_string;
    size_t string_len;

    if (!PyArg_ParseTuple(args, "s#", &json_string, &string_len))
    {
        printf("ERROR: Failed to initialize dict\n");
        return NULL;  
    }

    PyObject* py_dict = NULL;
    if (!(py_dict = PyDict_New())) {
        printf("ERROR: Failed to create Dict Object\n");
        return NULL;
    }

    char state = '0';
    char* key = NULL;
    size_t key_size = 0;
    char* value = NULL; 
    size_t value_size = 0;

    bool is_value_integer = true;

    for(size_t i = 0; i < string_len; ++i){
        char current = json_string[i];
        //printf("%c\n", current);
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
            state = '0';
            
            PyObject *py_key = NULL;
            PyObject *py_value = NULL;
            if (!(py_key = Py_BuildValue("s#", key, key_size))) {
                printf("ERROR: Failed to build string value\n");
                return NULL;
            }
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

            if (PyDict_SetItem(py_dict, py_key, py_value) < 0) {
                printf("ERROR: Failed to set item\n");
                return NULL;
            }

            value = NULL;
            key = NULL;
            key_size = 0;
            value_size = 0;

        } else {
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

                    //printf(" REALLOCED value ");
                    //printf("%c", key[strlen(key) - 1]);
                    //for(int k = 0; k < value_size; ++k){
                    //    printf("%c", value[k]);
                    //}
                }
            } else if(state == '1'){
                if (key == NULL) 
                {
                    key = malloc(sizeof(char));
                    *key = current;
                    key_size = 1;
                    //printf("MALLOCED key ");
                    //printf("%c", *key);
                } 
                else 
                {
                    key = realloc(key, key_size + 1);
                    key[key_size] = current;
                    key_size = key_size + 1;

                    //printf(" REALLOCED key ");
                    //for(int k = 0; k < key_size; ++k){
                    //    printf("%c", key[k]);
                    //}
                    //printf(" ");
                }
            } else if(state == '3'){
                if(current != ' '){
                    if (value == NULL)
                    {
                        value = malloc(sizeof(char));
                        *value = current;
                        value_size = 1;
                        //printf("MALLOCED value ");
                        //printf("%c", *value);
                        
                    } 
                    else
                    {
                        value = realloc(value, value_size + 1);
                        value[value_size] = current;
                        value_size = value_size + 1;

                        //printf(" REALLOCED value ");
                        //printf("%c", key[strlen(key) - 1]);
                        //for(int k = 0; k < value_size; ++k){
                        //    printf("%c", value[k]);
                        //}
                    }
                }
            }
            
        }
    
    }

    return py_dict;
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
            result_string = PyUnicode_Concat(result_string, comma);
        }
        is_first = false;
        
        result_string = PyUnicode_Concat(result_string, bracket);

        if (PyUnicode_Check(key)){
            result_string = PyUnicode_Concat(result_string, key);
        } else {
            printf("ERROR: Not supported key type\n");
        }
        result_string = PyUnicode_Concat(result_string, key_ending);

        
        
        if(PyLong_Check(value)){
            char str[256];
            sprintf(str, "%ld", PyLong_AsLong(value));
            result_string = PyUnicode_Concat(result_string, PyUnicode_FromString(str));
        } else if (PyUnicode_Check(value)){
            result_string = PyUnicode_Concat(result_string, bracket);
            result_string = PyUnicode_Concat(result_string, value);
            result_string = PyUnicode_Concat(result_string, bracket);
        } else {
            printf("ERROR: Not supported value type\n");
        }
        
    }
    result_string = PyUnicode_Concat(result_string, PyUnicode_FromString("}"));

    return result_string;

}


struct PyMethodDef cjson_method_def[] = {
    {"loads", cjson_loads, METH_VARARGS, "Iterate over array and return sum."},
    {"dumps", cjson_dumps, METH_VARARGS, "Iterate over array and return sum."},
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

