#include <iostream>
#include <vector>

#include "minitensor/tensor.hpp"

int main(){
    const std::vector<std::size_t> arr{3};
    const std::vector<std::size_t> shape{2,0,4};
    const std::vector<std::size_t> cube{1,2,3};

    const minitensor::Tensor tensor(
        shape,
        minitensor::DType::Float32
    );

    const minitensor::Tensor cube_tensor(
        cube,
        minitensor::DType::Float32
    );
    const minitensor::Tensor arr_1d(
        arr,
        minitensor::DType::Float32
    );

    std::cout << "1D_strides: " << arr_1d.strides()[0] << '\n';
    std::cout << "rank: " << tensor.rank() << '\n';
    std::cout << "shape: ["
              << tensor.shape()[0] << ", "
              << tensor.shape()[1] << "]\n";
    std::cout << tensor.numel() << '\n';

    std::cout << "cube: ";
    for (auto num: cube_tensor.data()){
        std::cout << num << "  ";
    }
              
    return 0;
}