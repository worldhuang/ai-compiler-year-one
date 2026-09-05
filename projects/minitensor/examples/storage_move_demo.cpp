#include "minitensor/storage.hpp"

#include <iostream>
#include <utility>

void print_state(const char* name, const minitensor::Storage<float>& storage) {
    std::cout << name
              << ": size=" << storage.size()
              << ", data=" << static_cast<const void*>(storage.data())
              << '\n';
}

int main(void){
    minitensor::Storage<float> source(4);

    print_state("source before move", source);

    minitensor::Storage<float> target(std::move(source));

    print_state("source after move ", source);
    print_state("target after move ", target);
}