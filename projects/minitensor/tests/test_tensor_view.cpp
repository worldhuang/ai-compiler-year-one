#include "minitensor/tensor.hpp"
#include "minitensor/tensor_view.hpp"

#include <stdexcept>
#include <gtest/gtest.h>

namespace minitensor{
namespace{
    TEST (TestTensorView, TestRowViewMetadata){
        Tensor matrix({2,3});
        TensorView second_row = matrix.row_view(1);

        const Shape expected_shape{3};
        const Strides expected_stride{1};

        EXPECT_EQ(second_row.shape(), expected_shape);
        EXPECT_EQ(second_row.stride(), expected_stride);
        EXPECT_EQ(second_row.offset(), 3U);
    }

    TensorView test_view_outlives_parent_tensor(){
        Tensor matrix({2,3});

        matrix.at(1, 0) = 30.0F;
        matrix.at(1, 1) = 31.0F;
        matrix.at(1, 2) = 32.0F;

        return matrix.row_view(1);

    }

    TEST(TestTensorView, TestInvalidAccessThrows){
        Tensor vector({3});
        Tensor matrix({2, 3});

        EXPECT_THROW(static_cast<void>(vector.row_view(0)), std::invalid_argument);
        EXPECT_THROW(static_cast<void>(matrix.row_view(2)), std::out_of_range);

        TensorView row = matrix.row_view(0);
        EXPECT_THROW(static_cast<void>(row.at(3)), std::out_of_range);
    }

    TEST(TestTensorView, TestFunc_row_view){
        TensorView view = test_view_outlives_parent_tensor();
        EXPECT_EQ(view.at(0), 30.0F);
    }

    TEST(TestTensorView, TestReshapeSharesStorages){
        minitensor::Tensor matrix({2,3});
        matrix.fill(0.0F);

        minitensor::TensorView reshaped = matrix.view().reshape({3,2});
        reshaped.at(2, 1) = 42.0F;
        reshaped.at(1, 0) = 1.0F;

        EXPECT_EQ(matrix.at(1, 2), 42.0F);
        EXPECT_EQ(matrix.at(0, 2), 1.0F);
        EXPECT_TRUE(reshaped.is_constiguous());
    }

    TEST(TestTensorView, TestTrabsposeSharesStorage){
        minitensor::Tensor matrix({2, 3});
        matrix.fill(0.0F);

        minitensor::TensorView transposed = matrix.view().transpose();
        transposed.at(2, 1) = 99.0F;
        EXPECT_EQ(matrix.at(1, 2), 99.0F);
        EXPECT_FALSE(transposed.is_constiguous());
        
    }

    minitensor::TensorView create_second_row_view() {
        minitensor::Tensor matrix({2, 3});

        matrix.at(1, 0) = 10.0F;
        matrix.at(1, 1) = 11.0F;
        matrix.at(1, 2) = 12.0F;

        return matrix.row_view(1);
    }  // matrix 在此销毁

    TEST(TestTensorView, TestOutlivesParentTensor){
        minitensor::TensorView view = create_second_row_view();
        EXPECT_EQ(view.at(0), 10.0F);
        EXPECT_EQ(view.at(1), 11.0F);
        EXPECT_EQ(view.at(2), 12.0F);

        view.at(1) = 999.0F;
        EXPECT_EQ(view.at(1), 999.0F);
    }
}
}

