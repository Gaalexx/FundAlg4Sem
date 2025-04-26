#include <initializer_list>
#include <stdexcept>
#include <algorithm>

namespace my_container{
    template<class T, size_t N = 0>
    class Container{
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

    template<class T>
    class Vector : public Container<T> {

        using NoConstT = std::remove_const_t<T>;

        private:
            NoConstT* _data;
            size_t _length;
            size_t _capacity;

            void resize(){
                _capacity *= 2;
                NoConstT* new_data = new T[_capacity];

                for (size_t i = 0; i < _length; i++)
                {
                    new_data[i] = _data[i];
                }
                
                delete[] _data;

                _data = new_data;
            }


        public:

            Vector() : _length(0), _capacity(8)  {
                _data = new T[_capacity];
            }

            Vector(size_t size) : Container<T>(), _length(size), _capacity(size) {
                
                _data = new T[size];

                for (size_t i = 0; i < size; i++)
                {
                    _data[i] = 0;
                }
                
            }

            Vector(size_t size, T value) : Container<T>(),_length(size), _capacity(size) {
                
                _data = new T[size];

                for (size_t i = 0; i < size; i++)
                {
                    _data[i] = value;
                }
                
            }

            Vector(std::initializer_list<T> initList) : _length(initList.size()), _capacity(initList.size()) {
                _data = new T[_capacity];
                std::copy(initList.begin(), initList.end(), _data);
            }

            Vector(const Vector& other) : Container<T>(), _length(other._length), _capacity(other._capacity) {
                _data = new T[_capacity];
                for (size_t i = 0; i < _length; ++i) {
                    _data[i] = other._data[i];
                }
            }

            Vector(Vector&& other) noexcept 
                : _data(other._data), _length(other._length), _capacity(other._capacity) {
                other._data = nullptr;
                other._length = 0;
                other._capacity = 0;
            }


            Vector& operator=(const Vector& other) {
                if (this != &other) {
                    Vector temp(other);
                    swap(*this, temp);
                }
                return *this;
            }

            Vector& operator=(Vector&& other) noexcept {
                if (this != &other) {
                    delete[] _data;
                    _data = other._data;
                    _length = other._length;
                    _capacity = other._capacity;
                    other._data = nullptr;
                    other._length = 0;
                    other._capacity = 0;
                }
                return *this;
            }

            size_t size() const {
                return _length;
            }
            size_t max_size() const {
                std::allocator<T> alloc;
                return std::allocator_traits<std::allocator<T>>::max_size(alloc);
            }

            size_t capacity(){
                return _capacity;
            }

            bool empty() const {
                return size() == 0;
            }

            void resize(size_t new_cap){
                _capacity = new_cap;
                NoConstT* new_data = new T[_capacity];

                if(_length > _capacity){
                    _length = _capacity;
                }

                for (size_t i = 0; i < _length; i++)
                {
                    new_data[i] = _data[i];
                }
                
                delete[] _data;

                _data = new_data;
            }

            void push_back(const T& element){
                if(_capacity == _length){
                    resize();
                }
                _data[_length++] = element;
            }

            void pop_back(){
                if(_length == 0){
                    throw std::out_of_range("No elements!");
                }
                --_length;
            }

            void clear(){
                _length = 0;
            }

            void insert(size_t pos, const T& value) {
                if (pos > _length) {
                    throw std::out_of_range("Insert position out of range");
                }
                
                if (_length == _capacity) {
                    resize(_capacity == 0 ? 1 : _capacity * 2);
                }
                
                for (size_t i = _length; i > pos; --i) {
                    _data[i] = _data[i - 1];
                }
                
                _data[pos] = value;
                ++_length;
            }

            void erase(size_t pos) {
                if (pos >= _length) {
                    throw std::out_of_range("Erase position out of range");
                }
                
                for (size_t i = pos; i < _length - 1; ++i) {
                    _data[i] = _data[i + 1];
                }
                
                --_length;
            }

            static void swap(Vector<T>& element1, Vector<T>& element2) {
                std::swap(element1._capacity, element2._capacity);
                std::swap(element1._length, element2._length);
                std::swap(element1._data, element2._data);
            }

            T& operator[] (const size_t& index){
                if(index >= _length){
                    throw std::out_of_range("Out of range!");
                }
                else{
                    return _data[index];
                }
            }


            const T& operator[] (const size_t& index) const {
                if(index >= _length){
                    throw std::out_of_range("Out of range!");
                }
                else{
                    return _data[index];
                }
            }

            T& at(const size_t& index) {
                return (*this)[index];
            }


            const T& at(const size_t& index) const {
                return (*this)[index];
            }

            const T* data() const {
                return _data;
            }

            T& front(){
                if(_length == 0){
                    throw std::out_of_range("No elements!");
                }
                else{
                    return _data[0];
                }
            }

            const T& front() const {
                if(_length == 0){
                    throw std::out_of_range("No elements!");
                }
                else{
                    return _data[0];
                }
            }

            T& back(){
                if(_length == 0){
                    throw std::out_of_range("No elements!");
                }
                else{
                    return _data[_length - 1];
                }
            }

            const T& back() const {
                if(_length == 0){
                    throw std::out_of_range("No elements!");
                }
                else{
                    return _data[_length - 1];
                }
            }

            void reserve(size_t new_cap){
                resize(new_cap);
            }

            void shrink_to_fit(){
                resize(_length);
            }

            

            auto operator<=>(const Vector& other) const {
                if(_length != other._length){
                    return _length <=> other._length;
                }
                for (size_t i = 0; i < _length; ++i) {
                    if (auto res = _data[i] <=> other._data[i]; res != std::strong_ordering::equal) {
                        return res;
                    }
                }
                return std::strong_ordering::equal;
            }


            bool operator==(const Container<T>& second) const override final {
                const Vector<T>* other = dynamic_cast<const Vector<T>*>(&second);
                if (!other){
                    return false;
                }
                
                if (_length != other->_length){
                    return false;
                }

                for (size_t i = 0; i < _length; ++i) {
                    if (_data[i] != other->_data[i]) {
                        return false;
                    }
                }
                return true;
            }
            
            bool operator!=(const Container<T>& second) const override final {
                return !(*this == second);
            }

            bool operator<(const Vector& other) const {
                if((*this <=> other) == std::strong_ordering::less){
                    return true;
                }
                return false;
            }
            
            bool operator<=(const Vector& other) const {
                return !(other < *this);
            }
            
            bool operator>(const Vector& other) const {
                return other < *this;
            }
            
            bool operator>=(const Vector& other) const {
                return !(*this < other);
            }

            virtual ~Vector() override {
                delete[] _data;
                _length = 0;
                _capacity = 0;
            }
    };
}