#include <cassert>
#include <stdexcept>
#include <gtest/gtest.h>

#include "minitensor/tensor.hpp"
namespace minitensor {
namespace {

TEST(TensorTest, CreatesOrdinaryMatrixWithCorrectMetadata) {
    Tensor matrix({2, 3});

    const Shape expected_shape{2, 3};
    const Strides expected_stride{3, 1};

    EXPECT_EQ(matrix.shape(), expected_shape);
    EXPECT_EQ(matrix.strides(), expected_stride);
    EXPECT_EQ(matrix.rank(), 2U);
    EXPECT_EQ(matrix.numel(), 6U);
    EXPECT_EQ(matrix.dtype(), DType::Float32);
    EXPECT_TRUE(matrix.is_contiguous());
}

TEST(TensorTest, FillSetsEveryElement) {
    Tensor vector({4});

    vector.fill(1.5F);

    for (float value : vector.data()) {
        EXPECT_FLOAT_EQ(value, 1.5F);
    }
}

TEST(TensorTest, OneDimensionalIndexReadsAndWrites) {
    Tensor vector({4});

    vector.fill(0.0F);
    vector.at(0) = 10.0F;
    vector.at(3) = 30.0F;

    EXPECT_FLOAT_EQ(vector.at(0), 10.0F);
    EXPECT_FLOAT_EQ(vector.at(3), 30.0F);
}

TEST(TensorTest, TwoDimensionalIndexMapsToContiguousStorage) {
    Tensor matrix({2, 3});

    matrix.fill(0.0F);
    matrix.at(0, 0) = 10.0F;
    matrix.at(1, 2) = 42.0F;

    EXPECT_FLOAT_EQ(matrix.at(0, 0), 10.0F);
    EXPECT_FLOAT_EQ(matrix.at(1, 2), 42.0F);
    EXPECT_FLOAT_EQ(matrix.data()[0], 10.0F);
    EXPECT_FLOAT_EQ(matrix.data()[5], 42.0F);
}

TEST(TensorTest, EmptyShapeRepresentsScalar) {
    Tensor scalar({});

    EXPECT_EQ(scalar.rank(), 0U);
    EXPECT_EQ(scalar.numel(), 1U);
    EXPECT_TRUE(scalar.shape().empty());
    EXPECT_TRUE(scalar.strides().empty());
    EXPECT_EQ(scalar.data().size(), 1U);
}

TEST(TensorTest, ZeroSizedDimensionCreatesEmptyTensor) {
    Tensor empty_tensor({2, 0, 3});

    EXPECT_EQ(empty_tensor.rank(), 3U);
    EXPECT_EQ(empty_tensor.numel(), 0U);
    EXPECT_TRUE(empty_tensor.data().empty());
}

TEST(TensorTest, OutOfRangeOneDimensionalIndexThrows) {
    Tensor vector({4});

    EXPECT_THROW(static_cast<void>(vector.at(4)), std::out_of_range);
}

TEST(TensorTest, WrongRankAndOutOfRangeTwoDimensionalIndexThrow) {
    Tensor matrix({2, 3});
    Tensor empty_matrix({2, 0});

    EXPECT_THROW(static_cast<void>(matrix.at(1)), std::invalid_argument);
    EXPECT_THROW(static_cast<void>(matrix.at(2, 0)), std::out_of_range);
    EXPECT_THROW(static_cast<void>(matrix.at(0, 3)), std::out_of_range);
    EXPECT_THROW(static_cast<void>(empty_matrix.at(0, 0)), std::out_of_range);
}

}  // namespace
}  // namespace minitensor