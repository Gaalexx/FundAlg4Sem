#include "unique_ptr.h"
#include <gtest/gtest.h>

namespace my_smart_ptr {

class UniquePtrTest : public ::testing::Test {
protected:
    void SetUp() override {
        int_ptr.reset(new int(42));
        array_ptr.reset(new int[3]{1, 2, 3});
    }

    UniquePtr<int> int_ptr;
    UniquePtr<int[]> array_ptr;
};

TEST_F(UniquePtrTest, DefaultConstructorCreatesNullPtr) {
    UniquePtr<int> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, ExplicitConstructorTakesOwnership) {
    int* raw = new int(10);
    UniquePtr<int> ptr(raw);
    EXPECT_EQ(ptr.get(), raw);
}

TEST_F(UniquePtrTest, MoveConstructorTransfersOwnership) {
    UniquePtr<int> moved(std::move(int_ptr));
    EXPECT_EQ(*moved, 42);
    EXPECT_EQ(int_ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, MoveAssignmentTransfersOwnership) {
    UniquePtr<int> moved;
    moved = std::move(int_ptr);
    EXPECT_EQ(*moved, 42);
    EXPECT_EQ(int_ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, DestructorReleasesMemory) {
    {
        UniquePtr<int> local(new int(100));
    } 
}

TEST_F(UniquePtrTest, GetReturnsRawPointer) {
    EXPECT_NE(int_ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, DereferenceOperatorWorks) {
    EXPECT_EQ(*int_ptr, 42);
}

TEST_F(UniquePtrTest, ArrowOperatorWorks) {
    struct Test { int x; };
    UniquePtr<Test> ptr(new Test{10});
    EXPECT_EQ(ptr->x, 10);
}

TEST_F(UniquePtrTest, BoolConversionWorks) {
    UniquePtr<int> empty;
    EXPECT_FALSE(empty);
    EXPECT_TRUE(int_ptr);
}

TEST_F(UniquePtrTest, ResetReleasesOldMemory) {
    int_ptr.reset(new int(99));
    EXPECT_EQ(*int_ptr, 99);
}

TEST_F(UniquePtrTest, ResetWithNullptrWorks) {
    int_ptr.reset();
    EXPECT_EQ(int_ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, ReleaseReturnsRawPointer) {
    int* raw = int_ptr.release();
    EXPECT_EQ(*raw, 42);
    EXPECT_EQ(int_ptr.get(), nullptr);
    delete raw;
}

TEST_F(UniquePtrTest, SwapExchangesPointers) {
    UniquePtr<int> other(new int(100));
    int_ptr.swap(other);
    EXPECT_EQ(*int_ptr, 100);
    EXPECT_EQ(*other, 42);
}


TEST_F(UniquePtrTest, ArrayDefaultConstructorCreatesNullPtr) {
    UniquePtr<int[]> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, ArrayExplicitConstructorTakesOwnership) {
    int* raw = new int[3];
    UniquePtr<int[]> ptr(raw);
    EXPECT_EQ(ptr.get(), raw);
}

TEST_F(UniquePtrTest, ArrayMoveConstructorTransfersOwnership) {
    UniquePtr<int[]> moved(std::move(array_ptr));
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(array_ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, ArrayMoveAssignmentTransfersOwnership) {
    UniquePtr<int[]> moved;
    moved = std::move(array_ptr);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(array_ptr.get(), nullptr);
}

TEST_F(UniquePtrTest, ArrayDestructorReleasesMemory) {
    {
        UniquePtr<int[]> local(new int[3]);
    } 
}

TEST_F(UniquePtrTest, ArraySubscriptOperatorWorks) {
    EXPECT_EQ(array_ptr[0], 1);
    EXPECT_EQ(array_ptr[1], 2);
    EXPECT_EQ(array_ptr[2], 3);
}

TEST_F(UniquePtrTest, ArrayResetReleasesOldMemory) {
    array_ptr.reset(new int[2]{4, 5});
    EXPECT_EQ(array_ptr[0], 4);
    EXPECT_EQ(array_ptr[1], 5);
}

TEST_F(UniquePtrTest, ArrayReleaseReturnsRawPointer) {
    int* raw = array_ptr.release();
    EXPECT_EQ(raw[0], 1);
    EXPECT_EQ(array_ptr.get(), nullptr);
    delete[] raw;
}

TEST_F(UniquePtrTest, SelfMoveAssignmentIsSafe) {
    int_ptr = std::move(int_ptr);
    EXPECT_EQ(*int_ptr, 42); // Should still be valid
}

} 

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}