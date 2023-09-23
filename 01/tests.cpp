#include <iostream>
#include <gtest/gtest.h>

#include "task.cpp"

class TestTask : ::testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};


TEST(TestTask, test){
    
    Allocator al = Allocator();
    al.makeAllocator(5);
    
    char* p1 = al.alloc(2);
    char* p2 = al.alloc(2);
    ASSERT_EQ(p1 + 2, p2);

    al.reset();
    ASSERT_EQ(p1, al.alloc(3));
    
}


TEST(TestTask, test_bounds){
    
    Allocator al = Allocator();
    al.makeAllocator(0);
    ASSERT_EQ(nullptr, al.alloc(1));
    ASSERT_EQ(nullptr, al.alloc(0));

    al.makeAllocator(3);
    ASSERT_NE(nullptr, al.alloc(3));
    ASSERT_EQ(nullptr, al.alloc(4));
    
}


int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}