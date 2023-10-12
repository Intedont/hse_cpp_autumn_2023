#include <iostream>
#include <gtest/gtest.h>

#include "matrix.hpp"

class TestTask : ::testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

// Тестирование геттеров
TEST(TestTask, test_getters){
    Matrix m(5, 4);
    ASSERT_EQ(m.get_columns(), 4);
    ASSERT_EQ(m.get_rows(), 5);
    
    Matrix m2(0, 0);
    ASSERT_EQ(m2.get_columns(), 0);
    ASSERT_EQ(m2.get_rows(), 0);
}

// Тестирование операторов получения элемента [][]
TEST(TestTask, test_member_access_operator){
    Matrix m(5, 4);

    // Заполнение матрицы числами
    for(size_t i = 0; i < m.get_rows(); ++i){
        for(size_t j = 0; j < m.get_columns(); ++j){
            m[i][j] = i * j;
        }
    }

    // Проверка корректности заполнения
    for(size_t i = 0; i < m.get_rows(); ++i){
        for(size_t j = 0; j < m.get_columns(); ++j){
            ASSERT_EQ(m[i][j], i * j);
        }
    }

    Matrix m1(0,0);

    ASSERT_THROW(m1[0][0], std::out_of_range);
    ASSERT_THROW(m[5][4], std::out_of_range);
    ASSERT_THROW(m[5][0], std::out_of_range);
    ASSERT_THROW(m[0][4], std::out_of_range);
}

// Тестирование оператора *=
TEST(TestTask, test_mult_assignment_operator){
    Matrix m(6, 6);

    for(size_t i = 0; i < m.get_rows(); ++i){
        for(size_t j = 0; j < m.get_columns(); ++j){
            m[i][j] = i * j;
        }
    }

    m *= 5;

    for(size_t i = 0; i < m.get_rows(); ++i){
        for(size_t j = 0; j < m.get_columns(); ++j){
            ASSERT_EQ(m[i][j], i * j * 5);
        }
    }

    m *= -1;

    for(size_t i = 0; i < m.get_rows(); ++i){
        for(size_t j = 0; j < m.get_columns(); ++j){
            ASSERT_EQ(m[i][j], - i * j * 5);
        }
    }

    m *= 0;

    for(size_t i = 0; i < m.get_rows(); ++i){
        for(size_t j = 0; j < m.get_columns(); ++j){
            ASSERT_EQ(m[i][j], 0);
        }
    }

    // Проверка, что ничего не упадет
    Matrix m1(0,0);
    m1 *= 5;
}

// Тестирование оператора <<
TEST(TestTask, test_ostream_operator){
    Matrix m(2, 4);
    m[1][3] = 7;
    m[0][0] = 9;

    std::ostringstream output;
    output << m;
    ASSERT_EQ( output.str(), "9 0 0 0 \n0 0 0 7 \n" );

    Matrix m1(3, 1);
    std::ostringstream output1;
    output1 << m1;
    ASSERT_EQ( output1.str(), "0 \n0 \n0 \n" );

    Matrix m2(0, 0);
    std::ostringstream output2;
    output2 << m2;
    ASSERT_EQ( output2.str(), "" );
}

// Тестирование оператора копирования и копирующих конструкторов
TEST(TestTask, test_assignment_and_copy_constructor){
    Matrix m1(2, 4);
    Matrix m2(4, 7);

    m1 = m2;
    
    ASSERT_EQ(m1.get_columns(), 7);
    ASSERT_EQ(m1.get_rows(), 4);
    ASSERT_EQ(m2.get_columns(), 7);
    ASSERT_EQ(m2.get_rows(), 4);

    std::ostringstream output;
    output << m1;
    ASSERT_EQ(output.str(), "0 0 0 0 0 0 0 \n0 0 0 0 0 0 0 \n0 0 0 0 0 0 0 \n0 0 0 0 0 0 0 \n" );

    for(size_t i = 0; i < m1.get_rows(); ++i){
        for(size_t j = 0; j < m1.get_columns(); ++j){
            m1[i][j] = i * j;
        }
    }

    m2 = m1;

    for(size_t i = 0; i < m1.get_rows(); ++i){
        for(size_t j = 0; j < m1.get_columns(); ++j){
            ASSERT_EQ(m2[i][j], i * j);
        }
    }

    m1 = Matrix(3, 1);

    ASSERT_EQ(m1.get_columns(), 1);
    ASSERT_EQ(m1.get_rows(), 3);

    output.str("");
    output.clear();
    output << m1;
    ASSERT_EQ(output.str(), "0 \n0 \n0 \n" );

    m1 = Matrix(0,0);
    ASSERT_EQ(m1.get_columns(), 0);
    ASSERT_EQ(m1.get_rows(), 0);

    output.str("");
    output.clear();
    output << m1;
    ASSERT_EQ(output.str(), "" );

    Matrix m3 = Matrix(5, 5);
    ASSERT_EQ(m3.get_columns(), 5);
    ASSERT_EQ(m3.get_rows(), 5);

    output.str("");
    output.clear();
    output << m3;
    ASSERT_EQ(output.str(), "0 0 0 0 0 \n0 0 0 0 0 \n0 0 0 0 0 \n0 0 0 0 0 \n0 0 0 0 0 \n" );

    // Тестирование копирующих конструкторов
    m1 = m2;
    Matrix m4(m2);
    Matrix m5 = Matrix(m1);

    for(size_t i = 0; i < m4.get_rows(); ++i){
        for(size_t j = 0; j < m4.get_columns(); ++j){
            ASSERT_EQ(m4[i][j], m5[i][j]);
        }
    }
}

// Тестирование оператора сложения
TEST(TestTask, test_add_operator){
    Matrix m1(2, 4);
    Matrix m2(2, 4);

    Matrix m3 = m1 + m2;

    std::ostringstream output;
    output << m3;
    ASSERT_EQ( output.str(), "0 0 0 0 \n0 0 0 0 \n" );
    
    
    for(size_t i = 0; i < m1.get_rows(); ++i){
        for(size_t j = 0; j < m1.get_columns(); ++j){
            m1[i][j] = i * m1.get_columns() + j;
            m2[i][j] = i * m1.get_columns() + j - 3;
        }
    }

    Matrix m4(2, 4);
    m3 = m1 + m2;
    m4 = m2 + m1;

    for(size_t i = 0; i < m1.get_rows(); ++i){
        for(size_t j = 0; j < m1.get_columns(); ++j){
            ASSERT_EQ(m3[i][j], (i * m1.get_columns() + j) + (i * m1.get_columns() + j - 3));
            ASSERT_EQ(m4[i][j], (i * m1.get_columns() + j) + (i * m1.get_columns() + j - 3));
        }
    }

    Matrix m5(0, 0);
    ASSERT_THROW(m5 + m1, std::exception);
    ASSERT_THROW(m1 + m5, std::exception);

    Matrix m6(10, 10);
    ASSERT_THROW(m6 + m1, std::exception);
    ASSERT_THROW(m1 + m6, std::exception);

}

// Тестирование операторов == и !=
TEST(TestTask, test_equal_operator){
    // Тестирование ==
    Matrix m1(10, 5);
    Matrix m2(10, 5);

    ASSERT_EQ(m1 == m2, true);
    m1[9][4] = -3;
    ASSERT_EQ(m1 == m2, false);

    m1 = Matrix(9, 5);
    ASSERT_EQ(m1 == m2, false);

    m1 = Matrix(0, 0);
    m2 = Matrix(0, 0);
    ASSERT_EQ(m1 == m2, true);

    // Тестирование !=
    m1 = Matrix(10, 5);
    m2 = Matrix(10, 5);

    ASSERT_EQ(m1 != m2, false);
    m1[9][4] = -3;
    ASSERT_EQ(m1 != m2, true);

    m1 = Matrix(9, 5);
    ASSERT_EQ(m1 != m2, true);

    m1 = Matrix(0, 0);
    m2 = Matrix(0, 0);
    ASSERT_EQ(m1 != m2, false);

}

// Тестируем еще два краевых случая на утечку памяти.
TEST(TestTask, test_some_bounds){
    Matrix m1(0,1);
    Matrix m2(1,0);
    Matrix m3(4, 4);

    ASSERT_THROW(m1[0][0], std::out_of_range);
    ASSERT_THROW(m2[0][0], std::out_of_range);
    ASSERT_THROW((m1 + m2), std::exception);
    ASSERT_THROW((m1 + m3), std::exception);
    ASSERT_THROW((m2 + m3), std::exception);
    ASSERT_NO_THROW(m1 *= 2);
    ASSERT_NO_THROW(m2 *= 2);
    ASSERT_EQ(m1 == m2, false);
    ASSERT_EQ(m1 == m3, false);
    ASSERT_EQ(m2 == m3, false);
    ASSERT_EQ(m1 == m1, true);
    ASSERT_EQ(m2 == m2, true);

    Matrix m4 = m1;
    Matrix m5 = m2;
    m1 = m4;
    m2 = m5;

    std::cout << m1 << std::endl;
    std::cout << m2 << std::endl;
    std::cout << m4 << std::endl;
    std::cout << m5 << std::endl;
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}