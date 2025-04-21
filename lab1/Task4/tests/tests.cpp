#include <gtest/gtest.h>
#include "my_container.h"  // Замените на путь к вашему заголовочному файлу

namespace {
    using namespace my_container;
    
    TEST(StackTest, DefaultConstructor) {
        Stack<int> st;
        EXPECT_TRUE(st.empty());
        EXPECT_EQ(st.size(), 0);
    }

    TEST(StackTest, InitializerListConstructor) {
        Stack<int> st = {1, 2, 3, 4, 5};
        EXPECT_EQ(st.size(), 5);
        EXPECT_EQ(st.top(), 5);
    }

    TEST(StackTest, CopyConstructor) {
        Stack<int> st1 = {10, 20, 30};
        Stack<int> st2(st1);
        
        EXPECT_EQ(st1.size(), st2.size());
        EXPECT_EQ(st1.top(), st2.top());
    }

    TEST(StackTest, PushAndTop) {
        Stack<int> st;
        st.push(100);
        st.push(200);
        
        EXPECT_EQ(st.size(), 2);
        EXPECT_EQ(st.top(), 200);
    }

    TEST(StackTest, Pop) {
        Stack<int> st = {1, 2, 3};
        st.pop();
        
        EXPECT_EQ(st.size(), 2);
        EXPECT_EQ(st.top(), 2);
    }

    TEST(StackTest, EmptyAndClear) {
        Stack<int> st = {1, 2, 3};
        EXPECT_FALSE(st.empty());
        
        while (!st.empty()) {
            st.pop();
        }
        EXPECT_TRUE(st.empty());
        EXPECT_EQ(st.size(), 0);
    }

    TEST(StackTest, AssignmentOperator) {
        Stack<int> st1 = {1, 2, 3};
        Stack<int> st2;
        st2 = st1;
        
        EXPECT_EQ(st1.size(), st2.size());
        EXPECT_EQ(st1.top(), st2.top());
    }

    TEST(StackTest, ComparisonOperators) {
        Stack<int> st1 = {1, 2, 3};
        Stack<int> st2 = {1, 2, 3};
        Stack<int> st3 = {4, 5, 6};
        
        EXPECT_TRUE(st1 == st2);
        EXPECT_FALSE(st1 == st3);
        EXPECT_TRUE(st1 < st3);
        EXPECT_TRUE(st3 > st1);
        EXPECT_TRUE(st1 <= st2);
        EXPECT_TRUE(st3 >= st1);
    }

    TEST(StackTest, Swap) {
        Stack<int> st1 = {1, 2, 3};
        Stack<int> st2 = {4, 5, 6};
        
        Stack<int>::swap(st1, st2);
        
        EXPECT_EQ(st1.top(), 6);
        EXPECT_EQ(st2.top(), 3);
    }

    TEST(StackTest, PopEmptyStackThrows) {
        Stack<int> st;
        EXPECT_THROW(st.pop(), std::out_of_range);
    }

    TEST(StackTest, TopEmptyStackThrows) {
        Stack<int> st;
        EXPECT_THROW(st.top(), std::out_of_range);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}