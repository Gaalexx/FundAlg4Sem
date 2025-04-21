#include <gtest/gtest.h>
#include "unique_ptr.h"

namespace my_smart_ptr {
namespace testing {

    class UniquePtrTest : public ::testing::Test {
        protected:
            void SetUp() override {
                int_ptr.reset(new int(42));
                array_ptr.reset(new int[5]{1, 2, 3, 4, 5});
            }

            void TearDown() override {}

            my_smart_ptr::UniquePtr<int> int_ptr;
            my_smart_ptr::UniquePtr<int[]> array_ptr;
        };

        TEST_F(UniquePtrTest, DefaultConstructor) {
            my_smart_ptr::UniquePtr<int> ptr;
            EXPECT_EQ(ptr.get(), nullptr);
            EXPECT_FALSE(static_cast<bool>(ptr));
        }

        TEST_F(UniquePtrTest, PointerConstructor) {
            int* raw = new int(10);
            my_smart_ptr::UniquePtr<int> ptr(raw);
            EXPECT_EQ(ptr.get(), raw);
            EXPECT_EQ(*ptr, 10);
        }

        TEST_F(UniquePtrTest, MoveConstructor) {
            int* raw = int_ptr.get();
            my_smart_ptr::UniquePtr<int> ptr(std::move(int_ptr));
            
            EXPECT_EQ(ptr.get(), raw);
            EXPECT_EQ(*ptr, 42);
            EXPECT_EQ(int_ptr.get(), nullptr);
        }

        TEST_F(UniquePtrTest, MoveAssignment) {
            my_smart_ptr::UniquePtr<int> ptr;
            ptr = std::move(int_ptr);
            
            EXPECT_EQ(*ptr, 42);
            EXPECT_EQ(int_ptr.get(), nullptr);
        }

        TEST_F(UniquePtrTest, SelfMoveAssignment) {
            int* raw = int_ptr.get();
            int_ptr = std::move(int_ptr);
            
            EXPECT_EQ(int_ptr.get(), raw);
            EXPECT_EQ(*int_ptr, 42);
        }

        TEST_F(UniquePtrTest, GetMethod) {
            EXPECT_NE(int_ptr.get(), nullptr);
            EXPECT_EQ(*int_ptr.get(), 42);
        }

        TEST_F(UniquePtrTest, DereferenceOperator) {
            EXPECT_EQ(*int_ptr, 42);
            *int_ptr = 100;
            EXPECT_EQ(*int_ptr, 100);
        }

        TEST_F(UniquePtrTest, ArrowOperator) {
            struct TestStruct { int x; };
            my_smart_ptr::UniquePtr<TestStruct> ptr(new TestStruct{42});
            
            EXPECT_EQ(ptr->x, 42);
            ptr->x = 100;
            EXPECT_EQ(ptr->x, 100);
        }

        TEST_F(UniquePtrTest, BoolOperator) {
            my_smart_ptr::UniquePtr<int> empty_ptr;
            EXPECT_FALSE(static_cast<bool>(empty_ptr));
            EXPECT_TRUE(static_cast<bool>(int_ptr));
        }

        TEST_F(UniquePtrTest, ResetWithPointer) {
            int* new_raw = new int(100);
            int* old_raw = int_ptr.get();
            (void)old_raw;
            int_ptr.reset(new_raw);
            
            EXPECT_EQ(int_ptr.get(), new_raw);
            EXPECT_EQ(*int_ptr, 100);
        }

        TEST_F(UniquePtrTest, ResetWithoutPointer) {
            int_ptr.reset();
            EXPECT_EQ(int_ptr.get(), nullptr);
            EXPECT_FALSE(static_cast<bool>(int_ptr));
        }

        TEST_F(UniquePtrTest, Release) {
            int* raw = int_ptr.release();
            
            EXPECT_EQ(*raw, 42);
            EXPECT_EQ(int_ptr.get(), nullptr);
            delete raw;
        }

        TEST_F(UniquePtrTest, Swap) {
            my_smart_ptr::UniquePtr<int> other(new int(100));
            int* first_raw = int_ptr.get();
            int* second_raw = other.get();
            
            int_ptr.swap(other);
            
            EXPECT_EQ(int_ptr.get(), second_raw);
            EXPECT_EQ(*int_ptr, 100);
            EXPECT_EQ(other.get(), first_raw);
            EXPECT_EQ(*other, 42);
        }

        TEST_F(UniquePtrTest, ArrayDefaultConstructor) {
            my_smart_ptr::UniquePtr<int[]> ptr;
            EXPECT_EQ(ptr.get(), nullptr);
        }

        TEST_F(UniquePtrTest, ArrayPointerConstructor) {
            int* raw = new int[3]{1, 2, 3};
            my_smart_ptr::UniquePtr<int[]> ptr(raw);
            EXPECT_EQ(ptr.get(), raw);
        }

        TEST_F(UniquePtrTest, ArrayMoveConstructor) {
            int* raw = array_ptr.get();
            my_smart_ptr::UniquePtr<int[]> ptr(std::move(array_ptr));
            
            EXPECT_EQ(ptr.get(), raw);
            EXPECT_EQ(array_ptr.get(), nullptr);
        }

        TEST_F(UniquePtrTest, ArraySubscriptOperator) {
            EXPECT_EQ(array_ptr[0], 1);
            EXPECT_EQ(array_ptr[4], 5);
            array_ptr[2] = 100;
            EXPECT_EQ(array_ptr[2], 100);
        }

        TEST_F(UniquePtrTest, ArrayReset) {
            int* new_raw = new int[2]{10, 20};
            array_ptr.reset(new_raw);
            
            EXPECT_EQ(array_ptr.get(), new_raw);
            EXPECT_EQ(array_ptr[0], 10);
        }

        TEST_F(UniquePtrTest, ArrayRelease) {
            int* raw = array_ptr.release();
            EXPECT_EQ(raw[0], 1);
            EXPECT_EQ(array_ptr.get(), nullptr);
            delete[] raw;
        }

    } 
} 

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}