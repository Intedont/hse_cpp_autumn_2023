#include <iostream>
#include <sstream>
#include <string>

// Исключение, выбрасываемое если аргумент, запрашиваемый в скобках не найден 
// (если число в скобках больше числа аргументов)
struct ArgumentNotFound{
    std::string message;
    int line;

    ArgumentNotFound(const std::string& m, const int& l) : message{m}, line{l} {}
};

// Исключение, выбрасываемое при появлении скобки вне контекста {n}
struct SymbolIsReserved{
    std::string message;
    int character;

    SymbolIsReserved(const std::string& m, const int& l) : message{m}, character{l} {}
};

// Исключение, выбрасываемое если не удается конвертировать выражение внутри скобок в номер аргумента
struct ConvertError{
    std::string message;
    std::string to_convert;

    ConvertError(const std::string& m, const std::string& l) : message{m}, to_convert{l} {}
};

// Более строгая версия stoi - бросается исключениями, если stoi перевел в int выпарсенное из входного выражения число
int stoi_strict(const std::string &str, int base = 10) {
    int res;
    size_t pos = 0;
    try {
        res = std::stoi(str, &pos, base);
    } catch (const std::invalid_argument &ia) {
        // Перехватываем родное stoi исключение
    }
    if (pos != str.size())
        throw ConvertError("Can't convert text in {} to argument number", str);
    return res;
}

template <typename T>
void print_argument(std::ostream& out, int search_num, int current_num, T&& arg){
    if(search_num == current_num){
        out << arg;
    } else {
        throw ArgumentNotFound("Argument not found", search_num);
    }
}

template <typename T, typename... Args>
void print_argument(std::ostream& out, int search_num, int current_num, T&& arg, Args&&... args){
    if(search_num == current_num){
        out << arg;
    } else {
        print_argument(out, search_num, current_num + 1, args...);
    }
}

template <typename... Args>
std::string format(const std::string&& text, Args&&... args){
    std::stringstream out;
    std::string arg_num = "";
    bool state = false; // Имеем два состояния при проходе по тексту: 1 - внутри {}, 0 - вне {}
    for(size_t i=0; i < text.size(); ++i){
        if(state){
            if(text[i] == '}'){
                print_argument(out, stoi_strict(arg_num), 0, args...);
                state = false;
                arg_num = "";
            } else if(text[i] == '{' || i == (text.size()-1)) {
                throw SymbolIsReserved("{} is reserved", i);
            } else {
                arg_num = arg_num + text[i];
            }
        } else {
            if(text[i] == '}' || (text[i] == '{' && i == (text.size()-1))) {
                throw SymbolIsReserved("{} is reserved", i);
            } else if(text[i] == '{'){
                state = true;
            } else {
                out << text[i];
            }
        }

    }
    return out.str();
}
