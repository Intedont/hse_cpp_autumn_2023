#include <stdint.h>
#include <sstream>
#include "serializer.cpp"

#include <iostream>
#include <gtest/gtest.h>


struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    void serialize(Serializer& serializer)
    {
        serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};

struct Data_custom
{
    uint64_t a;

    template <class Serializer>
    void serialize(Serializer& serializer)
    {
        serializer(a);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a);
    }
};

class TestTask : ::testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};


TEST(TestTask, test_common){
    Data x { 1, true, 2 };
    
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    std::cout << stream.str() <<std::endl;
    
    Data y { 0, false, 0 };

    Deserializer deserializer(stream);
    Error err = deserializer.load(y);

    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(x.a, y.a);
    ASSERT_EQ(x.b, y.b);
    ASSERT_EQ(x.c, y.c);
}

// Тест сериализации
TEST(TestTask, test_serializer){
    Data x { 1732489, false, 0 };
    
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    ASSERT_EQ(stream.str(), "1732489 false 0 ");


    Data_custom y { 1732489 };
    
    stream.str("");
    stream.clear();
    serializer.save(y);
    ASSERT_EQ(stream.str(), "1732489 ");
}


// Тест десериализации
TEST(TestTask, test_deserializer){
    Data x { 0, false, 0 };
    
    // Проверка на некорректные значения в потоке
    std::stringstream stream;
    stream << "vhufidh -4 5";
    Deserializer deserializer(stream);
    Error err = deserializer.load(x);
    ASSERT_EQ(err, Error::CorruptedArchive);

    // Проверка на работоспособность 
    stream.str("5 true 322");
    stream.clear();
    err = deserializer.load(x);
    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(x.a, 5);
    ASSERT_EQ(x.b, true);
    ASSERT_EQ(x.c, 322);

    // Проверка на недостаточную длину
    stream.str("5, true");
    stream.clear();
    err = deserializer.load(x);
    ASSERT_EQ(err, Error::CorruptedArchive);

    stream.str("");
    stream.clear();
    err = deserializer.load(x);
    ASSERT_EQ(err, Error::CorruptedArchive);

    // Проверка на слишком длинную строку
    stream.str("123 true 79 7832 false false");
    stream.clear();
    err = deserializer.load(x);
    ASSERT_EQ(err, Error::NoError);

    // Проверка на слишком большие uint
    stream.str("123789423327648327746327864876238761875278453278578187680000 true 1235676158651275172534651265346215674512657");
    stream.clear();
    err = deserializer.load(x);
    ASSERT_EQ(err, Error::UintOutOfRange);

    // Проверка правильного вызова перегрузки
    Data_custom y {0};
    stream.str("123");
    stream.clear();
    err = deserializer.load(y);
    ASSERT_EQ(y.a, 123);
    ASSERT_EQ(err, Error::NoError);
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}