#pragma once

#include <string>

namespace minitensor{

class Tensor{
public:
    Tensor();
    [[nodiscard]] std::string name() const;
};

}