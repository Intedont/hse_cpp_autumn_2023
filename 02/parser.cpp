#include <iostream>
#include <vector>
#include <errno.h>

class TokenParser
{
private:
    // флаги, используемые для проверки регистрации callback функций
    bool is_start_init = false;
    bool is_end_init = false;
    bool is_digit_init = false;
    bool is_string_init = false;

    void (*start_func)(const std::string& s);
    void (*end_func)();
    void (*digit_func)(long long unsigned int& token);
    void (*string_func)(std::string& token);

public:
    TokenParser() = default;

    // Устанавливаем callback-функцию перед стартом парсинга.
    void setStartCallback(void (*start_callback)(const std::string& line)){
        is_start_init = true;
        start_func = start_callback;
    }

    // Устанавливаем callback-функцию после окончания парсинга.
    void setEndCallback(void (*end_callback)()){
        is_end_init = true;
        end_func = end_callback;
    }

    // Устанавливаем callback-функцию для обработки чисел.
    void setDigitTokenCallback(void (*digit_callback)(long long unsigned int& token)){
        is_digit_init = true;
        digit_func = digit_callback;
    }

    // Устанавливаем callback функцию для обработки строковых токенов.
    void setStringTokenCallback(void (*string_callback)(std::string& token)){
        is_string_init = true;
        string_func = string_callback;
    }

    std::vector<std::string> split(const std::string &s){
        std::vector<std::string> tokens;
        std::string current_token = "";

        for(int i = 0; i < s.length(); i++){
            if(s[i] == ' ' || s[i] == '\t' || s[i] == '\n'){
                if(current_token != ""){
                    tokens.push_back(current_token);
                    current_token = "";
                }
            } else {
                current_token = current_token + s[i];
            }
        }
        
        if(current_token != ""){
            tokens.push_back(current_token);
        }

        return tokens;
    }

    bool is_number(const std::string& s){
        for (char const &ch : s) {
            if (std::isdigit(ch) == 0) 
                return false;
        }
        return true;
    }

    std::vector<std::string> parse(const std::string &line){
        if(is_start_init){
            start_func(line);
        }

        std::vector<std::string> tokens = split(line);
        
        // Определение типа токенов и их обработка соотвествующими функциями
        for(int i = 0; i < tokens.size(); i++){
            if(is_number(tokens[i])){
                char* end;
                unsigned long long token_converted = std::strtoull(tokens[i].c_str(), &end, 10);
                // если число не влезло в uint64_t, то обрабатываем как строку
                if (errno == ERANGE){
                    errno = 0;
                    if(is_string_init){
                        string_func(tokens[i]);
                    }
                } else {
                    if(is_digit_init){
                        digit_func(token_converted);
                    }
                    tokens[i] = std::to_string(token_converted);
                }
            } else {
                if(is_string_init){
                    string_func(tokens[i]);
                }
            }
        }

        if(is_end_init){
            end_func();
        }

        return tokens;
    }
};