#include <iostream>
#include <gtest/gtest.h>

#include "parser.cpp"

class TestTask : ::testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};


TEST(TestTask, test_splitting){
    
    TokenParser parser;
    
    std::string texts[4] = {"   one 2 3  four five", "one\t2\t3\tfour\tfive", "one\n2\n3\nfour\nfive", "one 2\n3\tfour\nfive"};

    std::vector<std::string> text_true = {"one", "2", "3", "four", "five"};

    for(auto& text : texts){
        auto tokens = parser.parse(text);
        for(int i = 0; i < tokens.size(); ++i){
            ASSERT_EQ(tokens[i], text_true[i]);
        }
    }
}

TEST(TestTask, test_type_definition){
    TokenParser parser;

    parser.setStartCallback([] (const std::string& line) { std::cout << "Hello. Parsing: " << line <<std::endl; });
    parser.setDigitTokenCallback([] (long long unsigned int& token) { token = token * 2; });
    parser.setStringTokenCallback([] (std::string& token) { token = "string " + token; });
    parser.setEndCallback([] () { std::cout << "Bye" <<std::endl; });
    
    std::vector<std::string> text_true = {"string one", "4", "string -3", "string 4.5", "string 500000000000000000000"};

    auto tokens = parser.parse("one 2 -3 4.5 500000000000000000000");

    for(int i = 0; i < tokens.size(); ++i){
        ASSERT_EQ(tokens[i], text_true[i]);
        }
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}