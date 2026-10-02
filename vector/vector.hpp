#pragma once 

#include <cstddef>
#include <stdexcept> 
#include <memory> 
#include <concepts> 

template <typename T, typename Allocator = std::allocator<T>> 
class Vector { 
public: 

    Vector();

    explicit Vector(std::size_t size); 

    // copy constructor
    Vector(const Vector<T, Allocator>& other); 

    // copy assignment operator
    Vector<T, Allocator>& operator=(const Vector<T, Allocator>& other); 

    // move constructor
    Vector(Vector<T, Allocator>&& other) noexcept; 

    // move assignment
    Vector<T, Allocator>& operator=(Vector<T, Allocator>&& other) noexcept; 

    ~Vector() noexcept; 

    // return size of the vector (number of element currently in vector)
    std::size_t size() const noexcept { return size_; }

    // return undelying capacity of the vector
    std::size_t capacity() const noexcept { return cap_; }

    // get item located at specific index in vector 
    T& operator[](std::size_t index) noexcept { return data_[index]; };

    const T& operator[](std::size_t index) const noexcept { return data_[index]; }; 

    // push (add) an item to the back of the vector 
    void push_back(T item); 

    // pop (remove) the last item in the vector 
    void pop_back() noexcept; 

    // construct a object in place to the back of the vector 
    void emplace_back(T&& item); 

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
    bool operator==(const Vector<T, Allocator>& other) const 
    requires std::equality_comparable<T>;  

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


    // TODO 

    // emplace back 

private: 

    using alloc_traits = std::allocator_traits<Allocator>; 

    // number items in vec   
    std::size_t size_; 

    // # underlying capacity
    std::size_t cap_; 

    T* data_ = nullptr; 

    Allocator alloc_; 


    // double capacity of vector
    void add_cap_(); 
};


template <typename T, typename Allocator> 
Vector<T, Allocator>::Vector() { 
    size_ = 0; 
    cap_ = 0; 
}

template <typename T, typename Allocator> 
Vector<T, Allocator>::Vector(std::size_t size) : size_(size), cap_(size) { 
    data_ = alloc_traits::allocate(alloc_, size); 

    for (std::size_t i = 0; i < size; ++i) { 
        alloc_traits::construct(alloc_, data_ + i);
    }
}

template <typename T, typename Allocator> 
Vector<T, Allocator>::Vector(const Vector<T, Allocator>& other) 
    : size_(other.size_), 
    cap_(other.cap_), 
    alloc_(alloc_traits::select_on_container_copy_construction(other.alloc_))
{ 
    if (cap_ > 0) { 

        data_ = alloc_traits::allocate(alloc_, cap_);

        for (std::size_t i = 0; i < size_; ++i) { 
            alloc_traits::construct(alloc_, data_ + i, other.data_[i]);
        }

    } else data_ = nullptr; 
}

template <typename T, typename Allocator> 
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector<T, Allocator>& other) 
{
    if (this == &other) return *this; 

    for (std::size_t i = 0; i < size_; ++i) { 
        alloc_traits::destroy(alloc_, data_ + i);
    }

    if (data_) { 
        alloc_traits::deallocate(alloc_, data_, cap_);
    }

    if constexpr (alloc_traits::propagate_on_container_copy_assignment::value) { 
        alloc_ = other.alloc_; // replace allocator with other allocator
    } 

    cap_ = other.cap_; 
    size_ = other.size_;

    if (cap_ > 0) { 

        T* new_data = alloc_traits::allocate(alloc_, cap_); 

        for (std::size_t i = 0; i < size_; ++i) { 
            alloc_traits::construct(alloc_, data_ + i, other.data_ + i);
        }

    } else data_ = nullptr; 

    return *this; 
}

template <typename T, typename Allocator> 
Vector<T, Allocator>::Vector(Vector<T, Allocator>&& other) noexcept
    : size_(other.size_), cap_(other.cap_), data_(other.data_),
    alloc_(std::move(other.alloc_))
{
    other.size_ = 0; 
    other.cap_ = 0; 
    other.data_ = nullptr; 
}

template <typename T, typename Allocator> 
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector<T, Allocator>&& other) noexcept {

    if (this == &other) return *this; 

    if constexpr (alloc_traits::propagate_on_container_move_assignment::value) { 
        alloc_ = std::move(other.alloc_);
    }

    for (std::size_t i = 0; i < size_; ++i) { 
        alloc_traits::destroy(alloc_, data_ + i);
    }

    if (data_) { 
        alloc_traits::deallocate(alloc_, data_, cap_); 
    }

    cap_ = other.cap_; 
    size_ = other.size_; 
    data_ = other.data_; 

    other.size_ = 0; 
    other.cap_ = 0; 
    other.data_ = nullptr; 

    return *this;
}

template <typename T, typename Allocator> 
Vector<T, Allocator>::~Vector() noexcept {
    for (std::size_t i = 0; i < size_; ++i) { 
        alloc_traits::destroy(alloc_, data_ + i); // destructs objects
    }

    if (data_) alloc_traits::deallocate(alloc_, data_, cap_); // dealloc memory block
}

template <typename T, typename Allocator> 
void Vector<T, Allocator>::push_back(T item) { 
    if (size_ + 1 >= cap_) { 
        add_cap_();
    }

    data_[size_] = item; 
    ++size_; 
}

template <typename T, typename Allocator>   
void Vector<T, Allocator>::emplace_back(T&& item) { 
    if (size_ + 1 > cap_) { 
        add_cap_();
    }

    alloc_traits::construct(alloc_, data_ + size_, std::forward<T>(item)); 
    ++size_; 
}

template <typename T, typename Allocator> 
void Vector<T, Allocator>::pop_back() noexcept { 
    alloc_traits::destroy(alloc_, data_ + size_ - 1);
    --size_; 
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::add_cap_() { 
    std::size_t new_cap = cap_ == 0 ? 1 : cap_ * 2;
    
    T* new_data = alloc_traits::allocate(alloc_, new_cap);
    for (std::size_t i = 0; i < size_; ++i) { 
        alloc_traits::construct(
            alloc_, 
            new_data + i, 
            std::move_if_noexcept(data_[i])
        );
    }

    for (std::size_t i = 0; i < size_; ++i) { 
        alloc_traits::destroy(alloc_, data_ + i);
    }

    if (data_) { 
        alloc_traits::deallocate(alloc_, data_, cap_);
    }

    cap_ = new_cap;
    data_ = new_data;
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::at(std::size_t index) { 
    if (index < 0 || index >= size_) { 
        throw std::out_of_range("index out of bounds");
    }
    return data_[index];
}

template <typename T, typename Allocator> 
const T& Vector<T, Allocator>::at(std::size_t index) const { 
    if (index < 0 || index >= size_) { 
        throw std::out_of_range("index out of bounds");
    }
    return data_[index];
}

template <typename T, typename Allocator> 
bool Vector<T, Allocator>::operator==(const Vector<T, Allocator>& other) const 
requires std::equality_comparable<T> { 
    if (other.size() != size_) return false; 

    for (std::size_t i = 0; i < size_; ++i) { 
        if (other[i] != data_[i]) return false; 
    }
    return true; 
}

template <typename T, typename Allocator> 
void Vector<T, Allocator>::reserve(std::size_t size) {  
    if (size <= cap_) return; 

    T* new_data = alloc_traits::allocate(alloc_, size);

    for (std::size_t i = 0; i < size_; ++i) { 
        alloc_traits::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
    }

    for (std::size_t i = 0; i < size_; ++i) { 
        alloc_traits::destroy(alloc_, data_ + i);
    }

    if (data_) { 
        alloc_traits::deallocate(alloc_, data_, cap_);
    }

    data_ = new_data; 
    cap_ = size; 
}

template <typename T, typename Allocator> 
void Vector<T, Allocator>::shrink_to_fit() { 
    if (size_ == cap_) return; 

    T* new_data = alloc_traits::allocate(alloc_, size_); 

    for (std::size_t i = 0; i < size_; ++i) { 
        alloc_traits::construct(
            alloc_, 
            new_data + i, 
            std::move_if_noexcept(data_[i])
        );
    }

    for (std::size_t i = 0; i < size_; ++i) { 
        alloc_traits::destroy(alloc_, data_ + i); 
    }

    if (data_) { 
      alloc_traits::deallocate(alloc_, data_, cap_);
    }

    data_ = new_data; 
    cap_ = size_; 
}

template <typename T, typename Allocator> 
void Vector<T, Allocator>::clear() { 
    for (std::size_t i = 0; i < size_; ++i) { 
        alloc_traits::destroy(alloc_, data_ + i);
    }
    size_ = 0;
}

template <typename T, typename Allocator> 
void Vector<T, Allocator>::resize(std::size_t size) {  
    if (size < size_) { // delete items past size (cap_ remains same)

        for (std::size_t i = size; i < size_; ++i) { 
            alloc_traits::destroy(alloc_, data_ + i);
        }

        size_ = size; 
    } else if (size != size_) { // extend capacity (default construction)
    
        T* new_data = alloc_traits::allocate(alloc_, size); 
        for (std::size_t i = 0; i < size_; ++i) { 
            alloc_traits::construct(
                alloc_, 
                new_data + i, 
                std::move_if_noexcept(data_[i])
            );
        }

        for (std::size_t i = size; i < size; ++i) { 
            alloc_traits::construct(alloc_, new_data + i);
        }
        
        for (std::size_t i = 0; i < size_; ++i) { 
            alloc_traits::destroy(alloc_, data_ + i);
        }

        if (data_) { 
            alloc_traits::deallocate(alloc_, data_, cap_);
        }

        cap_ = size; 
        size_ = size; 
        data_ = new_data; 
    }
 
}

