#pragma once 

#include <cstddef>
#include <stdexcept> 


template <typename T> 
class Vector { 
public: 

    Vector();

    explicit Vector(std::size_t size); 

    // copy constructor
    Vector(const Vector<T>& other); 

    // copy assignment operator
    Vector<T>& operator=(const Vector<T>& other); 

    // move constructor
    Vector(Vector<T>&& other) noexcept; 

    // move assignment
    Vector<T>& operator=(Vector<T>&& other) noexcept; 

    ~Vector() noexcept; 

    // return size of the vector (number of element currently in vector)
    std::size_t size() const noexcept { return size_; }

    // return undelying capacity of the vector
    std::size_t capacity() const noexcept { return cap_; }

    // get item located at specific index in vector 
    T& operator[](std::size_t index);

    // push (add) an item to the back of the vector 
    void push_back(T item); 

    // pop (remove) the last item in the vector 
    void pop_back(); 

    // return first element in the vector
    T& front() noexcept { return data_[0]; }

    const T& front() const noexcept { return data_[0]; }

    // return last element in the vector 
    T& back() noexcept { return data_[size_ - 1]; }

    const T& back() const noexcept { return data_[size_ - 1]; }

    // return pointer to the beginning of the vector 
    T* data() noexcept { return data_; }

    const T* data() const noexcept { return data_; }

    // return if vector contains any elements or not
    bool empty() const noexcept { return size_ == 0; }

    // return pointer to beginning of the array
    T* begin() noexcept { return data_; }

    // return pointer to the end of the array
    T* end() noexcept { return data_ + size_; }

    const T* begin() const noexcept { return data_; }

    const T* end() const noexcept { return data_ + size_; }

    // get item at a specific index 
    T& at(std::size_t index); 

    const T& at(std::size_t index) const; 

    // compare two vectors to check equivalency of underlying values
    bool operator==(const Vector<T>& other) const;  

    // reserve the given size of memory underneath, only changes 
    // underlying capacity of the array 
    void reserve(std::size_t size); 

    // removes unused memory inside capacity such that capacity 
    // equals size after being called
    void shrink_to_fit(); 

    // clear contents of the array
    void clear(); 

    // resize the array to the given size, changes capacity and size 
    // of the array 
    void resize(std::size_t size); 

private: 

    // double capacity of vector
    void add_cap_(); 

    // number items in vec   
    std::size_t size_; 

    // # underlying capacity
    std::size_t cap_; 

    T* data_ = nullptr; 
};


template <typename T> 
Vector<T>::Vector() { 
    size_ = 0; 
    cap_ = 0; 
}

template <typename T> 
Vector<T>::Vector(std::size_t size) : size_(size), cap_(size) { 
    data_ = new T[size](); // parenthesis do value init so they safely default primitive types
}

template <typename T> 
Vector<T>::Vector(const Vector<T>& other) 
    : size_(other.size_), cap_(other.cap_) 
{ 
    if (cap_ > 0) { 

        data_ = new T[cap_]; 

        for (std::size_t i = 0; i < size_; ++i) { 
            data_[i] = other.data_[i];
        }

    } else data_ = nullptr; 
}

template <typename T> 
Vector<T>& Vector<T>::operator=(const Vector<T>& other) 
{
    if (this == &other) return *this; 

    cap_ = other.cap_; 
    size_ = other.size_;

    delete[] data_; // prevent mem leak by delete whatever in this current obj
   
    if (cap_ > 0) {
        data_ = new T[cap_]; 

        for (std::size_t i = 0; i < size_; ++i) { 
            data_[i] = other.data_[i];
        }
    } else data_ = nullptr; 

    return *this; 
}

template <typename T> 
Vector<T>::Vector(Vector<T>&& other) noexcept
    : size_(other.size_), cap_(other.cap_), data_(other.data_) 
{
    other.size_ = 0; 
    other.cap_ = 0; 
    other.data_ = nullptr; 
}

template <typename T> 
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {

    if (this == &other) return *this; 

    delete[] data_; // delete the calling instance data to prevent memory leaks

    cap_ = other.cap_; 
    size_ = other.size_; 
    data_ = other.data_; 

    other.size_ = 0; 
    other.cap_ = 0; 
    other.data_ = nullptr; 

    return *this;
}

template <typename T> 
T& Vector<T>::operator[](std::size_t index) { 
    return data_[index];
}

template <typename T> 
Vector<T>::~Vector() noexcept {
    delete[] data_; 
}

template <typename T> 
void Vector<T>::push_back(T item) { 
    if (size_ + 1 >= cap_) { 
        add_cap_();
    }

    data_[size_] = item; 
    ++size_; 
}

template <typename T> 
void Vector<T>::pop_back() { 
    if (size_ == 0) { 
        throw std::logic_error("cannot pop_back from empty vector");
    }
    --size_;
}

template <typename T>
void Vector<T>::add_cap_() { 

    std::size_t new_cap = cap_ == 0 ? 1 : cap_ * 2; 

    T* new_data = new T[new_cap];

    for (std::size_t i = 0; i < size_; ++i) { 
        new_data[i] = data_[i];
    }

    delete[] data_;

    cap_ = new_cap;
    data_ = new_data;
}

template <typename T>
T& Vector<T>::at(std::size_t index) { 
    if (index < 0 || index >= size_) { 
        throw std::out_of_range("index out of bounds");
    }
    return data_[index];
}

template <typename T> 
const T& Vector<T>::at(std::size_t index) const { 
    if (index < 0 || index > size_) { 
        throw std::out_of_range("index out of bounds");
    }
    return data_[index];
}

template <typename T> 
bool Vector<T>::operator==(const Vector<T>& other) const { 
    if (other.size() != size_) return false; 

    for (std::size_t i = 0; i < size_; ++i) { 
        if (other[i] != data_[i]) return false; 
    }
    return true; 
}

template <typename T> 
void Vector<T>::reserve(std::size_t size) {  
    if (cap_ <= size) return; 

    T* new_data = new T[size]; 

    for (std::size_t i = 0; i < size_; ++i) { 
        new_data[i] = data_[i];
    }

    delete[] data_; 
    data_ = new_data; 
    cap_ = size; 
}

template <typename T> 
void Vector<T>::shrink_to_fit() { 
    if (size_ == cap_) return; 

    T* new_data = T[size_];

    for (std::size_t i = 0; i < size_; ++i) { 
        new_data[i] = data_[i];
    }

    delete[] data_; 
    data_ = new_data; 
    cap_ = size_; 
}

template <typename T> 
void Vector<T>::clear() { 
    size_ = 0; 
}

template <typename T> 
void Vector<T>::resize(std::size_t size) { 
    if (cap_ <= size) return; 

    T* new_data = new T[size]; 

    for (std::size_t i = 0; i < size_; ++i) { 
        new_data[i] = data_[i];
    }

    delete[] data_; 
    data_ = new_data; 
    cap_ = size; 
    size_ = size; 
}