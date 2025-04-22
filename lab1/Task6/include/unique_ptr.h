#include <utility>

namespace my_smart_ptr {

template <typename T>
    class UniquePtr {
        T* ptr_;

        public:
            UniquePtr() noexcept : ptr_(nullptr) {}

            explicit UniquePtr(T* ptr) noexcept : ptr_(ptr) {}

            UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
                other.ptr_ = nullptr;
            }

            ~UniquePtr() {
                delete ptr_;
            }

            UniquePtr(const UniquePtr&) = delete;
            UniquePtr& operator=(const UniquePtr&) = delete;

            UniquePtr& operator=(UniquePtr&& other) noexcept {
                if (this != &other) {
                    reset();
                    ptr_ = other.ptr_;
                    other.ptr_ = nullptr;
                }
                return *this;
            }

            T* get() const noexcept {
                return ptr_;
            }

            T& operator*() const {
                return *ptr_;
            }

            T* operator->() const noexcept {
                return ptr_;
            }

            explicit operator bool() const noexcept {
                return ptr_ != nullptr;
            }

            void reset(T* ptr = nullptr) noexcept {
                T* old = ptr_;
                ptr_ = ptr;
                delete old;
            }

            T* release() noexcept {
                T* result = ptr_;
                ptr_ = nullptr;
                return result;
            }

            void swap(UniquePtr& other) noexcept {
                std::swap(ptr_, other.ptr_);
            }
        };


        template <typename T>
    class UniquePtr<T[]> {
        T* ptr_;

        public:
            UniquePtr() noexcept : ptr_(nullptr) {}

            explicit UniquePtr(T* ptr) noexcept : ptr_(ptr) {}

            UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
                other.ptr_ = nullptr;
            }

            ~UniquePtr() {
                delete[] ptr_;
            }

            UniquePtr(const UniquePtr&) = delete;
            UniquePtr& operator=(const UniquePtr&) = delete;

            UniquePtr& operator=(UniquePtr&& other) noexcept {
                if (this != &other) {
                    reset();
                    ptr_ = other.ptr_;
                    other.ptr_ = nullptr;
                }
                return *this;
            }

            T* get() const noexcept {
                return ptr_;
            }

            T& operator*() const {
                return *ptr_;
            }

            T* operator->() const noexcept {
                return ptr_;
            }

            explicit operator bool() const noexcept {
                return ptr_ != nullptr;
            }

            T& operator[](std::size_t index) const {
                return ptr_[index];
            }

            void reset(T* ptr = nullptr) noexcept {
                T* old = ptr_;
                ptr_ = ptr;
                delete[] old;
            }

            T* release() noexcept {
                T* result = ptr_;
                ptr_ = nullptr;
                return result;
            }

            void swap(UniquePtr& other) noexcept {
                std::swap(ptr_, other.ptr_);
            }
    };
}