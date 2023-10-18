#include "../vector2.hpp"
#include "cint.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <type_traits>

namespace {
    using namespace containers;

    TEST(vector2_specific, ForwardItems) {
        {
            vector2<cint> v;

            cint::copy_count = 0;
            cint::move_count = 0;

            // rvalue
            v.push_back(100);
            EXPECT_EQ(cint::copy_count, 0);
            EXPECT_EQ(cint::move_count, 1);

            // rvalue
            v.insert(0, 300);
            EXPECT_EQ(cint::copy_count, 0);
            EXPECT_EQ(cint::move_count, 3); // +1 move in move_tail_right

            // lvalue
            cint i200(200);
            v.push_back(i200);
            EXPECT_EQ(cint::copy_count, 1);
            EXPECT_EQ(cint::move_count, 3);

            // lvalue
            cint i400(400);
            v.insert(0, i400);
            EXPECT_EQ(cint::copy_count, 2);
            EXPECT_EQ(cint::move_count, 6); // +3 move in move_tail_right

            v.erase(1);
            EXPECT_EQ(cint::copy_count, 2);
            EXPECT_EQ(cint::move_count, 8); // +2 move in move_tail_left

            v.erase(0);
            EXPECT_EQ(cint::copy_count, 2);
            EXPECT_EQ(cint::move_count, 10); // +2 move in move_tail_left

            v.erase(1);
            EXPECT_EQ(cint::copy_count, 2);
            EXPECT_EQ(cint::move_count, 10); 
        }

        EXPECT_EQ(cint::instances, 0);
    }
} // namespace