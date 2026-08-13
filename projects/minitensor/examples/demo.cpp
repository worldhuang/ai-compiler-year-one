#include <iostream>

#include "minitensor/tensor.hpp"

int main(){
    minitensor::Tensor tensor;
    std::cout << tensor.name() << " is alive\n";
    return 0;
}