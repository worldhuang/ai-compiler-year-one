#pragma once

#include "minitensor/tensor.hpp"

#include <cstddef>
#include <memory>

namespace minitensor{
    class TensorView{
    public:
        TensorView(
            std::shared_ptr<Storage<float>> storage,
            Shape shape,
            Strides stride,
            std::size_t offset
        );
        [[nodiscard]] const Shape& shape() const noexcept;
        [[nodiscard]] const Strides& stride() const noexcept;
        [[nodiscard]] std::size_t offset() const noexcept;

        [[nodiscard]] float& at(std::size_t index);
        [[nodiscard]] const float& at(std::size_t index) const;

        [[nodiscard]] std::size_t rank() const noexcept;
        [[nodiscard]] std::size_t numel() const noexcept;
        [[nodiscard]] bool is_constiguous() const noexcept;

        [[nodiscard]] TensorView reshape(Shape new_shape) const;
        [[nodiscard]] TensorView transpose() const;

        [[nodiscard]] float& at(std::size_t row, std::size_t col);
        [[nodiscard]] const float& at(std::size_t row, std::size_t col) const;

    private:
        static Strides make_contiguous_stride(const Shape& shape);
        static std::size_t element_count(const Shape& shape);
        
        std::shared_ptr<Storage<float>> storage_;
        Shape shape_;
        Strides stride_;
        std::size_t offset_;
    };
}