#include <stdint.h>
#include <cstddef>
#include <string>
#include <iostream>
#include <cmath>
#include "bigint.hpp"


BigInt::BigInt(){}

BigInt::BigInt(std::string num){
    // Проверяем на пустую строку
    if(num.size() == 0){
        throw std::exception();
    }

    // Парсим минус в начале
    if(num[0] == '-'){
        is_negative = true;
        num = num.substr(1, num.size() - 1);
    }

    // Удаляем ведущие нули
    while(num[0] == '0' && num.size() != 1){
        num = num.substr(1, num.size() - 1);
    }

    // Вычисляем необходимую длину массива
    data_size = num.size() % base_power != 0 ? (num.size() / base_power) + 1 : (num.size() / base_power);
    data = new int32_t[data_size]; 

    // Заполняем массив
    size_t i = 1;
    while(i * base_power <= num.size()){
        data[i - 1] = atoi(num.substr(num.size() - i * base_power, base_power).c_str());
        ++i;
    }
    if(i * base_power > num.size()){
        data[i - 1] = atoi(num.substr(0, num.size() - (i - 1) * base_power).c_str());
    }
}

BigInt::BigInt(size_t s) : data_size{s}, data{new int32_t[s]} {
    if(s == 0){
        throw std::exception();
    }

    for(size_t i = 0; i < data_size; ++i){
        data[i] = 0;
    }
}

// Копирующий конструктор
BigInt::BigInt(const BigInt& source) : data_size{source.data_size}, is_negative{source.is_negative}{
    data = new int32_t[data_size];
    for(size_t i = 0; i < data_size; ++i){
        data[i] = source.data[i];
    }
}

// Перемещающий конструктор
BigInt::BigInt(BigInt&& other)
{
    data_size = other.data_size;
    is_negative = other.is_negative;
    data = other.data;
    other.data = nullptr;
}

// Конструктор создания пустого BigInt
BigInt::BigInt(int32_t source){
    is_negative = source < 0 ? true : false;
    data_size = 1;
    int32_t tmp = source;
    while(tmp / base != 0){
        data_size += 1;
        tmp /= base;
    }
    data = new int32_t[data_size];
    for(size_t i = 0; i < data_size; ++i){
        data[i] = source % base;
        source = source / base;
    }
}

BigInt::~BigInt(){
    delete[] data;
}

// Функции
//==============================================================================================================

int32_t* BigInt::get_array() const {
    return data;
}

size_t BigInt::get_size() const {
    return data_size;
}

std::string BigInt::as_string() const {
    std::string tmp{""};
    if(is_negative){
        tmp.append("-");
    }
    tmp.append(std::to_string(data[data_size - 1]));
    for(size_t i = this->data_size - 1; i != 0; i--){
        std::string current_digit = std::to_string(data[i - 1]);
        while(current_digit.size() != base_power){
            current_digit = "0" + current_digit;
        }
        tmp.append(current_digit); 
    }
    
    return tmp;
}

// Уменьшаем длину числа, если после операций с ним появляются ведущие нули.
void BigInt::correct_length(BigInt& num){
    size_t i = num.data_size - 1;
    
    while(num.data[i] == 0 && i > 0){
        num.data_size--;
        i--;
    }
}

BigInt BigInt::add(const BigInt& a, const BigInt& b){
    BigInt tmp(std::max(a.data_size, b.data_size) + 1);
    int32_t carry = 0;
    for(int i = 0; i < tmp.data_size; ++i){
        if(i < a.data_size){
            tmp.data[i] += a.data[i];
        } 

        if(i < b.data_size){
            tmp.data[i] += b.data[i];
        }

        tmp.data[i] += carry;

        if(tmp.data[i] >= tmp.base){
            carry = 1;
            tmp.data[i] -= tmp.base;
        } else {
            carry = 0;
        }
    }
    // Корректируем "формальную" длину (переменную data_size) временного объекта и 
    // возвращаем новый объект, созданный из временного, но уже не занимающий лишней памяти.
    correct_length(tmp);
    return BigInt(tmp);
}

BigInt BigInt::subtract(const BigInt& a, const BigInt& b){
    BigInt tmp(std::max(a.data_size, b.data_size));
    int32_t carry = 0;
    for(int i = 0; i < std::max(a.data_size, b.data_size); ++i){
        tmp.data[i] = a.data[i] - b.data[i] - carry;
        if(tmp.data[i] < 0){
            tmp.data[i] += tmp.base;
            carry = 1;
        }
    }

    correct_length(tmp);
    return BigInt(tmp);
}

BigInt BigInt::mult(const BigInt& a, const BigInt& b){
    BigInt tmp(a.data_size + b.data_size);
    for(size_t i = 0; i < b.data_size; ++i){
        int32_t carry = 0;
        for (size_t j = 0; j < a.data_size; ++j){
            int64_t tmp_mult = tmp.data[i + j];
            tmp_mult += carry + static_cast<int64_t>(a.data[j]) * static_cast<int64_t>(b.data[i]);
            carry = tmp_mult / tmp.base;
            tmp_mult %= tmp.base;
            tmp.data[i + j] = tmp_mult;
        }
        tmp.data[i + a.data_size] += carry;
    }
    correct_length(tmp);
    return BigInt(tmp);
}

bool BigInt::bigger_abs(const BigInt& a, const BigInt& b) {
    if(a.data_size > b.data_size){
        return true;
    } else if(a.data_size < b.data_size){
        return false;
    } else {
        for(size_t i = a.data_size; i != 0; --i){
            if(a.data[i-1] != b.data[i-1]){
                return a.data[i-1] > b.data[i-1];
            }
        }
        return false;
    }
}

bool BigInt::bigger_or_equal_abs(const BigInt& a, const BigInt& b){
    if(a.data_size > b.data_size){
        return true;
    } else if(a.data_size < b.data_size){
        return false;
    } else {
        for(size_t i = a.data_size; i != 0; --i){
            if(a.data[i-1] > b.data[i-1]){
                return true;
            } else if(a.data[i-1] < b.data[i-1]){
                return false;
            }
        }
        return true;
    }
}


// Операторы
//==============================================================================================================

BigInt& BigInt::operator=(const BigInt& other){
    if (&other == this)
    {
        return *this;
    }

    delete[] data;

    data_size = other.data_size;
    is_negative = other.is_negative;
    data = new int32_t[data_size];
    for(size_t i = 0; i < data_size; ++i){
        data[i] = other.data[i];
    }

    return *this;
}

BigInt& BigInt::operator=(BigInt&& other){
    data_size = other.data_size;
    is_negative = other.is_negative;

    delete[] data;
    data = other.data;
    other.data = nullptr;
    return *this;
}

BigInt operator * (const BigInt& a, const BigInt& b){
    if((!a.is_negative && b.is_negative) || (a.is_negative && !b.is_negative)){
        return -BigInt::mult(a, b);
    } else {
        return BigInt::mult(a, b);
    }
}

// Унарный минус
BigInt BigInt::operator - () const {
    BigInt tmp(*this);
    // Исключаем -0
    if(tmp.data_size == 1 && tmp.data[0] == 0){
        return tmp;    
    }
    tmp.is_negative = ! tmp.is_negative;
    return tmp;
}

BigInt operator - (const BigInt& a, const BigInt& b) {
    if(a.is_negative && b.is_negative){
        if(BigInt::bigger_abs(a, b)){
            return -BigInt::subtract(a, b);
        } else {
            return BigInt::subtract(b, a);
        }
    } else if(a.is_negative && !b.is_negative){
        return -BigInt::add(a, b);
    } else if(!a.is_negative && b.is_negative){
        return BigInt::add(a, b);
    } else {
        if(BigInt::bigger_or_equal_abs(a, b)){
            return BigInt::subtract(a, b);
        } else {
            return -BigInt::subtract(b, a);
        }
    }
}

BigInt operator + (const BigInt& a, const BigInt& b) {
    if(a.is_negative && b.is_negative){
        return -BigInt::add(a, b);
    } else if(a.is_negative && !b.is_negative){
        if(BigInt::bigger_abs(a, b)){
            return -BigInt::subtract(a, b);
        } else {
            return BigInt::subtract(b, a);
        }
    } else if(!a.is_negative && b.is_negative){
        if(BigInt::bigger_or_equal_abs(a, b)){
            return BigInt::subtract(a, b);
        } else {
            return -BigInt::subtract(b, a);
        }
    } else {
        return BigInt::add(a,b);
    }
}


BigInt operator + (const BigInt& a, const int& b) {
    BigInt b_tmp(b);
    return a + b_tmp;
}

BigInt operator - (const BigInt& a, const int32_t& b){
    BigInt b_tmp(b);
    return a - b_tmp;
}

BigInt operator * (const BigInt& a, const int32_t& b){
    BigInt b_tmp(b);
    return a * b_tmp;
}


std::ostream& operator << (std::ostream& out, const BigInt& num){
    if(num.is_negative){
        out << '-';
    }
    out << num.data[num.data_size - 1];

    for(size_t i = num.data_size - 1; i != 0; i--){
        std::string current_digit = std::to_string(num.data[i - 1]);
        while(current_digit.size() != num.base_power){
            current_digit = "0" + current_digit;
        }
        out << current_digit; 
    }
    
    return out;
}

bool operator == (const BigInt& a, const BigInt& b){
    if((a.data_size == b.data_size) && (a.is_negative == b.is_negative)){
        for(int i = 0; i < a.data_size; ++i){
            if(a.data[i] != b.data[i]){
                return false;
            }
        }
        return true;
    } else {
        return false; 
    }
}

bool operator != (const BigInt& a, const BigInt& b){
    return !(a == b);
}

bool operator > (const BigInt& a, const BigInt& b){
    if(!a.is_negative && b.is_negative){
        return true;
    } else if(a.is_negative && !b.is_negative){
        return false;
    } else if(!a.is_negative && !b.is_negative){
        return BigInt::bigger_abs(a, b);
    } else {
        return BigInt::bigger_abs(-b, -a);
    }
}   
    
bool operator >= (const BigInt& a, const BigInt& b){
    if(!a.is_negative && b.is_negative){
        return true;
    } else if(a.is_negative && !b.is_negative){
        return false;
    } else if(!a.is_negative && !b.is_negative){
        return BigInt::bigger_or_equal_abs(a, b);
    } else {
        return BigInt::bigger_or_equal_abs(-b, -a);
    }

}

bool operator < (const BigInt& a, const BigInt& b){
    return !(a >= b);
}

bool operator <= (const BigInt& a, const BigInt& b){
    return !(a > b);
}