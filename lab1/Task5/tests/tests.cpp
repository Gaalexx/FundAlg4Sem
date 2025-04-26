#include <gtest/gtest.h>
#include "my_container.h" 

namespace {

using namespace my_container;

class VectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        
    }

    void TearDown() override {
        
    }
};

TEST_F(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_GE(v.capacity(), 8);
    EXPECT_TRUE(v.empty());
}

TEST_F(VectorTest, SizeConstructor) {
    const size_t size = 10;
    Vector<int> v(size);
    EXPECT_EQ(v.size(), size);
    EXPECT_EQ(v.capacity(), size);
    EXPECT_FALSE(v.empty());
    
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(v[i], 0);
    }
}

TEST_F(VectorTest, SizeValueConstructor) {
    const size_t size = 5;
    const int value = 42;
    Vector<int> v(size, value);
    EXPECT_EQ(v.size(), size);
    
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(v[i], value);
    }
}

TEST_F(VectorTest, InitializerListConstructor) {
    Vector<int> v = {1, 2, 3, 4, 5};
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[4], 5);
}

TEST_F(VectorTest, CopyConstructor) {
    Vector<int> original = {1, 2, 3};
    Vector<int> copy(original);
    
    EXPECT_EQ(original.size(), copy.size());
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original[i], copy[i]);
    }
}

TEST_F(VectorTest, MoveConstructor) {
    Vector<int> original = {1, 2, 3};
    Vector<int> moved(std::move(original));
    
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(original.capacity(), 0);
    EXPECT_EQ(original.data(), nullptr);
}

TEST_F(VectorTest, CopyAssignment) {
    Vector<int> original = {1, 2, 3};
    Vector<int> copy;
    copy = original;
    
    EXPECT_EQ(original.size(), copy.size());
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original[i], copy[i]);
    }
}

TEST_F(VectorTest, MoveAssignment) {
    Vector<int> original = {1, 2, 3};
    Vector<int> moved;
    moved = std::move(original);
    
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(original.capacity(), 0);
    EXPECT_EQ(original.data(), nullptr);
}

TEST_F(VectorTest, PushPopBack) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.back(), 3);
    
    v.pop_back();
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), 2);
    
    EXPECT_THROW(Vector<int>().pop_back(), std::out_of_range);
}

TEST_F(VectorTest, Resize) {
    Vector<int> v = {1, 2, 3};
    v.resize(5);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 5);
    
    v.resize(2);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.capacity(), 2);
    
}

TEST_F(VectorTest, Clear) {
    Vector<int> v = {1, 2, 3};
    v.clear();
    EXPECT_EQ(v.size(), 0);
    EXPECT_GT(v.capacity(), 0);
}

TEST_F(VectorTest, InsertErase) {
    Vector<int> v = {1, 3};
    v.insert(1, 2);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[1], 2);
    
    v.erase(0);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 2);
    
    EXPECT_THROW(v.insert(10, 42), std::out_of_range);
    EXPECT_THROW(v.erase(10), std::out_of_range);
}

TEST_F(VectorTest, ElementAccess) {
    Vector<int> v = {1, 2, 3};
    
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
    
    const Vector<int>& cv = v;
    EXPECT_EQ(cv[1], 2);
    EXPECT_EQ(cv.at(1), 2);
    EXPECT_EQ(cv.front(), 1);
    EXPECT_EQ(cv.back(), 3);
    
    EXPECT_THROW(v.at(10), std::out_of_range);
    EXPECT_THROW(cv.at(10), std::out_of_range);
    EXPECT_THROW(Vector<int>().front(), std::out_of_range);
    EXPECT_THROW(Vector<int>().back(), std::out_of_range);
}

TEST_F(VectorTest, Data) {
    Vector<int> v = {1, 2, 3};
    const int* data = v.data();
    EXPECT_EQ(data[1], 2);
    
    const Vector<int>& cv = v;
    const int* cdata = cv.data();
    EXPECT_EQ(cdata[1], 2);
}

TEST_F(VectorTest, ReserveShrink) {
    Vector<int> v = {1, 2, 3};
    v.reserve(10);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 10);
    
    v.shrink_to_fit();
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 3);
}

TEST_F(VectorTest, ComparisonOperators) {
    Vector<int> v1 = {1, 2, 3};
    Vector<int> v2 = {1, 2, 3};
    Vector<int> v3 = {1, 2};
    Vector<int> v4 = {1, 2, 4};
    
    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 != v2);
    
    EXPECT_TRUE(v3 < v1);
    EXPECT_TRUE(v1 > v3);
    EXPECT_TRUE(v1 <= v2);
    EXPECT_TRUE(v1 >= v2);
    EXPECT_TRUE(v1 <= v4);
    EXPECT_TRUE(v4 >= v1);
}

TEST_F(VectorTest, MaxSizeAndEmpty) {
    Vector<int> v;
    EXPECT_GT(v.max_size(), 0);
    EXPECT_TRUE(v.empty());
    
    v.push_back(1);
    EXPECT_FALSE(v.empty());
}

TEST_F(VectorTest, Swap) {
    Vector<int> v1 = {1, 2, 3};
    Vector<int> v2 = {4, 5};
    
    Vector<int>::swap(v1, v2);
    
    EXPECT_EQ(v1.size(), 2);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1[0], 4);
    EXPECT_EQ(v2[0], 1);
}

TEST_F(VectorTest, ContainerInterface) {
    Vector<int> v1 = {1, 2, 3};
    Vector<int> v2 = {1, 2, 3};
    Vector<int> v3 = {1, 2};
    
    Container<int>* c1 = &v1;
    Container<int>* c2 = &v2;
    Container<int>* c3 = &v3;
    
    EXPECT_TRUE(*c1 == *c2);
    EXPECT_FALSE(*c1 == *c3);
    EXPECT_TRUE(*c1 != *c3);
    EXPECT_EQ(c1->size(), 3);
    EXPECT_GT(c1->max_size(), 0);
    EXPECT_FALSE(c1->empty());
}

TEST_F(VectorTest, ConstCorrectness) {
    const Vector<int> v = {1, 2, 3};
    
    EXPECT_EQ(v.size(), 3);
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v.at(1), 2);
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
    EXPECT_NE(v.data(), nullptr);
}

} 

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}