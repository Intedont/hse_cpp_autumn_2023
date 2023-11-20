#include <stdint.h>
#include "string_format.cpp"

#include <iostream>
#include <gtest/gtest.h>


class TestTask : ::testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

// Тестовая структура с оператором вывода
struct TestStruct{
    int first;
    std::string second;
    double third;

    TestStruct(int f, std::string s, double t) : first{f}, second{s}, third{t}{
    }

    friend std::ostream& operator<<(std::ostream& out, TestStruct& ts);
};

std::ostream& operator<<(std::ostream& out, TestStruct& ts){
    out << ts.first << " " << ts.second << " " << ts.third;
    return out;
}


TEST(TestTask, test_common){
    
    auto text = format("this is {0} test in {1} homework", "first", "sixth");
    ASSERT_EQ(text, "this is first test in sixth homework");
    
    text = format("this is {1} test in {0} homework", "sixth", "first");
    ASSERT_EQ(text, "this is first test in sixth homework");

    text = format("this is {1} test in {0} homework", 6, "first");
    ASSERT_EQ(text, "this is first test in 6 homework");

    text = format("this is {1} test in {0} homework", "}", "{");
    ASSERT_EQ(text, "this is { test in } homework");

    text = format("{0} this is test in homework {1}", 6, "first");
    ASSERT_EQ(text, "6 this is test in homework first");

    // Тестирование вывода в строку кастомного объекта
    TestStruct ts(4567, "this is my string", -46.81);
    text = format("{0} this is {2} test in {1} homework {3}", ts, 6, "first", ts);
    ASSERT_EQ(text, "4567 this is my string -46.81 this is first test in 6 homework 4567 this is my string -46.81");
}

TEST(TestTask, test_exceptions){
    
    // Тестируем выбрасываение исключения при использовании скобок вне контекста
    ASSERT_THROW(format("this is {0} test in {1} hom{ework", "first", "sixth"), SymbolIsReserved);
    ASSERT_THROW(format("this is {0} test in {1} hom}ework", "first", "sixth"), SymbolIsReserved);
    ASSERT_THROW(format("{this is {0} test in {1} homework", "first", "sixth"), SymbolIsReserved);
    ASSERT_THROW(format("this is {0} test in {1} homework{", "first", "sixth"), SymbolIsReserved);
    ASSERT_THROW(format("this is {0} test in {1} homework}", "first", "sixth"), SymbolIsReserved);
    ASSERT_THROW(format("}this is {0} test in {1} homework", "first", "sixth"), SymbolIsReserved);
    ASSERT_THROW(format("this{{{{ is {0} test in {1} homework", "first", "sixth"), SymbolIsReserved);
    
    
    // Если аргументов меньше, чем требуется в строке
    ASSERT_THROW(format("this is {0} test in {5} homework", "first", "sixth"), ArgumentNotFound);
    // Не требуем выбрасывания исключения, если число аргументов больше, чем требуется в строке
    ASSERT_NO_THROW(format("this is {1} test in {0} homework", 6, "first", "another"));
    
    // Тестируем кривую передачу номеров аргументов
    ASSERT_THROW(format("this is {ьмао} test in {5} homework", "first", "sixth"), ConvertError);
    ASSERT_THROW(format("this is {1.2} test in {5} homework", "first", "sixth"), ConvertError);

}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}