#pragma once

#include "array_ptr.h"

#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <utility>

#include <iterator>

class ReserveProxyObj {
public:
    size_t capacity___;

    ReserveProxyObj(size_t capacity)
        :capacity___(capacity) {}
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size)
        :array_(size)
        , size_(size)
        , capacity_(size)
    {
        for (size_t i = 0; i < size; ++i) {
            array_[i] = {};
        }
    }

    SimpleVector(size_t size, const Type& value)
         :array_(size)
         , size_(size)
         , capacity_(size)
    {
        for (size_t i = 0; i < size; ++i) {
            array_[i] = value;
        }
    }

    SimpleVector(size_t size, Type&& value)  // переносим один и тот же VALUE, подумать про fill
        :array_(size)
        , size_(size)
        , capacity_(size)
    {
        for (size_t i = 0; i < size; ++i) {
            std::fill(begin(), end(), value);
        }
    }

    SimpleVector(std::initializer_list<Type> init)
        :array_(init.size())
        , size_(init.size())
        , capacity_(init.size())
    {
        int i = 0;
        for (const auto& it : init) {
            array_[i] = it;
            ++i;
        }
    }

    SimpleVector(const SimpleVector& other)
        :array_(other.size_)
        , size_(other.size_)
        , capacity_(other.capacity_)
    {
        for (size_t i = 0; i < other.size_; ++i) {
            array_[i] = other.array_[i];
        }
    }

    SimpleVector(SimpleVector&& other)
        :array_(other.size_)
        , size_(other.size_)
        , capacity_(other.capacity_)
    {
        for (size_t i = 0; i < other.size_; ++i) {
            array_[i] = std::move(other.array_[i]);
        }
        other.Clear();/////////////////////////////////////
    }

    SimpleVector(ReserveProxyObj ReserveCapacity)
        :array_()
        , size_()
        , capacity_(ReserveCapacity.capacity___)
    {
    }



    SimpleVector& operator=(const SimpleVector& rhs) {
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        ArrayPtr<Type> tmp_array(capacity_);
        for (size_t i = 0; i < size_; ++i) {
            tmp_array[i] = rhs[i];
        }
        for (size_t i = size_; i < capacity_; ++i) {
            tmp_array[i] = {};
        }
        array_.swap(tmp_array);
        return *this;
    }

    SimpleVector& operator=(SimpleVector&& rhs) {
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        ArrayPtr<Type> tmp_array(capacity_);
        for (size_t i = 0; i < size_; ++i) {
            tmp_array[i] = std::move(rhs[i]);
        }
        for (size_t i = size_; i < capacity_; ++i) {
            tmp_array[i] = {};
        }
        array_.swap(tmp_array);
        return *this;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) { return; }

        ArrayPtr<Type> tmp(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            tmp[i] = array_[i];
        }
        array_.swap(tmp);
        capacity_ = new_capacity;

    }

    size_t GetSize() const noexcept {
        return size_;
    }

    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    bool IsEmpty() const noexcept {
        return (size_ == 0);
    }

    Type& operator[](size_t index) noexcept {
        return array_[index];
    }


   
    const Type& operator[](size_t index) const noexcept {
        return array_[index]; 
    }

    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("out_of_range");
        }
        return array_[index];
    }

    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("out_of_range");
        }
        return array_[index];
    }

    void Clear() noexcept {
        size_ = 0;
    }

    void Resize(size_t new_size) {
        if (new_size <= capacity_) {
            for (size_t i = size_ - 1; i < new_size; ++i) {
                array_[i]={};
            }
        }
        else {
            ArrayPtr<Type> tmp(new_size);
            for (size_t i = 0; i < size_; ++i) {
                tmp[i] = std::move(array_[i]);
            }
            for (size_t i = size_; i < new_size; ++i) {
                tmp[i] = {};
            }
            array_.swap(tmp);
            capacity_ = new_size;
        }
        size_ = new_size;
    }

    void PushBack(const Type& item) {
        if (capacity_ == 0) {
            capacity_ = 1;
            ArrayPtr<Type> tmp_array(capacity_);
            tmp_array[size_] = item;
            array_.swap(tmp_array);
            ++size_;
            return;
        }

        if (size_ < capacity_) {
            array_[size_] = item;
            ++size_;
            return;
        }

        capacity_ *= 2;
        ArrayPtr<Type> tmp_array(capacity_);
        for (size_t i = 0; i < size_; ++i) {
            tmp_array[i] = array_[i];
        }
        array_.swap(tmp_array);
        array_[size_] = item;
        ++size_;
    }

    void PushBack(Type&& item) {
        if (capacity_ == 0) {
            capacity_ = 1;
            ArrayPtr<Type> tmp_array(capacity_);
            tmp_array[size_] = std::move(item);
            array_.swap(tmp_array);
            ++size_;
            return;
        }

        if (size_ < capacity_) {
            array_[size_] = std::move(item);
            ++size_;
            return;
        }

        capacity_ *= 2;
        ArrayPtr<Type> tmp_array(capacity_);
        for (size_t i = 0; i < size_; ++i) {
            tmp_array[i] = std::move(array_[i]);
        }
        array_.swap(tmp_array);
        array_[size_] = std::move(item);
        ++size_;
    }


    Iterator Insert(ConstIterator pos, const Type& value) {
        size_t count = pos - begin();
        if (capacity_ == 0) {
            capacity_ = 1;
            ArrayPtr<Type> tmp_array(capacity_);
            tmp_array[0] = value;
            array_.swap(tmp_array);
            ++size_;
            return &array_[count];
        }
        if (size_ == capacity_) {
            capacity_ = size_ * 2;
            ArrayPtr<Type> tmp(capacity_);

            for (size_t i = 0; i < size_; ++i) {
                tmp[i] = array_[i];
            }
            array_.swap(tmp);
        }
        ++size_;
        std::copy_backward(&array_[count], end(), end() + 1);
        array_[count] = value;
        return &array_[count];
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        size_t count = pos - begin();
        if (capacity_ == 0) {
            capacity_ = 1;
            ArrayPtr<Type> tmp_array(capacity_);
            tmp_array[0] = std::move(value);
            array_.swap(tmp_array);
            ++size_;
            return &array_[count];
        }
        if (size_ == capacity_) {
            capacity_ = size_ * 2;
            ArrayPtr<Type> tmp(capacity_);

            for (size_t i = 0; i < size_; ++i) {
                tmp[i] = std::move(array_[i]);
            }
            array_.swap(tmp);
        }
        ++size_;
        std::move_backward(&array_[count], end(), end() + 1);  /////////////////////////////////////////////
        array_[count] = std::move(value);
        return &array_[count];
    }

    void PopBack() noexcept {
        if (size_ > 0) {
            --size_;
        }
    }

    Iterator Erase(ConstIterator pos) {
        if (size_ == 0) {
            return begin();
        }
        size_t count = pos - begin();
        for (size_t i = size_-1; i > count; --i) {
            array_[i - 1] = array_[i];
        }
        --size_;
        return &array_[count];
    }

    Iterator Erase(Iterator pos) {
        if (size_ == 0) {
            return begin();
        }
        size_t count = pos - begin();
        std::move(pos + 1, end(), Iterator(pos));
        --size_;
        return &array_[count];
    }

    void swap(SimpleVector& other) noexcept {
        array_.swap(other.array_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    Iterator begin() noexcept {
        return &array_[0];
    }

    Iterator end() noexcept {
        return &array_[size_];
    }

    ConstIterator begin() const noexcept {
        return cbegin();
    }

    ConstIterator end() const noexcept {
        return cend();
    }

    ConstIterator cbegin() const noexcept {
        return &array_[0];
    }

    ConstIterator cend() const noexcept {
        return &array_[size_];
    }

private:
    ArrayPtr<Type> array_{};
    size_t size_{};
    size_t capacity_{};


};


template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs>rhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs<rhs);
}

