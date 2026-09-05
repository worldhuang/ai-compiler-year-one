#pragma once

#include <cstddef>//size_t、ptrdiff_t
#include <memory>//unique_ptr...
#include <stdexcept>//std::out_of_range、std::invalid_argument、std::runtime_error
#include <utility>//std::move\std::exchage\std::forward
#include <algorithm>//std::copy/copy_n,swap,max,min,fill,find,sort

namespace minitensor {

template <typename T>
class Storage {
public:
    explicit Storage(std::size_t size)//修饰单参数构造函数，禁止编译器进行隐性类型转换
        : size_(size),
          data_(size == 0 ? nullptr : std::make_unique<T[]>(size_)){}
    
    Storage(const Storage& other)
        : size_(other.size_),
         data_(size_ == 0 ? nullptr : std::make_unique<T[]>(size)){
         if (size_ != 0) {
            std::copy_n(other.data_.get(), size_, data_.get());
         }
         }
    Storage& operator=(const Storage& other) {
        if (this == &other){
            return *this;
        }
        Storage copy(other);
        swap(copy);
        return *this;
    }

    Storage(Storage&& other) noexcept 
        :size_(std::exchange(other.size_, 0)),
         data_(std::move(other.data_)){}
 
    Storage& operator=(Storage&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }

    void swap(Storage& other) noexcept {
        using std::swap;

        swap(size_, other.size_);
        swap(data_, other.data_);
    }

    ~Storage() = default;

    [[nodiscard]] std::size_t size() const noexcept {
        return size_;
    }

    [[nodiscard]] bool empty() const noexcept {
        return size_ == 0;
    }

    [[nodiscard]] T* data() noexcept {
        return data_.get();
    }

    [[nodiscard]] const T* data() const noexcept{
        return data_.get();
    }

    [[nodiscard]] T& at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("storage index out of range");
        }

        return data_[index];
    }

    [[nodiscard]] const T& at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("storage index out of range");
        }

        return data_[index];
    }

private:
    std::size_t size_;
    std::unique_ptr<T[]> data_;
};
}//namespace minitensor