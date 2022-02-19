#pragma once

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <utility>

template <typename Type>
class ArrayPtr {
public:

    ArrayPtr() = default;

    explicit ArrayPtr(size_t size) {
        if (size != 0) {
            raw_ptr_ = new Type[size];

        }
    }

    explicit ArrayPtr(Type* raw_ptr) noexcept {
        raw_ptr_ = raw_ptr;
    }

    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() {
        clear();
    }

    ArrayPtr& operator=(const ArrayPtr&) = delete;

    [[nodiscard]] Type* Release() noexcept {
        Type* res = raw_ptr_;
        raw_ptr_ = nullptr;
        return res;
    }

    Type& operator[](size_t index) noexcept {
        return raw_ptr_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        const Type& res = raw_ptr_[index];
        return res;
    }

    explicit operator bool() const {
        return (raw_ptr_ != nullptr);
    }

    Type* Get() const noexcept {
        return raw_ptr_;
    }

    void swap(ArrayPtr& other) noexcept {
        std::swap(raw_ptr_, other.raw_ptr_);
    }

private:

    void clear() {
        delete[] raw_ptr_;
    }

    Type* raw_ptr_ = nullptr;
};

