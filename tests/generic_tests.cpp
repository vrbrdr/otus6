#include "../list2.hpp"
#include "../vector2.hpp"
#include "cint.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <type_traits>

int cint::instances;
int cint::copy_count;
int cint::move_count;

namespace {
    using namespace containers;

    template <typename T> class ContainerTest : public ::testing::Test {};

    using ContainerTypes =
        ::testing::Types<vector2<int>, vector2<cint>, list2<int>, list2<cint>>;

    TYPED_TEST_CASE(ContainerTest, ContainerTypes);

    TYPED_TEST(ContainerTest, InitialState) {
        TypeParam v;
        EXPECT_EQ(v.size(), 0);
        EXPECT_EQ(v.begin(), v.end());
        EXPECT_THAT(v, ::testing::ElementsAre());

        EXPECT_THROW({ v[0] = 1; }, std::out_of_range);
        EXPECT_THROW({ v[0]; }, std::out_of_range);
    }

    TYPED_TEST(ContainerTest, Indexer) {
        {
            TypeParam v;
            v.push_back(typename TypeParam::value_type(10));
            EXPECT_NO_THROW({ v[0] = 99; });
            EXPECT_EQ(v[0], 99);
            EXPECT_NO_THROW({ v[0] = 10; });
            EXPECT_EQ(v[0], 10);

            v.push_back(typename TypeParam::value_type(20));
            v.push_back(typename TypeParam::value_type(30));
            
            EXPECT_THAT(v, ::testing::ElementsAre(10, 20, 30));

            for (size_t i = 0; i < v.size(); ++i) {
                v[i] = typename TypeParam::value_type(100 + i);
            }

            EXPECT_THAT(v, ::testing::ElementsAre(100, 101, 102));
        }

        EXPECT_EQ(cint::instances, 0);
    }

     TYPED_TEST(ContainerTest, BasicOperations) {
        {
            TypeParam v;
            for (int i = 0; i < 10; ++i) {
                v.push_back(typename TypeParam::value_type(i));
            }

            EXPECT_EQ(v.size(), 10);
            EXPECT_THAT(v,
                        ::testing::ElementsAre(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));

            v.erase(0);
            EXPECT_EQ(v.size(), 9);
            EXPECT_THAT(v, ::testing::ElementsAre(1, 2, 3, 4, 5, 6, 7, 8, 9));

            v.erase(8);
            EXPECT_EQ(v.size(), 8);
            EXPECT_THAT(v, ::testing::ElementsAre(1, 2, 3, 4, 5, 6, 7, 8));

            v.erase(2);
            EXPECT_EQ(v.size(), 7);
            EXPECT_THAT(v, ::testing::ElementsAre(1, 2, 4, 5, 6, 7, 8));

            v.insert(0, typename TypeParam::value_type(10));
            EXPECT_EQ(v.size(), 8);
            EXPECT_THAT(v, ::testing::ElementsAre(10, 1, 2, 4, 5, 6, 7, 8));

            v.insert(4, typename TypeParam::value_type(20));
            EXPECT_EQ(v.size(), 9);
            EXPECT_THAT(v, ::testing::ElementsAre(10, 1, 2, 4, 20, 5, 6, 7, 8));

            v.insert(v.size(), typename TypeParam::value_type(30));
            EXPECT_EQ(v.size(), 10);
            EXPECT_THAT(
                v, ::testing::ElementsAre(10, 1, 2, 4, 20, 5, 6, 7, 8, 30));

            v.empty();
            EXPECT_EQ(v.size(), 0);
            EXPECT_EQ(v.begin(), v.end());
            EXPECT_THAT(v, ::testing::ElementsAre());
        }

        EXPECT_EQ(cint::instances, 0);
    }

    TYPED_TEST(ContainerTest, Copy) {
        {
            TypeParam v;
            v.push_back(typename TypeParam::value_type(10));

            auto v2 = v;
            EXPECT_THAT(v, ::testing::ElementsAre(10));
            EXPECT_THAT(v2, ::testing::ContainerEq(v));

            v2.push_back(typename TypeParam::value_type(20));
            EXPECT_THAT(v, ::testing::ElementsAre(10));
            EXPECT_THAT(v2, ::testing::ElementsAre(10, 20));

            TypeParam v3;
            v3 = v2;
            EXPECT_THAT(v, ::testing::ElementsAre(10));
            EXPECT_THAT(v2, ::testing::ElementsAre(10, 20));
            EXPECT_THAT(v3, ::testing::ContainerEq(v2));

            v3.empty();
            EXPECT_THAT(v, ::testing::ElementsAre(10));
            EXPECT_THAT(v2, ::testing::ElementsAre(10, 20));
            EXPECT_EQ(v3.size(), 0);
        }

        EXPECT_EQ(cint::instances, 0);
    }

    TYPED_TEST(ContainerTest, Move) {
        {
            TypeParam v;
            v.push_back(typename TypeParam::value_type(10));

            auto v2 = std::move(v);
            EXPECT_EQ(v.size(), 0);
            EXPECT_EQ(v2.size(), 1);
            EXPECT_THAT(v, ::testing::ElementsAre());
            EXPECT_THAT(v2, ::testing::ElementsAre(10));

            v2.push_back(typename TypeParam::value_type(20));
            EXPECT_EQ(v.size(), 0);
            EXPECT_EQ(v2.size(), 2);
            EXPECT_THAT(v, ::testing::ElementsAre());
            EXPECT_THAT(v2, ::testing::ElementsAre(10, 20));

            TypeParam v3;
            v3 = std::move(v2);
            EXPECT_EQ(v.size(), 0);
            EXPECT_EQ(v2.size(), 0);
            EXPECT_EQ(v3.size(), 2);

            v3.push_back(typename TypeParam::value_type(30));
            EXPECT_EQ(v.size(), 0);
            EXPECT_EQ(v2.size(), 0);
            EXPECT_EQ(v3.size(), 3);
            EXPECT_THAT(v, ::testing::ElementsAre());
            EXPECT_THAT(v2, ::testing::ElementsAre());
            EXPECT_THAT(v3, ::testing::ElementsAre(10, 20, 30));
        }

        EXPECT_EQ(cint::instances, 0);
    }

    TYPED_TEST(ContainerTest, Iterators) {
        {
            TypeParam v;
            for (int i = 0; i < 4; ++i) {
                v.push_back(typename TypeParam::value_type(i));
            }

            auto vi = v.begin();
            EXPECT_EQ(*vi, 0);

            ++vi;
            EXPECT_EQ(*vi, 1);
            EXPECT_NE(vi, v.begin());
            EXPECT_NE(vi, v.end());

            ++vi;
            EXPECT_EQ(*vi, 2);

            ++vi;
            EXPECT_EQ(*vi, 3);

            ++vi;
            EXPECT_EQ(vi, v.end());
            EXPECT_NE(vi, v.begin());

            EXPECT_THROW({ *vi; }, std::out_of_range);

            EXPECT_THROW({ ++vi; }, std::out_of_range);

            EXPECT_EQ(vi, v.end());

            --vi;
            EXPECT_EQ(*vi, 3);
            EXPECT_NE(vi, v.end());

            --vi;
            EXPECT_EQ(*vi, 2);

            --vi;
            EXPECT_EQ(*vi, 1);

            --vi;
            EXPECT_EQ(*vi, 0);
            EXPECT_EQ(vi, v.begin());

            EXPECT_THROW({ --vi; }, std::out_of_range);

            EXPECT_EQ(*vi, 0);
            EXPECT_EQ(vi, v.begin());
        }

        EXPECT_EQ(cint::instances, 0);
    }

} // namespace
