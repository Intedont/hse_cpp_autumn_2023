#include <iostream>
#include <gtest/gtest.h>

#include "bigint.hpp"

class TestTask : ::testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};


TEST(TestTask, test_constructor){
    BigInt bigint1("4239857348975893478");
    int32_t* arr = bigint1.get_array();
    ASSERT_EQ(bigint1.get_size(), 3);
    ASSERT_EQ(arr[0], 975893478);
    ASSERT_EQ(arr[1], 239857348);
    ASSERT_EQ(arr[2], 4);
    

    BigInt bigint2("239857348975893478");
    arr = bigint2.get_array();
    ASSERT_EQ(bigint2.get_size(), 2);
    ASSERT_EQ(arr[0], 975893478);
    ASSERT_EQ(arr[1], 239857348);
    
    BigInt bigint3("00000000039857348975893478");
    arr = bigint3.get_array();
    ASSERT_EQ(bigint3.get_size(), 2);
    ASSERT_EQ(arr[0], 975893478);
    ASSERT_EQ(arr[1], 39857348);

    BigInt bigint4("0");
    arr = bigint4.get_array();
    ASSERT_EQ(bigint4.get_size(), 1);
    ASSERT_EQ(arr[0], 0);

    BigInt bigint5("1");
    arr = bigint5.get_array();
    ASSERT_EQ(bigint5.get_size(), 1);
    ASSERT_EQ(arr[0], 1);

    BigInt bigint6("1000000000000000000001");
    arr = bigint6.get_array();
    ASSERT_EQ(bigint6.get_size(), 3);
    ASSERT_EQ(arr[0], 1);
    ASSERT_EQ(arr[1], 0);
    ASSERT_EQ(arr[2], 1000);
    
}

TEST(TestTask, test_out){
    BigInt bigint1("4239857348975893478");
    std::cout << bigint1 <<std::endl;
    std::cout << -bigint1 <<std::endl;

    BigInt bigint2("-236782");
    std::cout << bigint2 <<std::endl;
    std::cout << -bigint2 <<std::endl;
    
}



TEST(TestTask, test_int32){
    int32_t b = 1000000000;
    ASSERT_EQ((BigInt("234") + b).as_string(), "1000000234");
    ASSERT_EQ((BigInt("234") - b).as_string(), "-999999766");
    ASSERT_EQ((BigInt("234") * b).as_string(), "234000000000");
}

TEST(TestTask, test_move_constructor){
    BigInt a = 1;    
    BigInt b("123456789012345678901234567890");
    BigInt c = a * b + 2;
    BigInt d;
    d = std::move(c);
    a = d + b;
    ASSERT_EQ(a.as_string(), "246913578024691357802469135782");
}

TEST(TestTask, test_operations){
    std::vector<int64_t> test_numbers{0, 1, 123, 4567, 999999999, 9999999999, 100000000, 1000000000, 1999999999, 2000000000};

    for(size_t i = 0; i < test_numbers.size(); ++i){
        for(size_t j = i; j < test_numbers.size(); ++j){
            BigInt a(std::to_string(test_numbers[i]));
            BigInt b(std::to_string(test_numbers[j]));

            //Тестируем сложение
            ASSERT_EQ((a + b).as_string(), std::to_string(test_numbers[i] + test_numbers[j]));
            ASSERT_EQ((a + (-b)).as_string(), std::to_string(test_numbers[i] + (-test_numbers[j])));
            ASSERT_EQ(((-a) + b).as_string(), std::to_string((-test_numbers[i]) + test_numbers[j]));
            ASSERT_EQ(((-a) + (-b)).as_string(), std::to_string((-test_numbers[i]) + (-test_numbers[j])));
            
            ASSERT_EQ((b + a).as_string(), std::to_string(test_numbers[j] + test_numbers[i]));
            ASSERT_EQ((b + (-a)).as_string(), std::to_string(test_numbers[j] + (-test_numbers[i])));
            ASSERT_EQ(((-b) + a).as_string(), std::to_string((-test_numbers[j]) + test_numbers[i]));
            ASSERT_EQ(((-b) + (-a)).as_string(), std::to_string((-test_numbers[j]) + (-test_numbers[i])));

            //Тестируем вычитание
            ASSERT_EQ((a - b).as_string(), std::to_string(test_numbers[i] - test_numbers[j]));
            ASSERT_EQ((a - (-b)).as_string(), std::to_string(test_numbers[i] - (-test_numbers[j])));
            ASSERT_EQ(((-a) - b).as_string(), std::to_string((-test_numbers[i]) - test_numbers[j]));
            ASSERT_EQ(((-a) - (-b)).as_string(), std::to_string((-test_numbers[i]) - (-test_numbers[j])));

            ASSERT_EQ((b - a).as_string(), std::to_string(test_numbers[j] - test_numbers[i]));
            ASSERT_EQ((b - (-a)).as_string(), std::to_string(test_numbers[j] - (-test_numbers[i])));
            ASSERT_EQ(((-b) - a).as_string(), std::to_string((-test_numbers[j]) - test_numbers[i]));
            ASSERT_EQ(((-b) - (-a)).as_string(), std::to_string((-test_numbers[j]) - (-test_numbers[i])));

            //Тестируем >
            ASSERT_EQ(a > b, test_numbers[i] > test_numbers[j]);
            ASSERT_EQ(a > (-b), test_numbers[i] > (-test_numbers[j]));
            ASSERT_EQ((-a) > b, (-test_numbers[i]) > test_numbers[j]);
            ASSERT_EQ((-a) > (-b), (-test_numbers[i]) > (-test_numbers[j]));
            
            ASSERT_EQ(b > a, test_numbers[j] > test_numbers[i]);
            ASSERT_EQ(b > (-a), test_numbers[j] > (-test_numbers[i]));
            ASSERT_EQ((-b) > a, (-test_numbers[j]) > test_numbers[i]);
            ASSERT_EQ((-b) > (-a), (-test_numbers[j]) > (-test_numbers[i]));
            
            // Тестируем >=
            
            ASSERT_EQ(a >= b, test_numbers[i] >= test_numbers[j]);
            ASSERT_EQ(a >= (-b), test_numbers[i] >= (-test_numbers[j]));
            ASSERT_EQ((-a) >= b, (-test_numbers[i]) >= test_numbers[j]);
            ASSERT_EQ((-a) >= (-b), (-test_numbers[i]) >= (-test_numbers[j]));
            
            ASSERT_EQ(b >= a, test_numbers[j] >= test_numbers[i]);
            ASSERT_EQ(b >= (-a), test_numbers[j] >= (-test_numbers[i]));
            ASSERT_EQ((-b) >= a, (-test_numbers[j]) >= test_numbers[i]);
            ASSERT_EQ((-b) >= (-a), (-test_numbers[j]) >= (-test_numbers[i]));

            // Тестируем < 
            ASSERT_EQ(a < b, test_numbers[i] < test_numbers[j]);
            ASSERT_EQ(a < (-b), test_numbers[i] < (-test_numbers[j]));
            ASSERT_EQ((-a) < b, (-test_numbers[i]) < test_numbers[j]);
            ASSERT_EQ((-a) < (-b), (-test_numbers[i]) < (-test_numbers[j]));
            
            ASSERT_EQ(b < a, test_numbers[j] < test_numbers[i]);
            ASSERT_EQ(b < (-a), test_numbers[j] < (-test_numbers[i]));
            ASSERT_EQ((-b) < a, (-test_numbers[j]) < test_numbers[i]);
            ASSERT_EQ((-b) < (-a), (-test_numbers[j]) < (-test_numbers[i]));

            // Тестируем <= 
            ASSERT_EQ(a <= b, test_numbers[i] <= test_numbers[j]);
            ASSERT_EQ(a <= (-b), test_numbers[i] <= (-test_numbers[j]));
            ASSERT_EQ((-a) <= b, (-test_numbers[i]) <= test_numbers[j]);
            ASSERT_EQ((-a) <= (-b), (-test_numbers[i]) <= (-test_numbers[j]));
            
            ASSERT_EQ(b <= a, test_numbers[j] <= test_numbers[i]);
            ASSERT_EQ(b <= (-a), test_numbers[j] <= (-test_numbers[i]));
            ASSERT_EQ((-b) <= a, (-test_numbers[j]) <= test_numbers[i]);
            ASSERT_EQ((-b) <= (-a), (-test_numbers[j]) <= (-test_numbers[i]));

            // Тестируем ==
            ASSERT_EQ(a == b, test_numbers[i] == test_numbers[j]);
            ASSERT_EQ(a == (-b), test_numbers[i] == (-test_numbers[j]));
            ASSERT_EQ((-a) == b, (-test_numbers[i]) == test_numbers[j]);
            ASSERT_EQ((-a) == (-b), (-test_numbers[i]) == (-test_numbers[j]));
            
            ASSERT_EQ(b == a, test_numbers[j] == test_numbers[i]);
            ASSERT_EQ(b == (-a), test_numbers[j] == (-test_numbers[i]));
            ASSERT_EQ((-b) == a, (-test_numbers[j]) == test_numbers[i]);
            ASSERT_EQ((-b) == (-a), (-test_numbers[j]) == (-test_numbers[i]));

            // Тестируем !=
            ASSERT_EQ(a != b, test_numbers[i] != test_numbers[j]);
            ASSERT_EQ(a != (-b), test_numbers[i] != (-test_numbers[j]));
            ASSERT_EQ((-a) != b, (-test_numbers[i]) != test_numbers[j]);
            ASSERT_EQ((-a) != (-b), (-test_numbers[i]) != (-test_numbers[j]));
            
            ASSERT_EQ(b != a, test_numbers[j] != test_numbers[i]);
            ASSERT_EQ(b != (-a), test_numbers[j] != (-test_numbers[i]));
            ASSERT_EQ((-b) != a, (-test_numbers[j]) != test_numbers[i]);
            ASSERT_EQ((-b) != (-a), (-test_numbers[j]) != (-test_numbers[i]));
        }
    }    

    
}

// Тестируем умножение отдельно, так как результат не влезает в int64
TEST(TestTask, test_mult){
    
    BigInt bigint1("0");
    BigInt bigint2("0");
    BigInt bigint3("1");
    ASSERT_EQ((bigint1 * bigint2).as_string(), "0");
    ASSERT_EQ((bigint1 * bigint2).get_size(), 1);
    ASSERT_EQ((bigint1 * bigint3).as_string(), "0");
    ASSERT_EQ((bigint1 * bigint3).get_size(), 1);
    ASSERT_EQ((bigint3 * bigint1).as_string(), "0");
    ASSERT_EQ((bigint3 * bigint1).get_size(), 1);
    ASSERT_EQ((bigint3 * bigint3).as_string(), "1");
    ASSERT_EQ((bigint3 * bigint3).get_size(), 1);

    // Тестирование чисел, влезающих в один элемент массива
    BigInt bigint4("999999999");
    ASSERT_EQ((bigint4 * bigint1).as_string(), "0");
    ASSERT_EQ((bigint4 * bigint3).as_string(), "999999999");
    ASSERT_EQ((bigint4 * bigint4).as_string(), "999999998000000001");
    
    ASSERT_EQ((bigint4 * bigint1).get_size(), 1);
    ASSERT_EQ((bigint4 * bigint3).get_size(), 1);
    ASSERT_EQ((bigint4 * bigint4).get_size(), 2);

    BigInt bigint5("100000000");
    ASSERT_EQ((bigint5 * bigint1).as_string(), "0");
    ASSERT_EQ((bigint5 * bigint3).as_string(), "100000000");
    ASSERT_EQ((bigint5 * bigint4).as_string(), "99999999900000000");
    ASSERT_EQ((bigint5 * bigint5).as_string(), "10000000000000000"); 
    
    ASSERT_EQ((bigint5 * bigint1).get_size(), 1);
    ASSERT_EQ((bigint5 * bigint3).get_size(), 1);
    ASSERT_EQ((bigint5 * bigint4).get_size(), 2);


    // Тестирование числа, требующего два элемента массива
    BigInt bigint6("9999999999");
    ASSERT_EQ((bigint6 * bigint1).as_string(), "0");
    ASSERT_EQ((bigint6 * bigint3).as_string(), "9999999999");
    ASSERT_EQ((bigint6 * bigint6).as_string(), "99999999980000000001");
    ASSERT_EQ((bigint6 * bigint4).as_string(), "9999999989000000001");
    ASSERT_EQ((bigint6 * bigint5).as_string(), "999999999900000000");

    ASSERT_EQ((bigint6 * bigint1).get_size(), 1);
    ASSERT_EQ((bigint6 * bigint3).get_size(), 2);
    ASSERT_EQ((bigint6 * bigint6).get_size(), 3);
    ASSERT_EQ((bigint6 * bigint4).get_size(), 3);
    ASSERT_EQ((bigint6 * bigint5).get_size(), 2);

    BigInt bigint7("734908570982347819656780898912347895747981278358143097598437987912365784168767813678561876893457698171698749365743687910329875651236576513264757856132786856347697123657812");
    ASSERT_EQ((bigint7 * bigint1).as_string(), "0");
    ASSERT_EQ((bigint7 * bigint7).as_string(), "540090607703316563738172721230407623459486007620559093666702271742435131670522673018408621584464316401067300442817526324564785629415794396871194321316026988852855068940365217744173537443654600653013802412948145793454385429476317037728177850703943228354967730775072525532372402211417146218706134462238847551834317444565440438059809182468627344");
    ASSERT_EQ((bigint7 * bigint4).as_string(), "734908570247439248674433079255566996835633382610161819240294890313927796256402029509794063214895821278241051194044938544586187740906700862028181342868029000214910267310114876342188");
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
