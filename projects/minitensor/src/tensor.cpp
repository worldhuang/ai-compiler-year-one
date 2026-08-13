#include "minitensor/tensor.hpp"

namespace minitensor{

    Tensor::Tensor() = default;

    std::string Tensor::name() const{
        return "MiniTensor";
    }
}