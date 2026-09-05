#include "minitensor/operators.hpp"

#include <algorithm>
#include <stdexcept>

namespace minitensor {
Tensor matmul_naive(const Tensor& lhs, const Tensor& rhs) {
    if (lhs.rank() != 2 || rhs.rank() != 2) {
        throw std::invalid_argument("matmul requires 2D tensors");
    }
    
    const std::size_t lhs_m = lhs.shape()[0];
    const std::size_t lhs_k = lhs.shape()[1];
    const std::size_t rhs_k = rhs.shape()[0];
    const std::size_t rhs_n = rhs.shape()[1];

    if (lhs_k != rhs_k) {
        throw std::invalid_argument("matmul inner dimensions must match");
    }

    Tensor output({lhs_m, rhs_n});
    output.fill(0.0F);

    const auto lhs_data = lhs.data();
    const auto rhs_data = rhs.data();
    auto output_data = output.data();

    for (std::size_t row=0; row<lhs_m; ++row){
        for (std::size_t col = 0; col < rhs_n; ++col) {
            float sum = 0.0F;

            for (std::size_t inner = 0; inner < rhs_k; ++inner){
                sum+=lhs_data[row*rhs_k+inner]*rhs_data[inner*rhs_n+col];
            }

            output_data[row * rhs_n + col] = sum;
        }
    }
    return output;
}

Tensor matmul_tiled(
    const Tensor& lhs,
    const Tensor& rhs,
    std::size_t tile_size) {
    if (tile_size == 0) {
        throw std::invalid_argument("tile_size must be greater than zero");
    }

    if (lhs.rank() != 2 || rhs.rank() != 2) {
        throw std::invalid_argument("matmul requires 2D tensors");
    }

    const std::size_t m = lhs.shape()[0];
    const std::size_t k = lhs.shape()[1];
    const std::size_t rhs_k = rhs.shape()[0];
    const std::size_t n = rhs.shape()[1];

    if (k != rhs_k) {
        throw std::invalid_argument("matmul inner dimensions must match");
    }

    Tensor output({m, n});
    output.fill(0.0F);

    const auto lhs_data = lhs.data();
    const auto rhs_data = rhs.data();
    auto output_data = output.data();

    for (std::size_t row_block = 0; row_block < m; row_block += tile_size) {
        const std::size_t row_end =
            std::min(row_block + tile_size, m);

        for (std::size_t inner_block = 0;
             inner_block < k;
             inner_block += tile_size) {
            const std::size_t inner_end =
                std::min(inner_block + tile_size, k);

            for (std::size_t col_block = 0;
                 col_block < n;
                 col_block += tile_size) {
                const std::size_t col_end =
                    std::min(col_block + tile_size, n);

                for (std::size_t row = row_block;
                     row < row_end;
                     ++row) {
                    for (std::size_t inner = inner_block;
                         inner < inner_end;
                         ++inner) {
                        const float lhs_value =
                            lhs_data[row * k + inner];

                        for (std::size_t col = col_block;
                             col < col_end;
                             ++col) {
                            output_data[row * n + col] +=
                                lhs_value * rhs_data[inner * n + col];
                        }
                    }
                }
            }
        }
    }

    return output;
}
}