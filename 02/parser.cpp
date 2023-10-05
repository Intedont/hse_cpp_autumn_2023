#include <iostream>
#include <vector>
#include <errno.h>

class TokenParser
{
private:
    void (*start_func)(const std::string& s);
    void (*end_func)();
    void (*digit_func)(long long unsigned int& token);
    void (*string_func)(std::string& token);

public:
    TokenParser() {
        // задаем дефолтные функции
        start_func = [] (const std::string& line) { std::cout << "Parsing is started" <<std::endl; };
        end_func = [] () { std::cout << "Parsing is complete" <<std::endl; };
        digit_func = [] (long long unsigned int& token) { std::cout << "This is digit token: " << token <<std::endl; };
        string_func = [] (std::string& token) { std::cout << "This is string token: " << token <<std::endl; };

    }

    // Устанавливаем callback-функцию перед стартом парсинга.
    void setStartCallback(void (*start_callback)(const std::string& line)){
        start_func = start_callback;
    }

    // Устанавливаем callback-функцию после окончания парсинга.
    void setEndCallback(void (*end_callback)()){
        end_func = end_callback;
    }

    // Устанавливаем callback-функцию для обработки чисел.
    void setDigitTokenCallback(void (*digit_callback)(long long unsigned int& token)){
        digit_func = digit_callback;
    }

    // Тут другие методы для установки callback-функций.
    void setStringTokenCallback(void (*string_callback)(std::string& token)){
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
        start_func(line);

        std::vector<std::string> tokens = split(line);
        
        for(int i = 0; i < tokens.size(); i++){
            if(is_number(tokens[i])){
                char* end;
                unsigned long long token_converted = std::strtoull(tokens[i].c_str(), &end, 10);
                // если число не влезло в uint64_t, то обрабатываем как строку
                if (errno == ERANGE){
                    errno = 0;
                    string_func(tokens[i]);
                } else {
                    digit_func(token_converted);
                    tokens[i] = std::to_string(token_converted);
                }
            } else {
                string_func(tokens[i]);
            }
        }

        end_func();

        return tokens;
    }
};


