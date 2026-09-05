#include "minitensor/operators.hpp"

#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>

namespace minitensor{
namespace{
    #define EXPECT_CLOSE(a,b,rtol,atol)                         \
    do{                                                         \
        const auto val_a=(a);                                   \
        const auto val_b=(b);                                   \
        if(!std::isfinite(val_a)||!std::isfinite(val_b)){       \
            ADD_FAILURE(); \
        }else{                                                  \
            const auto diff=std::abs(val_a-val_b);              \
            const auto maxv=std::max(std::abs(val_a),std::abs(val_b));\
            if(diff>(atol)+(rtol)*maxv){                        \
                ADD_FAILURE();\
            }                                                   \
        }                                                       \
    }while(0)

    TEST(TestOperators, TestClose)
    {
        minitensor::Tensor lhs({2, 3});
        minitensor::Tensor rhs({3, 2});


        lhs.at(0, 0) = 1.0F;
        lhs.at(0, 1) = 2.0F;
        lhs.at(0, 2) = 3.0F;
        lhs.at(1, 0) = 4.0F;
        lhs.at(1, 1) = 5.0F;
        lhs.at(1, 2) = 6.0F;

        rhs.at(0, 0) = 7.0F;
        rhs.at(0, 1) = 8.0F;
        rhs.at(1, 0) = 9.0F;
        rhs.at(1, 1) = 10.0F;
        rhs.at(2, 0) = 11.0F;
        rhs.at(2, 1) = 12.0F;

        const minitensor::Tensor output1 =
        minitensor::matmul_naive(lhs, rhs);

        const minitensor::Tensor output2 =
        minitensor::matmul_tiled(lhs, rhs, 2);

        EXPECT_CLOSE(output1.at(0,0),output2.at(0,0),1e-5F,1e-5F);
        EXPECT_CLOSE(output1.at(0,1),output2.at(0,1),1e-5F,1e-5F);
        EXPECT_CLOSE(output1.at(1,0),output2.at(1,0),1e-5F,1e-5F);
        EXPECT_CLOSE(output1.at(1,1),output2.at(1,1),1e-5F,1e-5F);
    }
}
}