#include <iostream>
#include "matrix.hpp"

Matrix::Row::Row() {}

Matrix::Row::Row(size_t c) : columns{new int32_t[c]}, row_size{c} {
    for(size_t i = 0; i < c; ++i){
        columns[i] = 0;
    }
}

// Копирующий конструктор для строк
Matrix::Row::Row(const Row& r) : columns{new int32_t[r.row_size]}, row_size{r.row_size} {
    for(size_t i = 0; i < r.row_size; ++i){
        columns[i] = r.columns[i];
    }
}

Matrix::Row::~Row() {
    delete[] columns;
}

int32_t& Matrix::Row::operator [] (size_t index) const {   
    if(index < row_size){
        return columns[index];
    }
    else{
        throw std::out_of_range("");
    }
}

Matrix::Matrix(size_t r, size_t c) : rows{new Row*[r]}, n_rows{r}, n_columns{c} {
    for(size_t i = 0; i < r; ++i){
        rows[i] = new Row(n_columns);
    }
}

// Копирующий конструктор для матрицы
Matrix::Matrix(const Matrix& m) : rows{new Row*[m.n_rows]}, n_rows{m.n_rows}, n_columns{m.n_columns} {
    for(size_t i = 0; i < n_rows; ++i){
        rows[i] = new Row(m[i]);
    }
}

// Оператор копирования. 
// В описании задачи его перегрузка не требуется, но без него ничего не работает. Выполняет deep copy.
void Matrix::operator= (const Matrix& m) {
    // Очищаем память, выделенную под матрицу левого операнда
    for(size_t i = 0; i < n_rows; ++i){
        delete rows[i];
    }
    delete[] rows;

    n_rows = m.n_rows;
    n_columns = m.n_columns;
    // Инициализируем новую память под матрицу, размером с матрицу правого операнда, и копируем.
    rows = new Row*[n_rows];

    for(size_t i = 0; i < n_rows; ++i){
        rows[i] = new Row(m[i]);
    }    
}

Matrix::~Matrix() {
    for(size_t i = 0; i < n_rows; ++i){
        delete rows[i];
    }
    delete[] rows;
}

size_t Matrix::get_rows() const { return n_rows; }

size_t Matrix::get_columns() const { return n_columns; }

Matrix::Row& Matrix::operator [] (size_t index) const
{
    if(index < n_rows){
        return *rows[index];
    } else {
        throw std::out_of_range("Out of range index");
    }
}

Matrix& Matrix::operator *= (int32_t num){
    for(size_t x = 0; x < n_rows; ++x){
        for(size_t y = 0; y < n_columns; ++y){
            (*rows[x])[y] = (*rows[x])[y] * num;
        }
    }
    return *this;
}

Matrix operator + (const Matrix& first, const Matrix& second) {
    if(first.get_rows() == second.get_rows() && first.get_columns() == second.get_columns()){
        Matrix mat_sum(first.get_rows(), first.get_columns());
        for(size_t x = 0; x < mat_sum.get_rows(); ++x){
            for(size_t y = 0; y < mat_sum.get_columns(); ++y){
                mat_sum[x][y] = first[x][y] + second[x][y];
            }
        }
        return mat_sum;
    } else {
        throw std::exception();
    }
}

std::ostream& operator << (std::ostream& out, const Matrix& m) {
    for(size_t x = 0; x < m.n_rows; ++x){
        for(size_t y = 0; y < m.n_columns; ++y){
            out << m[x][y] << " ";
        }
        out << std::endl;
    }
    return out;
}

bool operator == (const Matrix& first, const Matrix& second) {
    if(first.get_rows() == second.get_rows() && first.get_columns() == second.get_columns()){
        for(size_t x = 0; x < first.get_rows(); ++x){
            for(size_t y = 0; y < first.get_columns(); ++y){
                if(first[x][y] != second[x][y]){
                    return false;
                }
            }
        }
        return true;
    } else {
        return false;
    }
}

bool operator != (const Matrix& first, const Matrix& second) {
    return !(first == second);
}