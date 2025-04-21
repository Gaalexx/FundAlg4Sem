#include <gtest/gtest.h>
#include "my_container.h" 

namespace {

using namespace my_container;

TEST(ArrayTest, DefaultConstructor) {
    Array<int, 5> arr;
    EXPECT_EQ(arr.size(), 5);
    EXPECT_EQ(arr.max_size(), 5);
    EXPECT_FALSE(arr.empty());
    
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 0);
    }
}

TEST(ArrayTest, InitializerListConstructor) {
    Array<int, 5> arr = {1, 2, 3};
    EXPECT_EQ(arr.size(), 5);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 0);
    EXPECT_EQ(arr[4], 0);
}

TEST(ArrayTest, CopyConstructor) {
    Array<int, 4> arr1 = {1, 2, 3, 4};
    Array<int, 4> arr2(arr1);
    
    EXPECT_EQ(arr1.size(), arr2.size());
    for (size_t i = 0; i < arr1.size(); ++i) {
        EXPECT_EQ(arr1[i], arr2[i]);
    }
}

TEST(ArrayTest, MoveConstructor) {
    Array<int, 3> arr1 = {1, 2, 3};
    Array<int, 3> arr2(std::move(arr1));
    
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
    EXPECT_EQ(arr2[2], 3);
    EXPECT_EQ(arr1.size(), 0);  
}

TEST(ArrayTest, AssignmentOperator) {
    Array<int, 4> arr1 = {1, 2, 3, 4};
    Array<int, 4> arr2;
    arr2 = arr1;
    
    for (size_t i = 0; i < arr1.size(); ++i) {
        EXPECT_EQ(arr1[i], arr2[i]);
    }
}

TEST(ArrayTest, ElementAccess) {
    Array<int, 3> arr = {10, 20, 30};
    
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
    EXPECT_EQ(arr[2], 30);
    
    arr[1] = 25;
    EXPECT_EQ(arr[1], 25);
    
    EXPECT_THROW(arr[3], std::out_of_range);
}

TEST(ArrayTest, AtMethod) {
    Array<int, 3> arr = {10, 20, 30};
    
    EXPECT_EQ(arr.at(0), 10);
    EXPECT_EQ(arr.at(1), 20);
    EXPECT_EQ(arr.at(2), 30);
    
    EXPECT_THROW(arr.at(3), std::out_of_range);
}

TEST(ArrayTest, FrontAndBack) {
    Array<int, 4> arr = {1, 2, 3, 4};
    
    EXPECT_EQ(arr.front(), 1);
    EXPECT_EQ(arr.back(), 4);
    
    arr.front() = 10;
    arr.back() = 40;
    
    EXPECT_EQ(arr.front(), 10);
    EXPECT_EQ(arr.back(), 40);
}

TEST(ArrayTest, Data) {
    Array<int, 3> arr = {1, 2, 3};
    const int* data = arr.data();
    
    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[1], 2);
    EXPECT_EQ(data[2], 3);
}

TEST(ArrayTest, Fill) {
    Array<int, 5> arr;
    arr.fill(42);
    
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr[i], 42);
    }
}

TEST(ArrayTest, RangeFill) {
    Array<int, 5> arr = {1, 2, 3, 4, 5};
    arr.fill(arr.begin() + 1, arr.begin() + 4, 0);
    
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 0);
    EXPECT_EQ(arr[2], 0);
    EXPECT_EQ(arr[3], 0);
    EXPECT_EQ(arr[4], 0);
}

TEST(ArrayTest, Iterators) {
    Array<int, 4> arr = {1, 2, 3, 4};
    
    int expected = 1;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    
    expected = 4;
    for (auto it = arr.rbegin(); it != arr.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
}

TEST(ArrayTest, ConstIterators) {
    const Array<const int, 4> arr = {1, 2, 3, 4};
    
    int expected = 1;
    for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
    
    expected = 4;
    for (auto it = arr.crbegin(); it != arr.crend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
}

TEST(ArrayTest, ComparisonOperators) {
    Array<int, 3> arr1 = {1, 2, 3};
    Array<int, 3> arr2 = {1, 2, 3};
    Array<int, 3> arr3 = {1, 2, 4};
    Array<int, 3> arr4 = {1, 1, 3};
    
    EXPECT_TRUE(arr1 == arr2);
    EXPECT_FALSE(arr1 != arr2);
    
    EXPECT_TRUE(arr1 < arr3);
    EXPECT_TRUE(arr4 < arr1);
    EXPECT_TRUE(arr1 <= arr2);
    EXPECT_TRUE(arr1 >= arr2);
    EXPECT_TRUE(arr3 > arr1);
}

TEST(ArrayTest, Swap) {
    Array<int, 3> arr1 = {1, 2, 3};
    Array<int, 3> arr2 = {4, 5, 6};
    
    Array<int, 3>::swap(arr1, arr2);
    
    EXPECT_EQ(arr1[0], 4);
    EXPECT_EQ(arr1[1], 5);
    EXPECT_EQ(arr1[2], 6);
    
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);
    EXPECT_EQ(arr2[2], 3);
}

TEST(ArrayTest, ThreeWayComparison) {
    Array<int, 3> arr1 = {1, 2, 3};
    Array<int, 3> arr2 = {1, 2, 3};
    Array<int, 3> arr3 = {1, 2, 4};
    Array<int, 3> arr4 = {1, 1, 5};
    
    EXPECT_EQ(arr1 <=> arr2, std::strong_ordering::equal);
    EXPECT_EQ(arr1 <=> arr3, std::strong_ordering::less);
    EXPECT_EQ(arr3 <=> arr1, std::strong_ordering::greater);
    EXPECT_EQ(arr1 <=> arr4, std::strong_ordering::greater);
}

TEST(ArrayTest, IteratorOperations) {
    Array<int, 5> arr = {1, 2, 3, 4, 5};
    
    auto it = arr.begin();
    EXPECT_EQ(*it, 1);
    
    ++it;
    EXPECT_EQ(*it, 2);
    
    it += 2;
    EXPECT_EQ(*it, 4);
    
    it--;
    EXPECT_EQ(*it, 3);
    
    auto it2 = it + 1;
    EXPECT_EQ(*it2, 4);
    
    EXPECT_EQ(it2 - it, 1);
}

TEST(ArrayTest, ReverseIteratorOperations) {
    Array<int, 5> arr = {1, 2, 3, 4, 5};
    
    auto rit = arr.rbegin();
    EXPECT_EQ(*rit, 5);
    
    ++rit;
    EXPECT_EQ(*rit, 4);
    
    rit += 2;
    EXPECT_EQ(*rit, 2);
    
    rit--;
    EXPECT_EQ(*rit, 3);
    
    auto rit2 = rit + 1;
    EXPECT_EQ(*rit2, 2);
    
}

}  

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}