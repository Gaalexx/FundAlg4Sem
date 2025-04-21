#include <gtest/gtest.h>
#include "my_container.h"

namespace {
    using namespace my_container;

    TEST(DequeTest, DefaultConstructor) {
        Deque<int> dq;
        EXPECT_TRUE(dq.empty());
        EXPECT_EQ(dq.size(), 0);
    }

    TEST(DequeTest, InitializerListConstructor) {
        Deque<int> dq = {1, 2, 3, 4, 5};
        EXPECT_EQ(dq.size(), 5);
        EXPECT_EQ(dq.front(), 1);
        EXPECT_EQ(dq.back(), 5);
    }

    TEST(DequeTest, CopyConstructor) {
        Deque<int> dq1 = {10, 20, 30};
        Deque<int> dq2(dq1);
        
        EXPECT_EQ(dq1.size(), dq2.size());
        EXPECT_EQ(dq1.front(), dq2.front());
        EXPECT_EQ(dq1.back(), dq2.back());
    }

    TEST(DequeTest, PushBackPopBack) {
        Deque<int> dq;
        dq.push_back(100);
        dq.push_back(200);
        
        EXPECT_EQ(dq.size(), 2);
        EXPECT_EQ(dq.back(), 200);
        
        dq.pop_back();
        EXPECT_EQ(dq.size(), 1);
        EXPECT_EQ(dq.back(), 100);
    }

    TEST(DequeTest, PushFrontPopFront) {
        Deque<int> dq;
        dq.push_front(100);
        dq.push_front(200);
        
        EXPECT_EQ(dq.size(), 2);
        EXPECT_EQ(dq.front(), 200);
        
        dq.pop_front();
        EXPECT_EQ(dq.size(), 1);
        EXPECT_EQ(dq.front(), 100);
    }

    TEST(DequeTest, EmptyAndClear) {
        Deque<int> dq = {1, 2, 3};
        EXPECT_FALSE(dq.empty());
        
        dq.clear();
        EXPECT_TRUE(dq.empty());
        EXPECT_EQ(dq.size(), 0);
    }

    TEST(DequeTest, Iterators) {
        Deque<int> dq = {1, 2, 3, 4, 5};
        
        int sum = 0;
        for (auto it = dq.begin(); it != dq.end(); ++it) {
            sum += *it;
        }
        EXPECT_EQ(sum, 15);
    }

    TEST(DequeTest, ConstIterators) {
        const Deque<const int> dq = {1, 2, 3, 4, 5};
        
        int sum = 0;
        for (auto it = dq.cbegin(); it != dq.cend(); ++it) {
            sum += *it;
        }
        EXPECT_EQ(sum, 15);
    }

    TEST(DequeTest, ReverseIterators) {
        Deque<int> dq = {1, 2, 3, 4, 5};
        
        std::vector<int> reversed;
        for (auto it = dq.rbegin(); it != dq.rend(); ++it) {
            reversed.push_back(*it);
        }
        
        std::vector<int> expected = {5, 4, 3, 2, 1};
        EXPECT_EQ(reversed, expected);
    }

    TEST(DequeTest, ComparisonOperators) {
        Deque<int> dq1 = {1, 2, 3};
        Deque<int> dq2 = {1, 2, 3};
        Deque<int> dq3 = {4, 5, 6};
        
        EXPECT_TRUE(dq1 == dq2);
        EXPECT_FALSE(dq1 == dq3);
        EXPECT_TRUE(dq1 < dq3);
        EXPECT_TRUE(dq3 > dq1);
    }

    TEST(DequeTest, Resize) {
        Deque<int> dq = {1, 2, 3, 4, 5};
        dq.resize(3);
        EXPECT_EQ(dq.size(), 3);
        EXPECT_EQ(dq.back(), 3);
        
        dq.resize(5, 100);
        EXPECT_EQ(dq.size(), 5);
        EXPECT_EQ(dq.back(), 100);
    }

    TEST(DequeTest, Swap) {
        Deque<int> dq1 = {1, 2, 3};
        Deque<int> dq2 = {4, 5, 6};
        
        List<int>::swap(dq1, dq2);
        
        EXPECT_EQ(dq1.front(), 4);
        EXPECT_EQ(dq2.front(), 1);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}