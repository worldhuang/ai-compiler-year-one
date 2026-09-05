#include "minitensor/tensor_view.hpp"

#include <stdexcept>
#include <utility>

namespace minitensor {

TensorView::TensorView(
    std::shared_ptr<Storage<float>> storage,
    Shape shape,
    Strides stride,
    std::size_t offset)
    : storage_(std::move(storage)),
      shape_(std::move(shape)),
      stride_(std::move(stride)),
      offset_(offset) {
    if (storage_ == nullptr) {
        throw std::invalid_argument("TensorView storage must not be null");
    }

    if (shape_.size() != stride_.size()) {
        throw std::invalid_argument("TensorView shape and stride ranks differ");
    }

    if (offset_ > storage_->size()) {
        throw std::out_of_range("TensorView offset out of range");
    }
}

const Shape& TensorView::shape() const noexcept {
    return shape_;
}

const Strides& TensorView::stride() const noexcept {
    return stride_;
}

std::size_t TensorView::offset() const noexcept {
    return offset_;
}

float& TensorView::at(std::size_t index) {
    if (shape_.size() != 1) {
        throw std::invalid_argument("TensorView::at(index) requires a 1D view");
    }

    if (index >= shape_[0]) {
        throw std::out_of_range("TensorView index out of range");
    }

    return storage_->at(offset_ + index * stride_[0]);
}

const float& TensorView::at(std::size_t index) const {
    if (shape_.size() != 1) {
        throw std::invalid_argument("TensorView::at(index) requires a 1D view");
    }

    if (index >= shape_[0]) {
        throw std::out_of_range("TensorView index out of range");
    }

    return storage_->at(offset_ + index * stride_[0]);  
}

std::size_t TensorView::rank() const noexcept{
    return shape_.size();
}

std::size_t TensorView::numel() const noexcept{
    return storage_->size(); 
}

bool TensorView::is_constiguous() const noexcept{
    if (shape_.size() != stride_.size()) {
            return false;
    }

        std::size_t expected_stride = 1;

    for (std::size_t i = shape_.size(); i > 0; --i) {
        const std::size_t index = i - 1;

        if (stride_[index] != expected_stride) {
            return false;
        }

        expected_stride *= shape_[index];
    }
    return true;
}

TensorView TensorView::reshape(Shape new_shape) const {
    if (!is_constiguous()) {
        throw std::invalid_argument(
            "reshape requires a contiguous TensorView");
    }

    if (element_count(new_shape) != numel()) {
        throw std::invalid_argument(
            "reshape changes the number of elements");
    }

    Strides new_stride = make_contiguous_stride(new_shape);

    return TensorView(
        storage_,
        std::move(new_shape),
        std::move(new_stride),
        offset_);
}

TensorView TensorView::transpose() const {
    if (rank() != 2) {
        throw std::invalid_argument(
            "transpose requires a 2D TensorView");
    }

    return TensorView(
        storage_,
        Shape{shape_[1], shape_[0]},
        Strides{stride_[1], stride_[0]},
        offset_);
}

float& TensorView::at(std::size_t row, std::size_t col){
    if (rank() != 2) {
        throw std::invalid_argument("at(row, col) requires a 2D tensor");
    }
    if (row >= shape_[0] || col >= shape_[1]) {
        throw std::out_of_range("tensor index out of range");
    }

    const std::size_t offset = row * stride_[0] + col * stride_[1];
    return storage_->at(offset);
}

const float& TensorView::at(std::size_t row, std::size_t col) const{
    return const_cast<TensorView*>(this)->at(row, col);
}

Strides TensorView::make_contiguous_stride(const Shape& shape){
    Strides strides(shape.size());
    std::size_t current = 1;

    for (std::size_t i = shape.size(); i>0; --i){
        strides[i-1] = current;
        current*=shape[i-1];
    }
    return strides;
}

std::size_t TensorView::element_count(const Shape& shape){
    std::size_t total = 1;

    for (std::size_t dimension : shape) {
        total *= dimension;
    }

    return total;
}

}  // namespace minitensor