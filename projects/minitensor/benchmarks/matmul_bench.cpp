#include "minitensor/operators.hpp"

#include <algorithm>
#include <chrono>//std::chrono::system_clock/steady_clock/high_resolution_clock
#include <cstddef>
#include <iostream>
#include <vector>

namespace{
    using Clock = std::chrono::steady_clock;

    minitensor::Tensor make_matrix(std::size_t rows, std::size_t cols){
        minitensor::Tensor tensor({rows, cols});
        auto data = tensor.data();

        for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] = static_cast<float>((i * 17U + 3U) % 23U) / 23.0F;
        }

        return tensor;
    }

    float checksum(const minitensor::Tensor& tensor){
        float sum = 0.0F;
        for (float value : tensor.data()) {
            sum += value;
        }

        return sum;
    }

    template <typename Kernel>
    double measure_p50_us(
        Kernel&& kernel,
        int warmup_count,
        int repeat_count) {
            
        float result_checksum = 0.0F;

        // Warmup 不计入统计。
        for (int i = 0; i < warmup_count; ++i) {
            const minitensor::Tensor output = kernel();
            result_checksum += checksum(output);
        }

        std::vector<double> samples;
        samples.reserve(static_cast<std::size_t>(repeat_count));

        for (int i = 0; i < repeat_count; ++i) {
            const auto start = Clock::now();

            const minitensor::Tensor output = kernel();

            const auto end = Clock::now();

            result_checksum += checksum(output);

            const std::chrono::duration<double, std::micro> elapsed =
                end - start;

            samples.push_back(elapsed.count());
        }

        std::sort(samples.begin(), samples.end());

        // 使用 checksum，防止编译器把计算当成无用工作优化掉。
        std::cout << "checksum: " << result_checksum << '\n';

        return samples[samples.size() / 2];
    }
}//namespace

    int main() {
        constexpr std::size_t m = 256;
        constexpr std::size_t k = 256;
        constexpr std::size_t n = 256;

        constexpr std::size_t tile_size = 32;
        constexpr int warmup_count = 5;
        constexpr int repeat_count = 30;

        const minitensor::Tensor lhs = make_matrix(m, k);
        const minitensor::Tensor rhs = make_matrix(k, n);

        const double naive_p50_us = measure_p50_us(
            [&] {
                return minitensor::matmul_naive(lhs, rhs);
            },
            warmup_count,
            repeat_count);

        const double tiled_p50_us = measure_p50_us(
            [&] {
                return minitensor::matmul_tiled(lhs, rhs, tile_size);
            },
            warmup_count,
            repeat_count);

        std::cout << "\nshape: "
                << m << "x" << k << "x" << n << '\n';

        std::cout << "warmup: " << warmup_count
                << ", repeats: " << repeat_count << '\n';

        std::cout << "naive P50: " << naive_p50_us << " us\n";
        std::cout << "tiled P50: " << tiled_p50_us << " us\n";

        std::cout << "speedup: "
                << naive_p50_us / tiled_p50_us
                << "x\n";

        return 0;
    }   
