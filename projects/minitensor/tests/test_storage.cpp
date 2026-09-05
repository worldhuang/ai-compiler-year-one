#include "minitensor/storage.hpp"

#include <gtest/gtest.h>
#include <stdexcept>

namespace minitensor{
namespace {

    TEST (storageTest, AllcoatesContiguousMemory){
        Storage<float> storage(4);

        EXPECT_EQ(storage.size(), 4U);
        EXPECT_FALSE(storage.empty());
        EXPECT_NE(storage.data(), nullptr);
        storage.at(0) = 1.0F;
        storage.at(1) = 2.0F;
        storage.at(2) = 3.0F;
        storage.at(3) = 4.0F;

        EXPECT_EQ(&storage.at(0), storage.data());
        EXPECT_EQ(&storage.at(1), storage.data() + 1);
        EXPECT_EQ(&storage.at(3), storage.data() + 3);
    }

    TEST(StorageTest, SupportsZeroSizedStorage) {
        Storage<float> storage(0);

        EXPECT_EQ(storage.size(), 0U);
        EXPECT_TRUE(storage.empty());
        EXPECT_EQ(storage.data(), nullptr);
    }

    TEST(StorageTest, ThrowsForOutOfRangeAccess) {
        Storage<int> storage(2);

        EXPECT_THROW(static_cast<void>(storage.at(2)), std::out_of_range);
    }

    struct LifetimeProbe {
        static inline int alive_count = 0;

        LifetimeProbe() {
            ++alive_count;
        }

        ~LifetimeProbe() {
            --alive_count;
        }
    };

    TEST(StorageTest, ReleasesElementsWhenLeavingScope) {
        EXPECT_EQ(LifetimeProbe::alive_count, 0);

        {
            Storage<LifetimeProbe> storage(3);
            EXPECT_EQ(LifetimeProbe::alive_count, 3);
        }

        EXPECT_EQ(LifetimeProbe::alive_count, 0);
    }
    
    TEST(storageTest, MoveConstruct){
        Storage<float> source(3);
        source.at(0) = 10.0F;

        float* original_address = source.data();

        Storage<float> target(std::move(source));

        EXPECT_EQ(target.data(), original_address);
        EXPECT_EQ(target.size(), 3U);
        EXPECT_EQ(target.at(0), 10.0F);

        EXPECT_EQ(source.data(), nullptr);
        EXPECT_EQ(source.size(), 0U);
    }

    TEST(storageTest, MoveAssignment){
        Storage<float> source(3);
        source.at(1) = 20.0F;

        float* original_address = source.data();
        Storage<float> target(2);
        target.at(0) = 999.0F;

        target = std::move(source);

        EXPECT_EQ(target.data(), original_address);
        EXPECT_EQ(target.size(), 3U);
        EXPECT_EQ(target.at(1), 20.0F);

        EXPECT_EQ(source.data(), nullptr);
        EXPECT_EQ(source.size(), 0U);
    }
}
}