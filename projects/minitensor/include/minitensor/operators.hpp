#include "minitensor/tensor.hpp"

#include <cstddef>

namespace minitensor {
    [[nodiscard]] Tensor matmul_naive(const Tensor& lhs, const Tensor& rhs);

    [[nodiscard]] Tensor matmul_tiled(const Tensor& lhs, const Tensor& rhs, 
                    std::size_t tile_size);
}