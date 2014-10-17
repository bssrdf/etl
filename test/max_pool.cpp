//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "catch.hpp"

#include "etl/etl.hpp"

//{{{ p_max_pool

TEST_CASE( "p_max_pool_1", "p_max_pool_2d" ) {
    etl::fast_matrix<double, 4, 4> a({1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5, 1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5});
    etl::fast_matrix<double, 4, 4> b(etl::p_max_pool<2, 2>(a));

    REQUIRE(b(0, 0) == Approx(0.52059));
    REQUIRE(b(0, 1) == Approx(0.02591));
    REQUIRE(b(0, 2) == Approx(0.01085));
    REQUIRE(b(0, 3) == Approx(0.00089));
    REQUIRE(b(1, 0) == Approx(0.19151));
    REQUIRE(b(1, 1) == Approx(0.07045));
    REQUIRE(b(1, 2) == Approx(0.01085));
    REQUIRE(b(1, 3) == Approx(0.97686));
    REQUIRE(b(2, 0) == Approx(0.52059));
    REQUIRE(b(2, 1) == Approx(0.02591));
    REQUIRE(b(2, 2) == Approx(0.01085));
    REQUIRE(b(2, 3) == Approx(0.00089));
}

TEST_CASE( "p_max_pool_2", "p_max_pool_3d" ) {
    etl::fast_matrix<double, 2, 4, 4> a({1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5, 1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5, 1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5, 1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5});
    etl::fast_matrix<double, 2, 4, 4> b(etl::p_max_pool<2, 2>(a));

    REQUIRE(b(0, 0, 0) == Approx(0.52059));
    REQUIRE(b(0, 0, 1) == Approx(0.02591));
    REQUIRE(b(0, 0, 2) == Approx(0.01085));
    REQUIRE(b(0, 0, 3) == Approx(0.00089));
    REQUIRE(b(0, 1, 0) == Approx(0.19151));
    REQUIRE(b(0, 1, 1) == Approx(0.07045));
    REQUIRE(b(0, 1, 2) == Approx(0.01085));
    REQUIRE(b(0, 1, 3) == Approx(0.97686));
    REQUIRE(b(0, 2, 0) == Approx(0.52059));
    REQUIRE(b(0, 2, 1) == Approx(0.02591));
    REQUIRE(b(0, 2, 2) == Approx(0.01085));
    REQUIRE(b(0, 2, 3) == Approx(0.00089));
    REQUIRE(b(1, 0, 0) == Approx(0.52059));
    REQUIRE(b(1, 0, 1) == Approx(0.02591));
    REQUIRE(b(1, 0, 2) == Approx(0.01085));
    REQUIRE(b(1, 0, 3) == Approx(0.00089));
    REQUIRE(b(1, 1, 0) == Approx(0.19151));
    REQUIRE(b(1, 1, 1) == Approx(0.07045));
    REQUIRE(b(1, 1, 2) == Approx(0.01085));
    REQUIRE(b(1, 1, 3) == Approx(0.97686));
    REQUIRE(b(1, 2, 0) == Approx(0.52059));
    REQUIRE(b(1, 2, 1) == Approx(0.02591));
    REQUIRE(b(1, 2, 2) == Approx(0.01085));
    REQUIRE(b(1, 2, 3) == Approx(0.00089));
}

TEST_CASE( "p_max_pool_3", "p_max_pool_2d" ) {
    etl::fast_matrix<double, 4, 4> a({1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5, 1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5});
    etl::fast_matrix<double, 4, 4> b;

    b = etl::p_max_pool<2, 2>(a + 1.0);

    CHECK(b(0, 0) == Approx(0.59229));
    CHECK(b(0, 1) == Approx(0.02948));
    CHECK(b(0, 2) == Approx(0.01085));
    CHECK(b(0, 3) == Approx(0.00089));
    CHECK(b(1, 0) == Approx(0.21789));
    CHECK(b(1, 1) == Approx(0.08015));
    CHECK(b(1, 2) == Approx(0.01085));
    CHECK(b(1, 3) == Approx(0.97719));
    CHECK(b(2, 0) == Approx(0.59229));
    CHECK(b(2, 1) == Approx(0.02948));
    CHECK(b(2, 2) == Approx(0.01085));
    CHECK(b(2, 3) == Approx(0.00089));
}

TEST_CASE( "p_max_pool_4", "p_max_pool_3d" ) {
    etl::fast_matrix<double, 2, 4, 4> a({1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5, 1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5, 1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5, 1.0, -2.0, 3.0, 0.5, 0.0, -1, 3.0, 7.5});
    etl::fast_matrix<double, 2, 4, 4> b;

    b = etl::p_max_pool<2, 2>(a + 1.0);

    CHECK(b(0, 0, 0) == Approx(0.59229));
    CHECK(b(0, 0, 1) == Approx(0.02948));
    CHECK(b(0, 0, 2) == Approx(0.01085));
    CHECK(b(0, 0, 3) == Approx(0.00089));
    CHECK(b(0, 1, 0) == Approx(0.21789));
    CHECK(b(0, 1, 1) == Approx(0.08015));
    CHECK(b(0, 1, 2) == Approx(0.01085));
    CHECK(b(0, 1, 3) == Approx(0.97719));
    CHECK(b(0, 2, 0) == Approx(0.59229));
    CHECK(b(0, 2, 1) == Approx(0.02948));
    CHECK(b(0, 2, 2) == Approx(0.01085));
    CHECK(b(0, 2, 3) == Approx(0.00089));
    CHECK(b(1, 0, 0) == Approx(0.59229));
    CHECK(b(1, 0, 1) == Approx(0.02948));
    CHECK(b(1, 0, 2) == Approx(0.01085));
    CHECK(b(1, 0, 3) == Approx(0.00089));
    CHECK(b(1, 1, 0) == Approx(0.21789));
    CHECK(b(1, 1, 1) == Approx(0.08015));
    CHECK(b(1, 1, 2) == Approx(0.01085));
    CHECK(b(1, 1, 3) == Approx(0.97719));
    CHECK(b(1, 2, 0) == Approx(0.59229));
    CHECK(b(1, 2, 1) == Approx(0.02948));
    CHECK(b(1, 2, 2) == Approx(0.01085));
    CHECK(b(1, 2, 3) == Approx(0.00089));
}

//}}}