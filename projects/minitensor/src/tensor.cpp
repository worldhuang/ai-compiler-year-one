#include "minitensor/tensor.hpp"
#include "minitensor/tensor_view.hpp"

#include <sstream>//std::ostringstream
#include <utility>
#include <stdexcept>

namespace minitensor{

namespace
{
    std::string vector_to_string(const std::vector<std::size_t>& values){
        std::ostringstream output;
        output << "[";
        for (std::size_t i=0; i<values.size(); ++i){
            if (i != 0){
                output << ", ";
            }
            output << values[i];
        }
        output << "]";
        return output.str();
    }

    const char* dtype_to_string(DType dtype){
        switch (dtype){
            case DType::Float32:
                return "Float32";
            default:
                return "Unknown";
        }
    }
}//namespace

    Tensor::Tensor(
    Shape shape,
    DType dtype
    )
    : shape_(shape.begin(), shape.end()),
      strides_(make_contiguous_stride(shape_)),
      dtype_(dtype),
      storage_(std::make_shared<Storage<float>>(element_count(shape_))) {}

    void Tensor::fill(float value) noexcept{
        for (std::size_t index=0; index<storage_->size(); ++index){
            storage_->at(index)=value;
        }
    }
    
    float& Tensor::at(std::size_t index){
        if (rank() != 1){
            throw std::invalid_argument("tensor index requires a 1D tensor");
        }
        if (index >= shape_[0]){
            throw std::out_of_range("tensor index requires a 1D tensor");
        }
        return storage_->at(index * strides_[0]);
    }


    const float& Tensor::at(std::size_t index) const{
        return const_cast<Tensor*>(this)->at(index);
    }

    float& Tensor::at(std::size_t row, std::size_t col) {
        if (rank() != 2) {
            throw std::invalid_argument("at(row, col) requires a 2D tensor");
        }
        if (row >= shape_[0] || col >= shape_[1]) {
            throw std::out_of_range("tensor index out of range");
        }

        const std::size_t offset = row * strides_[0] + col * strides_[1];
        return storage_->at(offset);
    }

    const float& Tensor::at(std::size_t row, std::size_t col) const {
        return const_cast<Tensor*>(this)->at(row, col);
    }

    std::size_t Tensor::rank() const noexcept {
        return shape_.size();
    }

    const Shape& Tensor::shape() const noexcept {
        return shape_;
    }

    const Strides& Tensor::strides() const noexcept {
        return strides_;
    }

    DType Tensor::dtype() const noexcept {
        return dtype_;
    }


    std::size_t Tensor::numel() const noexcept{
        return storage_->size();
    }

    bool Tensor::is_contiguous() const noexcept {
        if (shape_.size() != strides_.size()) {
            return false;
        }

        std::size_t expected_stride = 1;

        for (std::size_t i = shape_.size(); i > 0; --i) {
            const std::size_t index = i - 1;

            if (strides_[index] != expected_stride) {
                return false;
            }

            expected_stride *= shape_[index];
        }

        return true;
    }

    std::span<float> Tensor::data() noexcept {
        return {storage_->data(), storage_->size()};
    }

    std::span<const float> Tensor::data() const noexcept {
        return {storage_->data(), storage_->size()};
    }

    std::string Tensor::to_string() const {
        std::ostringstream output;

        output << "Tensor("
            << "shape=" << vector_to_string(shape_)
            << ", stride=" << vector_to_string(strides_)
            << ", dtype=" << dtype_to_string(dtype_)
            << ", numel=" << numel()
            << ")";

        return output.str();
    }

    TensorView Tensor::row_view(std::size_t row) {
        if (rank() != 2) {
            throw std::invalid_argument("row_view requires a 2D tensor");
        }

        if (row >= shape_[0]) {
            throw std::out_of_range("row index out of range");
        }

        return TensorView(
            storage_,
            Shape{shape_[1]},
            Strides{strides_[1]},
            row * strides_[0]);
    }

    TensorView Tensor::view(){
        return TensorView(storage_, shape_, strides_, 0);
    }

    Strides Tensor::make_contiguous_stride(const Shape& shape) {
        Strides strides(shape.size());
        std::size_t current = 1;

        for (std::size_t i = shape.size(); i > 0; --i) {
            strides[i - 1] = current;
            current *= shape[i - 1];
        }

        return strides;
    }

    std::size_t Tensor::element_count(const Shape& shape) {
        std::size_t total = 1;

        for (std::size_t dimension : shape) {
            total *= dimension;
        }

        return total;
    }

}