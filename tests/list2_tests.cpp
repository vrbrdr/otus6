#include "../list2.hpp"
#include "cint.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <type_traits>

namespace {
    using namespace containers;

    TEST(list2_specific, ForwardItems) {
        {
            list2<cint> l;

            cint::copy_count = 0;
            cint::move_count = 0;

            // rvalue
            l.push_back(100);
            EXPECT_EQ(cint::copy_count, 0);
            EXPECT_EQ(cint::move_count, 1);

            // rvalue
            l.insert(0, 300);
            EXPECT_EQ(cint::copy_count, 0);
            EXPECT_EQ(cint::move_count, 2); 

            // lvalue
            cint i200(200);
            l.push_back(i200);
            EXPECT_EQ(cint::copy_count, 1);
            EXPECT_EQ(cint::move_count, 2);

            // lvalue
            cint i400(400);
            l.insert(0, i400);
            EXPECT_EQ(cint::copy_count, 2);
            EXPECT_EQ(cint::move_count, 2); 

            l.erase(1);
            EXPECT_EQ(cint::copy_count, 2);
            EXPECT_EQ(cint::move_count, 2); 

            l.erase(0);
            EXPECT_EQ(cint::copy_count, 2);
            EXPECT_EQ(cint::move_count, 2); 

            l.erase(1);
            EXPECT_EQ(cint::copy_count, 2);
            EXPECT_EQ(cint::move_count, 2);
        }

        EXPECT_EQ(cint::instances, 0);
    }
} // namespace