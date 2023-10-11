#include <stdint.h>
#include <cstddef>

class Matrix
{
    class Row
    {
    private:
        int32_t* columns;
        size_t row_size;
    public:
        Row();
        Row(size_t c);
        Row(const Row& r);
        ~Row();
        int32_t& operator [] (size_t index) const;
    };
private:
    Row** rows;
    size_t n_rows;
    size_t n_columns;
public:
    Matrix(size_t r, size_t c);
    Matrix(const Matrix&);
    ~Matrix();
    
    size_t get_rows() const;
    size_t get_columns() const;

    void operator=(const Matrix& m); // оператор копирования
    Row& operator [] (size_t index) const;
    Matrix& operator *= (int32_t num);
    friend std::ostream& operator << (std::ostream& out, const Matrix& m);
};

Matrix operator + (const Matrix& first, const Matrix& second);
bool operator == (const Matrix& first, const Matrix& second);
bool operator != (const Matrix& first, const Matrix& second);