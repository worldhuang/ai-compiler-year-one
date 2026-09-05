#pragma once

#include <cstddef>
#include <span>
#include <vector>
#include <string>
#include <memory>

#include "minitensor/storage.hpp"

namespace minitensor{

    using Shape = std::vector<std::size_t>;
    using Strides = std::vector<std::size_t>;

    enum class DType {
        Float32,
        Float64,
        Int32,
    };

    class TensorView;

    class Tensor{
    public:
        explicit Tensor(
            Shape shape,
            DType dtype = DType::Float32
        );

        void fill(float value) noexcept;
        [[nodiscard]] float& at(std::size_t index);
        [[nodiscard]] const float& at(std::size_t index) const;

        [[nodiscard]] float& at(std::size_t row, std::size_t col);
        [[nodiscard]] const float& at(std::size_t row, std::size_t col) const;

        [[nodiscard]] std::size_t rank() const noexcept;
        [[nodiscard]] const Shape& shape() const noexcept;
        [[nodiscard]] const Strides& strides() const noexcept;
        [[nodiscard]] DType dtype() const noexcept;
        [[nodiscard]] std::size_t numel() const noexcept;
        [[nodiscard]] bool is_contiguous() const noexcept;

        [[nodiscard]] std::span<float> data() noexcept;
        [[nodiscard]] std::span<const float> data() const noexcept;

        [[nodiscard]] std::string to_string() const;

        [[nodiscard]] TensorView row_view(std::size_t row);

        [[nodiscard]] TensorView view();

    private:
        static Strides make_contiguous_stride(const Shape& shape);
        static std::size_t element_count(const Shape& shape);
        
        Shape shape_;
        Strides strides_;
        DType dtype_;
        std::shared_ptr<Storage<float>> storage_;
    };

}