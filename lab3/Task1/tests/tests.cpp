#include "BST.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>

class MyMapTest : public ::testing::Test {
protected:
    MyMap<int, std::string> intStrMap;
    MyMap<std::string, int> strIntMap;

    void SetUp() override {
        intStrMap.insert(1, "one");
        intStrMap.insert(2, "two");
        intStrMap.insert(3, "three");

        strIntMap.insert("apple", 5);
        strIntMap.insert("banana", 6);
        strIntMap.insert("cherry", 6);
    }
};

TEST_F(MyMapTest, DefaultConstructorCreatesEmptyMap) {
    MyMap<int, int> emptyMap;
    EXPECT_EQ(emptyMap.size(), 0);
}

TEST_F(MyMapTest, InsertIncreasesSize) {
    size_t initialSize = intStrMap.size();
    intStrMap.insert(4, "four");
    EXPECT_EQ(intStrMap.size(), initialSize + 1);
}

TEST_F(MyMapTest, FindReturnsCorrectValue) {
    auto* value = intStrMap.find(2);
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, "two");
}

TEST_F(MyMapTest, FindReturnsNullptrForNonexistentKey) {
    auto* value = intStrMap.find(99);
    EXPECT_EQ(value, nullptr);
}

TEST_F(MyMapTest, ContainsReturnsTrueForExistingKey) {
    EXPECT_TRUE(intStrMap.contains(1));
}

TEST_F(MyMapTest, ContainsReturnsFalseForNonexistentKey) {
    EXPECT_FALSE(intStrMap.contains(99));
}

/* TEST_F(MyMapTest, RemoveDecreasesSize) {
    size_t initialSize = intStrMap.size();
    intStrMap.remove(2);
    EXPECT_EQ(intStrMap.size(), initialSize - 1);
} */

/* TEST_F(MyMapTest, RemoveMakesKeyUnfindable) {
    intStrMap.remove(2);
    EXPECT_FALSE(intStrMap.contains(2));
} */

/* TEST_F(MyMapTest, ClearMakesMapEmpty) {
    intStrMap.clear();
    EXPECT_EQ(intStrMap.size(), 0);
    EXPECT_FALSE(intStrMap.contains(1));
} */

TEST_F(MyMapTest, InorderTraversalVisitsNodesInOrder) {
    std::vector<int> keys;
    std::vector<std::string> values;
    
    intStrMap.inorder([&](const int& key, const std::string& value) {
        keys.push_back(key);
        values.push_back(value);
    });

    ASSERT_EQ(keys.size(), 3);
    EXPECT_EQ(keys[0], 1);
    EXPECT_EQ(keys[1], 2);
    EXPECT_EQ(keys[2], 3);
    
    ASSERT_EQ(values.size(), 3);
    EXPECT_EQ(values[0], "one");
    EXPECT_EQ(values[1], "two");
    EXPECT_EQ(values[2], "three");
}

TEST_F(MyMapTest, PreorderTraversalVisitsRootFirst) {
    std::vector<int> keys;
    
    intStrMap.preoder([&](const int& key, const std::string&) {
        keys.push_back(key);
    });

    EXPECT_EQ(keys[0], 1); 
}

TEST_F(MyMapTest, PostorderTraversalVisitsRootLast) {
    std::vector<int> keys;
    
    intStrMap.postorder([&](const int& key, const std::string&) {
        keys.push_back(key);
    });

    EXPECT_EQ(keys.back(), 1);
}

TEST_F(MyMapTest, IteratorTraversesAllElements) {
    std::vector<std::string> values;
    for (auto it = intStrMap.begin(); it != intStrMap.end(); ++it) {
        values.push_back(*it);
    }

    ASSERT_EQ(values.size(), 3);
    EXPECT_EQ(values[0], "one");
    EXPECT_EQ(values[1], "two");
    EXPECT_EQ(values[2], "three");
}

TEST_F(MyMapTest, StringKeyWorksCorrectly) {
    auto* value = strIntMap.find("banana");
    ASSERT_NE(value, nullptr);
    EXPECT_EQ(*value, 6);
}

TEST_F(MyMapTest, GreaterComparatorWorks) {
    MyMap<int, std::string, BST, GreaterComparator> reverseMap;
    reverseMap.insert(1, "one");
    reverseMap.insert(2, "two");
    reverseMap.insert(3, "three");

    std::vector<int> keys;
    reverseMap.inorder([&](const int& key, const std::string&) {
        keys.push_back(key);
    });

    ASSERT_EQ(keys.size(), 3);
    EXPECT_EQ(keys[0], 3);
    EXPECT_EQ(keys[1], 2);
    EXPECT_EQ(keys[2], 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}