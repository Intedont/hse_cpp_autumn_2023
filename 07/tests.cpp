#include <stdint.h>
#include "avl_container.cpp"

#include <iostream>
#include <gtest/gtest.h>

#include <algorithm>


class TestTask : ::testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

// Тестирование алгоритмов поворота, балансировки дерева, а также вставки новых узлов (метод insert)
TEST(TestTask, test_structure_and_insertion){
    using key = const int;
    using value = std::string;

    // Тестирование построения и балансировки дерева в ту сторону
    AVL<int, std::string, std::less<key>, DumbAllocator<std::pair<key, value>>> mytree;
    ASSERT_EQ(mytree.empty(), true);
    mytree.insert(std::pair<key, value>(1, "something"));
    ASSERT_EQ(mytree.print().str(), "1");
    mytree.insert(std::pair<key, value>(2, "something2"));
    ASSERT_EQ(mytree.print().str(), "12");
    mytree.insert(std::pair<key, value>(3, "something3"));
    ASSERT_EQ(mytree.print().str(), "213");
    mytree.insert(std::pair<key, value>(4, "something4"));
    ASSERT_EQ(mytree.print().str(), "2134");
    mytree.insert(std::pair<key, value>(5, "something5"));
    ASSERT_EQ(mytree.print().str(), "21435");
    mytree.insert(std::pair<key, value>(6, "something6"));
    ASSERT_EQ(mytree.print().str(), "421356");
    mytree.insert(std::pair<key, value>(7, "something7"));
    ASSERT_EQ(mytree.print().str(), "4213657");
    mytree.insert(std::pair<key, value>(8, "something8"));
    ASSERT_EQ(mytree.print().str(), "42136578");
    mytree.insert(std::pair<key, value>(9, "something9"));
    ASSERT_EQ(mytree.print().str(), "421365879");
    mytree.insert(std::pair<key, value>(10, "something10"));
    ASSERT_EQ(mytree.print().str(), "42138657910");
    mytree.insert(std::pair<key, value>(11, "something11"));
    ASSERT_EQ(mytree.print().str(), "4213865710911");
    mytree.insert(std::pair<key, value>(12, "something12"));
    ASSERT_EQ(mytree.print().str(), "842136571091112");

    // В обратную сторону
    AVL<int, std::string, std::less<key>, DumbAllocator<std::pair<key, value>>> mytree_reversed;
    mytree_reversed.insert(std::pair<key, value>(12, "something12"));
    ASSERT_EQ(mytree_reversed.print().str(), "12");
    mytree_reversed.insert(std::pair<key, value>(11, "something11"));
    ASSERT_EQ(mytree_reversed.print().str(), "1211");
    mytree_reversed.insert(std::pair<key, value>(10, "something10"));
    ASSERT_EQ(mytree_reversed.print().str(), "111012");
    mytree_reversed.insert(std::pair<key, value>(9, "something9"));
    ASSERT_EQ(mytree_reversed.print().str(), "1110912");
    mytree_reversed.insert(std::pair<key, value>(8, "something8"));
    ASSERT_EQ(mytree_reversed.print().str(), "11981012");
    mytree_reversed.insert(std::pair<key, value>(7, "something7"));
    ASSERT_EQ(mytree_reversed.print().str(), "987111012");
    mytree_reversed.insert(std::pair<key, value>(6, "something6"));
    ASSERT_EQ(mytree_reversed.print().str(), "9768111012");
    mytree_reversed.insert(std::pair<key, value>(5, "something5"));
    ASSERT_EQ(mytree_reversed.print().str(), "97658111012");
    mytree_reversed.insert(std::pair<key, value>(4, "something4"));
    ASSERT_EQ(mytree_reversed.print().str(), "975468111012");
    mytree_reversed.insert(std::pair<key, value>(3, "something3"));
    ASSERT_EQ(mytree_reversed.print().str(), "9543768111012");
    mytree_reversed.insert(std::pair<key, value>(2, "something2"));
    ASSERT_EQ(mytree_reversed.print().str(), "95324768111012");
    mytree_reversed.insert(std::pair<key, value>(1, "something1"));
    ASSERT_EQ(mytree_reversed.print().str(), "532149768111012");

    
    // Тестирование вставки в середину на float числах
    AVL<const float, std::string, std::less<const float>, DumbAllocator<std::pair<const float, value>>> mytree_float;
    mytree_float.insert(std::pair<const float, value>(7.9, "something8"));
    ASSERT_EQ(mytree_float.print().str(), "7.9");
    mytree_float.insert(std::pair<const float, value>(2.4, "something7"));
    ASSERT_EQ(mytree_float.print().str(), "7.92.4");
    mytree_float.insert(std::pair<const float, value>(3.1432, "something6"));
    ASSERT_EQ(mytree_float.print().str(), "3.14322.47.9");
    //mytree_float.insert(std::pair<const float, value>(float(3.17), "something5"));
    //ASSERT_EQ(mytree_float.print().str(), "3.14322.47.93.17");
    /*mytree_float.insert(std::pair<const float, value>(3.16, "something6"));
    ASSERT_EQ(mytree_float.print().str(), "3.142.43.173.167.9");
    mytree_float.insert(std::pair<const float, value>(3.18, "something6"));
    ASSERT_EQ(mytree_float.print().str(), "3.173.142.43.167.93.18");*/

}

// Тестирование итераторов
TEST(TestTask, test_iterators){
    // Тестирование оператора ++
    std::vector<int> results;
    std::vector<int> truths{1,2,3,4,5,6};
    AVL<int, std::string, std::less<int>, DumbAllocator<std::pair<const int, std::string>>> mytree{{1, "amogus1"}, {2, "amogus2"}, {3, "amogus3"}, {4, "amogus4"}, {5, "amogus5"}, {6, "amogus6"}};

    for (auto it = mytree.begin(); it != mytree.end(); ++it){
        results.push_back((*it).data.first);
    }

    ASSERT_EQ(results, truths);

    //Тестирование оператора --
    results.clear();
    truths.clear();
    truths = std::vector<int>{6,5,4,3,2};

    for (auto it = --mytree.end(); it != mytree.begin(); --it){
        results.push_back((*it).data.first);
    }
    ASSERT_EQ(results, truths);

    // Тестирование с одним элементом
    results.clear();
    truths.clear();
    truths = std::vector<int>{1};
    AVL<int, std::string, std::less<int>, DumbAllocator<std::pair<const int, std::string>>> newtree{{1, "amogus1"}};

    for (auto it = newtree.begin(); it != newtree.end(); ++it){
        results.push_back((*it).data.first);
    }

    ASSERT_EQ(results, truths);

    // Тестирование итерации по пустому дереву
    results.clear();
    truths.clear();
    truths = std::vector<int>();
    AVL<int, std::string, std::less<int>, DumbAllocator<std::pair<const int, std::string>>> newtree2;

    for (auto it = newtree2.begin(); it != newtree2.end(); ++it){
        results.push_back((*it).data.first);
    }

    ASSERT_EQ(results, truths);
    
    // Тестирование reverse итератора
    results.clear();
    truths.clear();
    truths = std::vector<int>{6,5,4,3,2,1};
    AVL<int, std::string, std::less<int>, DumbAllocator<std::pair<const int, std::string>>> mytree3{{1, "amogus1"}, {2, "amogus2"}, {3, "amogus3"}, {4, "amogus4"}, {5, "amogus5"}, {6, "amogus6"}};

    for (auto it = mytree3.rbegin(); it != mytree3.rend(); ++it){
        results.push_back((*it).data.first);
    }

    ASSERT_EQ(results, truths);
}

// Тестирование метода очистки дерева clear
TEST(TestTask, test_clear){
    std::vector<int> results;
    std::vector<int> truths{1,2,3,4,5,6};
    AVL<int, std::string, std::less<int>, DumbAllocator<std::pair<const int, std::string>>> mytree{{1, "amogus1"}, {2, "amogus2"}, {3, "amogus3"}, {4, "amogus4"}, {5, "amogus5"}, {6, "amogus6"}};

    mytree.clear();
    // Очистка пустого дерева
    mytree.clear();
    
    ASSERT_EQ(mytree.empty(), true);

    mytree.insert(std::pair<int, std::string>(2, "amogus root"));
    ASSERT_EQ(mytree.size(), 1);
}

// Тестирование метода erase
TEST(TestTask, test_erase){
    AVL<int, std::string, std::less<int>, DumbAllocator<std::pair<const int, std::string>>> mytree{{1, "amogus1"}, 
    {2, "amogus2"}, {3, "amogus3"}, {4, "amogus4"}, {5, "amogus5"}, {6, "amogus6"}, {7, "amogus7"}, {8, "amogus8"},
    {9, "amogus9"}, {10, "amogus10"}};

    mytree.erase(6);
    ASSERT_EQ(mytree.print().str(), "4213875910");

    // Проверка, что дерево не сломалось и нормально итерируется
    std::vector<int> results;
    std::vector<int> truths{1,2,3,4,5,7,8,9,10};
    for (auto it = mytree.begin(); it != mytree.end(); ++it){
        results.push_back((*it).data.first);
    }
    ASSERT_EQ(results, truths);

    //---------------------------------------
    mytree.erase(5);
    ASSERT_EQ(mytree.print().str(), "421387910");

    results.clear();
    truths = std::vector<int>{1,2,3,4,7,8,9,10};
    for (auto it = mytree.begin(); it != mytree.end(); ++it){
        results.push_back((*it).data.first);
    }
    ASSERT_EQ(results, truths);

    //---------------------------------------
    // Удаляем самый большой элемент
    mytree.erase(10);
    ASSERT_EQ(mytree.print().str(), "4213879");

    results.clear();
    truths = std::vector<int>{1,2,3,4,7,8,9};
    for (auto it = mytree.begin(); it != mytree.end(); ++it){
        results.push_back((*it).data.first);
    }
    ASSERT_EQ(results, truths);

    //---------------------------------------
    // Удаляем самый малый
    mytree.erase(1);
    ASSERT_EQ(mytree.print().str(), "423879");

    results.clear();
    truths = std::vector<int>{2,3,4,7,8,9};
    for (auto it = mytree.begin(); it != mytree.end(); ++it){
        results.push_back((*it).data.first);
    }
    ASSERT_EQ(results, truths);

    //---------------------------------------
    // Удаляем корень
    mytree.erase(4);
    ASSERT_EQ(mytree.print().str(), "72398");

    results.clear();
    truths = std::vector<int>{2,3,7,8,9};
    for (auto it = mytree.begin(); it != mytree.end(); ++it){
        results.push_back((*it).data.first);
    }
    ASSERT_EQ(results, truths);

    // Проверка повторного удаления
    mytree.erase(4);

    results.clear();
    truths = std::vector<int>{2,3,7,8,9};
    for (auto it = mytree.begin(); it != mytree.end(); ++it){
        results.push_back((*it).data.first);
    }
    ASSERT_EQ(results, truths);

    mytree.clear();
    mytree.erase(2);
}

// Тестирование метода size
TEST(TestTask, test_size){
    AVL<int, std::string, std::less<int>, DumbAllocator<std::pair<const int, int>>> mytree{{1, "amogus1"}, {2, "amogus2"}, {3, "amogus3"}, {4, "amogus4"}, {5, "amogus5"}, {6, "amogus6"}};

    ASSERT_EQ(mytree.size(), 6);

    mytree.clear();
    ASSERT_EQ(mytree.size(), 0);

    mytree.insert(std::pair<int, std::string>(2, ""));
    ASSERT_EQ(mytree.size(), 1);
}

// Тестирование метода empty
TEST(TestTask, test_empty){
    AVL<int, std::string, std::less<int>, DumbAllocator<std::pair<const int, int>>> mytree{{1, "amogus1"}, {2, "amogus2"}, {3, "amogus3"}, {4, "amogus4"}, {5, "amogus5"}, {6, "amogus6"}};

    mytree.clear();
    ASSERT_EQ(mytree.empty(), true);

    mytree.insert(std::pair<int, std::string>(2, ""));
    ASSERT_EQ(mytree.empty(), false);
}

// Тестирование методов find и contains
TEST(TestTask, test_find_and_contains){
    AVL<int, std::string, std::less<int>, DumbAllocator<std::pair<const int, int>>> mytree{{1, "amogus1"}, {2, "amogus2"}, {3, "amogus3"}, {4, "amogus4"}, {5, "amogus5"}, {6, "amogus6"}};

    ASSERT_EQ((*mytree.find(1)).data.first, 1);
    ASSERT_EQ((*mytree.find(2)).data.first, 2);
    ASSERT_EQ((*mytree.find(3)).data.first, 3);
    ASSERT_EQ((*mytree.find(4)).data.first, 4);
    ASSERT_EQ((*mytree.find(5)).data.first, 5);
    ASSERT_EQ((*mytree.find(6)).data.first, 6);

    // Проверка возврата end итератора, если элемент не найден
    ASSERT_EQ(mytree.find(7), mytree.end());
    ASSERT_EQ(mytree.find(0), mytree.end());

    // Проверка метода contains
    ASSERT_EQ(mytree.contains(1), true);
    ASSERT_EQ(mytree.contains(7), false);
    ASSERT_EQ(mytree.contains(0), false);

    mytree.clear();
    ASSERT_EQ(mytree.contains(1), false);
    ASSERT_EQ(mytree.find(1), mytree.end());
}

// Тестирование методов at и оператора []
TEST(TestTask, test_at_and_brackets){
    AVL<int, std::string, std::less<int>, DumbAllocator<std::pair<const int, int>>> mytree{{1, "amogus1"}, {2, "amogus2"}, {3, "amogus3"}, {4, "amogus4"}, {5, "amogus5"}, {6, "amogus6"}};

    ASSERT_EQ(mytree.at(1), "amogus1");
    ASSERT_EQ(mytree.at(2), "amogus2");
    ASSERT_EQ(mytree.at(3), "amogus3");
    ASSERT_EQ(mytree.at(4), "amogus4");
    ASSERT_EQ(mytree.at(5), "amogus5");
    ASSERT_EQ(mytree.at(6), "amogus6");
    ASSERT_THROW(mytree.at(7), std::out_of_range);
    ASSERT_THROW(mytree.at(0), std::out_of_range);
    ASSERT_THROW(mytree.at(-7187), std::out_of_range);

    // Тестирование присваивания нового значения
    ASSERT_EQ(mytree[2], "amogus2");
    mytree[2] = "booba";
    ASSERT_EQ(mytree[2], "booba");

    // Тестирование инициализации нового элемента, если ключ не найден
    ASSERT_EQ(mytree[7], "");
    ASSERT_EQ(mytree[0], "");
    mytree[7] = "amogus7";
    ASSERT_EQ(mytree[7], "amogus7");
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}