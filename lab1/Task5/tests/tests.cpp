#include <gtest/gtest.h>
#include "my_container.h"

namespace {
    class VectorTest : public ::testing::Test {
    protected:
        void SetUp() override {
            emptyVec = my_container::Vector<int>();
            intVec = my_container::Vector<int>{1, 2, 3, 4, 5};
            strVec = my_container::Vector<std::string>{"hello", "world"};
        }

        my_container::Vector<int> emptyVec;
        my_container::Vector<int> intVec;
        my_container::Vector<std::string> strVec;
    };

    TEST_F(VectorTest, DefaultConstructor) {
        EXPECT_TRUE(emptyVec.empty());
        EXPECT_EQ(emptyVec.size(), 0);
    }

    TEST_F(VectorTest, InitializerListConstructor) {
        EXPECT_EQ(intVec.size(), 5);
        EXPECT_EQ(intVec[0], 1);
        EXPECT_EQ(intVec[4], 5);
    }

    TEST_F(VectorTest, CopyConstructor) {
        my_container::Vector<int> copy(intVec);
        EXPECT_EQ(copy.size(), intVec.size());
        EXPECT_EQ(copy[0], intVec[0]);
        EXPECT_EQ(copy[4], intVec[4]);
    }

    TEST_F(VectorTest, MoveConstructor) {
        my_container::Vector<int> moved(std::move(intVec));
        EXPECT_EQ(moved.size(), 5);
        EXPECT_TRUE(intVec.empty());
    }

    TEST_F(VectorTest, CopyAssignment) {
        my_container::Vector<int> copy;
        copy = intVec;
        EXPECT_EQ(copy.size(), intVec.size());
        EXPECT_EQ(copy[0], intVec[0]);
    }

    TEST_F(VectorTest, MoveAssignment) {
        my_container::Vector<int> moved;
        moved = std::move(intVec);
        EXPECT_EQ(moved.size(), 5);
        EXPECT_TRUE(intVec.empty());
    }

    TEST_F(VectorTest, SubscriptOperator) {
        EXPECT_EQ(intVec[0], 1);
        EXPECT_EQ(intVec[4], 5);
        EXPECT_THROW(intVec[5], std::out_of_range);
    }

    TEST_F(VectorTest, AtMethod) {
        EXPECT_EQ(intVec.at(0), 1);
        EXPECT_THROW(intVec.at(5), std::out_of_range);
    }

    TEST_F(VectorTest, FrontBack) {
        EXPECT_EQ(intVec.front(), 1);
        EXPECT_EQ(intVec.back(), 5);
        EXPECT_THROW(emptyVec.front(), std::out_of_range);
        EXPECT_THROW(emptyVec.back(), std::out_of_range);
    }

    TEST_F(VectorTest, Data) {
        const int* ptr = intVec.data();
        EXPECT_EQ(ptr[0], 1);
        EXPECT_EQ(ptr[4], 5);
    }

    TEST_F(VectorTest, PushBack) {
        intVec.push_back(6);
        EXPECT_EQ(intVec.size(), 6);
        EXPECT_EQ(intVec[5], 6);
    }

    TEST_F(VectorTest, PopBack) {
        intVec.pop_back();
        EXPECT_EQ(intVec.size(), 4);
        EXPECT_EQ(intVec.back(), 4);
        EXPECT_THROW(emptyVec.pop_back(), std::out_of_range);
    }

    TEST_F(VectorTest, Insert) {
        intVec.insert(2, 10);
        EXPECT_EQ(intVec.size(), 6);
        EXPECT_EQ(intVec[2], 10);
        EXPECT_EQ(intVec[3], 3);
        EXPECT_THROW(intVec.insert(10, 0), std::out_of_range);
    }

    TEST_F(VectorTest, Erase) {
        intVec.erase(2);
        EXPECT_EQ(intVec.size(), 4);
        EXPECT_EQ(intVec[2], 4);
        EXPECT_THROW(intVec.erase(10), std::out_of_range);
    }

    TEST_F(VectorTest, Clear) {
        intVec.clear();
        EXPECT_TRUE(intVec.empty());
        EXPECT_EQ(intVec.size(), 0);
    }

    TEST_F(VectorTest, Reserve) {
        intVec.reserve(20);
        EXPECT_GE(intVec.capacity(), 20);
        EXPECT_EQ(intVec.size(), 5);
    }

    TEST_F(VectorTest, ShrinkToFit) {
        intVec.reserve(20);
        intVec.shrink_to_fit();
        EXPECT_EQ(intVec.capacity(), intVec.size());
    }

    TEST_F(VectorTest, Equality) {
        my_container::Vector<int> same{1, 2, 3, 4, 5};
        my_container::Vector<int> diff{1, 2, 3};
        
        EXPECT_TRUE(intVec == same);
        EXPECT_FALSE(intVec == diff);
        EXPECT_TRUE(intVec != diff);
    }

    TEST_F(VectorTest, RelationalOperators) {
        my_container::Vector<int> smaller{1, 2, 3};
        my_container::Vector<int> sameSize{5, 4, 3, 2, 1};
        
        EXPECT_TRUE(smaller < intVec);
        EXPECT_TRUE(intVec > smaller);
        EXPECT_TRUE(smaller <= intVec);
        EXPECT_FALSE(intVec >= sameSize);
    }

    TEST_F(VectorTest, StringVector) {
        strVec.push_back("test");
        EXPECT_EQ(strVec.size(), 3);
        EXPECT_EQ(strVec.back(), "test");
    }

    TEST_F(VectorTest, Capacity) {
        EXPECT_GE(intVec.capacity(), intVec.size());
        emptyVec.reserve(10);
        EXPECT_GE(emptyVec.capacity(), 10);
    }
} 