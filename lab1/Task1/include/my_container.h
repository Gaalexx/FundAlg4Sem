#include <initializer_list>
#include <stdexcept>
#include <algorithm>

namespace my_container{
    template<class T, size_t N>
    class Container{
        protected:
            
            virtual T* _begin() = 0;
            virtual T* _rbegin() = 0;
            virtual T* _end() = 0;
            virtual T* _rend() = 0;
        public:  
            Container() {}
            Container(const Container<T, N> &source [[maybe_unused]]){}

            virtual ~Container(){}
            virtual bool operator==(const Container<T, N> &second) const = 0;
            virtual bool operator!=(const Container<T, N> &second) const = 0;
            virtual size_t size() const = 0;
            virtual size_t max_size() const = 0;
            virtual bool empty() const = 0;
    };

    template<class T, size_t N>
    class Array : public Container<T, N>{
    private:
        T elements[N];
        size_t _size;
        T* _begin() noexcept override final{
            T* ptr = this->elements;
            return ptr;
        }
        
        T* _end() noexcept override final{
            T* ptr = this->elements + 1;
            if(this->_size > 0){
                ptr = this->elements + this->_size;
                return ptr;
            }
            return ptr;
        }

        T* _rend() noexcept override final {
            return _begin() - 1;
        }

        T* _rbegin() noexcept override final {
            return _end() - 1;
        }

        const T* _cbegin() const noexcept{
            const T* ptr = this->elements;
            return ptr;
        }
        
        const T* _cend() const noexcept{
            const T* ptr = this->elements + 1;
            if(this->_size > 0){
                ptr = this->elements + this->_size;
                return ptr;
            }
            return ptr;
        }

        const T* _crend() const noexcept {
            return this->elements - 1;
        }

        const T* _crbegin() const noexcept {
            return _size > 0 ? this->elements + this->_size - 1 : this->elements;
        }

    public:
        class Iterator {
            private:
                T* ptr;
            public:
                Iterator(T* ptr = nullptr) : ptr(ptr) {}
                Iterator(const Iterator &itr) : ptr(itr.ptr) {}
                T& operator*() const {
                    return *ptr;
                }
            
            
                Iterator& operator++() {
                    ++ptr;
                    return *this;
                }
            
                Iterator operator++(int) {
                    Iterator tmp = *this;
                    ++(*this);
                    return tmp;
                }
            
                Iterator& operator--() {
                    --ptr;
                    return *this;
                }
            
                Iterator operator--(int) {
                    Iterator tmp = *this;
                    --(*this);
                    return tmp;
                }
            
                Iterator operator+(int n) const {
                    return Iterator(ptr + n);
                }
            
                Iterator operator-(int n) const {
                    return Iterator(ptr - n);
                }
            
                Iterator& operator+=(int n) {
                    ptr += n;
                    return *this;
                }
                
                Iterator& operator-=(int n) {
                    ptr -= n;
                    return *this;
                }
            
                bool operator!=(const Iterator& other) const {
                    return ptr != other.ptr;
                }

                std::strong_ordering operator<=> (const Iterator& other){
                    if(other.ptr == ptr){
                        return std::strong_ordering::equal;
                    }
                    else if(other.ptr < ptr){
                        return std::strong_ordering::greater;
                    }
                    else{
                        return std::strong_ordering::less;
                    }
                }
            
                int operator-(const Iterator& other) const {
                    return ptr - other.ptr;
                }
        };

        class ConstIterator {
            private:
                const T* ptr;
            public:
                ConstIterator(const T* ptr = nullptr) : ptr(ptr) {}
                ConstIterator(const ConstIterator &itr) : ptr(itr.ptr) {}
                const T& operator*() const {
                    return *ptr;
                }
            
            
                ConstIterator& operator++() {
                    ++ptr;
                    return *this;
                }
            
                ConstIterator operator++(int) {
                    ConstIterator tmp = *this;
                    ++(*this);
                    return tmp;
                }
            
                ConstIterator& operator--() {
                    --ptr;
                    return *this;
                }
            
                ConstIterator operator--(int) {
                    ConstIterator tmp = *this;
                    --(*this);
                    return tmp;
                }
            
                ConstIterator operator+(int n) const {
                    return ConstIterator(ptr + n);
                }
            
                ConstIterator operator-(int n) const {
                    return ConstIterator(ptr - n);
                }
            
                ConstIterator& operator+=(int n) {
                    ptr += n;
                    return *this;
                }
                
                ConstIterator& operator-=(int n) {
                    ptr -= n;
                    return *this;
                }
            
                bool operator!=(const ConstIterator& other) const {
                    return ptr != other.ptr;
                }

                std::strong_ordering operator<=> (const ConstIterator& other){
                    if(other.ptr == ptr){
                        return std::strong_ordering::equal;
                    }
                    else if(other.ptr < ptr){
                        return std::strong_ordering::greater;
                    }
                    else{
                        return std::strong_ordering::less;
                    }
                }
            
                int operator-(const ConstIterator& other) const {
                    return ptr - other.ptr;
                }
        };

        class ReverseIterator{
            protected:
                T* ptr;
            public:
                ReverseIterator(T* ptr = nullptr) : ptr(ptr) {}
                ReverseIterator(const ReverseIterator &itr) : ptr(itr.ptr) {}
                T& operator*() const {
                    return *ptr;
                }
            
            
                ReverseIterator& operator++() {
                    --ptr;
                    return *this;
                }
            
                ReverseIterator operator++(int) {
                    ReverseIterator tmp = *this;
                    --(*this).ptr;
                    return tmp;
                }
            
                ReverseIterator& operator--() {
                    ++ptr;
                    return *this;
                }
            
                ReverseIterator operator--(int) {
                    ReverseIterator tmp = *this;
                    ++(*this).ptr;
                    return tmp;
                }
            
                ReverseIterator operator+(int n) const {
                    return ReverseIterator(ptr - n);
                }
            
                ReverseIterator operator-(int n) const {
                    return ReverseIterator(ptr + n);
                }
            
                ReverseIterator& operator+=(int n) {
                    ptr -= n;
                    return *this;
                }
                
                ReverseIterator& operator-=(int n) {
                    ptr += n;
                    return *this;
                }

                
            
                bool operator==(const ReverseIterator& other) const {
                    return ptr == other.ptr;
                }
            
                bool operator!=(const ReverseIterator& other) const {
                    return ptr != other.ptr;
                }
            
                std::strong_ordering operator<=> (const ReverseIterator& other){
                    if(other.ptr == ptr){
                        return std::strong_ordering::equal;
                    }
                    else if(other.ptr < ptr){
                        return std::strong_ordering::less;
                    }
                    else{
                        return std::strong_ordering::greater;
                    }
                }
            
                int operator-(const Iterator& other) const {
                    return other.ptr - ptr;
                }

        };

        class ConstReverseIterator{
            protected:
                const T* ptr;
            public:
                ConstReverseIterator(const T* ptr = nullptr) : ptr(ptr) {}
                ConstReverseIterator(const ConstReverseIterator &itr) : ptr(itr.ptr) {}
                const T& operator*() const {
                    return *ptr;
                }
            
            
                ConstReverseIterator& operator++() {
                    --ptr;
                    return *this;
                }
            
                ConstReverseIterator operator++(int) {
                    ConstReverseIterator tmp = *this;
                    --(*this).ptr;
                    return tmp;
                }
            
                ConstReverseIterator& operator--() {
                    ++ptr;
                    return *this;
                }
            
                ConstReverseIterator operator--(int) {
                    ConstReverseIterator tmp = *this;
                    ++(*this).ptr;
                    return tmp;
                }
            
                ConstReverseIterator operator+(int n) const {
                    return ReverseIterator(ptr - n);
                }
            
                ConstReverseIterator operator-(int n) const {
                    return ReverseIterator(ptr + n);
                }
            
                ConstReverseIterator& operator+=(int n) {
                    ptr -= n;
                    return *this;
                }
                
                ConstReverseIterator& operator-=(int n) {
                    ptr += n;
                    return *this;
                }

                
            
                bool operator==(const ConstReverseIterator& other) const {
                    return ptr == other.ptr;
                }
            
                bool operator!=(const ConstReverseIterator& other) const {
                    return ptr != other.ptr;
                }
            
                std::strong_ordering operator<=> (const ConstReverseIterator& other){
                    if(other.ptr == ptr){
                        return std::strong_ordering::equal;
                    }
                    else if(other.ptr < ptr){
                        return std::strong_ordering::less;
                    }
                    else{
                        return std::strong_ordering::greater;
                    }
                }
            
                int operator-(const ConstReverseIterator& other) const {
                    return other.ptr - ptr;
                }

        };

        Array() : Container<T, N>() {
            this->_size = N;
            for (size_t i = 0; i < N; i++)
            {
                elements[i] = 0;
            }
        }

        Array(const Array<T, N> &source) : Container<T, N>(source) {
            for (size_t i = 0; i < N; i++)
            {
                this->elements[i] = source.elements[i];
            }
            this->_size = source._size;
        }

        Array(const std::initializer_list<T> initList) : Container<T, N>() {
            this->_size = N;
            std::copy(initList.begin(), initList.end(), this->elements);
            for (size_t i = initList.size(); i < N; i++)
            {
                elements[i] = 0;
            }
            
        }
        Array(Array<T, N>&& other) noexcept : Container<T, N>(std::move(other)) {
            for (size_t i = 0; i < N; ++i) {
                elements[i] = std::move(other.elements[i]);
            }
            this->_size = other._size;
            other._size = 0;  
        }

        using iterator = Iterator;
        using const_iterator = ConstIterator;
        using reverse_iterator = ReverseIterator;
        using const_reverse_iterator = ConstReverseIterator;

        reverse_iterator rbegin() noexcept {
            return reverse_iterator(_rbegin());
        }

        const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(_crbegin());
        }

        reverse_iterator rend() noexcept {
            return reverse_iterator(_rend());
        }

        const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(_crend());
        }

        iterator begin() noexcept {
            return iterator(_begin());
        }

        iterator end() noexcept {
            return iterator(_end());
        }

        const_iterator cbegin() const noexcept {
            return const_iterator(_cbegin());
        }

        const_iterator cend() const noexcept {
            return const_iterator(_cend());
        }

        T& front() {
            return elements[0];
        }

        T& back() {
            return elements[_size - 1];
        }

        const T& front() const {
            return elements[0];
        }

        const T& back() const {
            return elements[_size - 1];
        }

        bool operator==(const Container<T, N> &second) const override final{
            const Array<T, N> *tmp = dynamic_cast<const Array<T, N>*>(&second);
            if(_size != tmp->_size){
                return false;
            }
            for (size_t i = 0; i < tmp->_size; i++)
            {
                if(tmp->elements[i] != elements[i]){
                    return false;
                }
            }
            return true;
        }
        bool operator!=(const Container<T, N> &second) const override final{
            const Array<T, N> *tmp = dynamic_cast<const Array<T, N>*>(&second);
            if(_size != tmp->_size){
                return true;
            }
            for (size_t i = 0; i < tmp->_size; i++)
            {
                if(tmp->elements[i] != elements[i]){
                    return true;
                }
            }
            return false;
        }

        std::strong_ordering operator<=>(const Array& array) const{
            for (size_t i = 0; i < N; i++)
            {
                if(this->elements[i] < array.elements[i]){
                    return std::strong_ordering::less;
                }
                else if(this->elements[i] > array.elements[i]){
                    return std::strong_ordering::greater;
                }
            }
            return std::strong_ordering::equal;
        }

        Array& operator=(const Array& array){
            for (size_t i = 0; i < N; i++)
            {
                this->elements[i] = array.elements[i];
            }
            return (*this);
        }

        size_t size() const override final{
            return this->_size;
        }

        size_t max_size() const override final{
            return N;
        }

        bool empty() const override{
            return this->_size == 0? true : false;
        }

        T& operator[](const size_t &index){
            if(index >= N || index >= this->_size){
                throw std::out_of_range("Out of range");
            }
            return this->elements[index];
        }

        const T& operator[](const size_t &index) const {
            if(index >= N || index >= this->_size){
                throw std::out_of_range("Out of range");
            }
            return this->elements[index];
        }

        bool operator<(const Array& other) const {
            return (*this <=> other) == std::strong_ordering::less;
        }
    
        bool operator>(const Array& other) const {
            return (*this <=> other) == std::strong_ordering::greater;
        }
    
        bool operator<=(const Array& other) const {
            return !(*this > other);
        }
    
        bool operator>=(const Array& other) const {
            return !(*this < other);
        }


        
        void fill(Iterator start, Iterator end, T value){
            if(end < start){
                throw std::exception();
            }
            for (; start <= end; start++)
            {
                *start = value;
            }
        }

        void fill(T value){
            for (size_t i = 0; i < N; i++)
            {
                elements[i] = value;
            }
        }

        const T* data() const noexcept{
            const T* ret = this->elements;
            return ret;
        }

        T& at(const size_t index){
            return (*this)[index];
        }

        const T& at(const size_t index) const {
            return (*this)[index];
        }

        static void swap(Array &a1, Array &a2){
            size_t maxSize = std::max(a1._size, a2._size);
            std::swap(a1._size, a2._size);
            for (size_t i = 0; i < maxSize; i++)
            {
                std::swap(a1.elements[i], a2.elements[i]);
            }
        }

        virtual ~Array() override = default;

    };
}
