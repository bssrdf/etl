//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "test.hpp"
#include "etl/stop.hpp"

#include "mmul_test.hpp"

// Matrix Matrix multiplication tests

GEMM_TN_TEST_CASE("gemm_tn/1", "[gemm_tn]") {
    etl::fast_matrix<T, 2, 3> aa = {1, 2, 3, 4, 5, 6};
    etl::fast_matrix<T, 3, 2> a;
    etl::fast_matrix<T, 3, 2> b = {7, 8, 9, 10, 11, 12};
    etl::fast_matrix<T, 2, 2> c;

    a = transpose(aa);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 58);
    REQUIRE_EQUALS(c(0, 1), 64);
    REQUIRE_EQUALS(c(1, 0), 139);
    REQUIRE_EQUALS(c(1, 1), 154);
}

GEMM_TN_TEST_CASE("gemm_tn/2", "[gemm_tn]") {
    etl::fast_matrix<T, 3, 3> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    etl::fast_matrix<T, 3, 3> b = {7, 8, 9, 9, 10, 11, 11, 12, 13};
    etl::fast_matrix<T, 3, 3> c;

    a = transpose(a);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 58);
    REQUIRE_EQUALS(c(0, 1), 64);
    REQUIRE_EQUALS(c(0, 2), 70);
    REQUIRE_EQUALS(c(1, 0), 139);
    REQUIRE_EQUALS(c(1, 1), 154);
    REQUIRE_EQUALS(c(1, 2), 169);
    REQUIRE_EQUALS(c(2, 0), 220);
    REQUIRE_EQUALS(c(2, 1), 244);
    REQUIRE_EQUALS(c(2, 2), 268);
}

GEMM_TN_TEST_CASE("gemm_tn/3", "[gemm_tn]") {
    etl::dyn_matrix<T> a(4, 4, etl::values(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
    etl::dyn_matrix<T> b(4, 4, etl::values(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
    etl::dyn_matrix<T> c(4, 4);

    a = transpose(a);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 90);
    REQUIRE_EQUALS(c(0, 1), 100);
    REQUIRE_EQUALS(c(1, 0), 202);
    REQUIRE_EQUALS(c(1, 1), 228);
    REQUIRE_EQUALS(c(2, 0), 314);
    REQUIRE_EQUALS(c(2, 1), 356);
    REQUIRE_EQUALS(c(3, 0), 426);
    REQUIRE_EQUALS(c(3, 1), 484);
}

GEMM_TN_TEST_CASE("gemm_tn/4", "[gemm_tn]") {
    etl::dyn_matrix<T> a(2, 2, etl::values(1, 2, 3, 4));
    etl::dyn_matrix<T> b(2, 2, etl::values(1, 2, 3, 4));
    etl::dyn_matrix<T> c(2, 2);

    a = transpose(a);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 7);
    REQUIRE_EQUALS(c(0, 1), 10);
    REQUIRE_EQUALS(c(1, 0), 15);
    REQUIRE_EQUALS(c(1, 1), 22);
}

GEMM_TN_TEST_CASE("gemm_tn/5", "[gemm_tn]") {
    etl::dyn_matrix<T> a(3, 3, std::initializer_list<T>({1, 2, 3, 4, 5, 6, 7, 8, 9}));
    etl::dyn_matrix<T> b(3, 3, std::initializer_list<T>({7, 8, 9, 9, 10, 11, 11, 12, 13}));
    etl::dyn_matrix<T> c(3, 3);

    a = transpose(a);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 58);
    REQUIRE_EQUALS(c(0, 1), 64);
    REQUIRE_EQUALS(c(0, 2), 70);
    REQUIRE_EQUALS(c(1, 0), 139);
    REQUIRE_EQUALS(c(1, 1), 154);
    REQUIRE_EQUALS(c(1, 2), 169);
    REQUIRE_EQUALS(c(2, 0), 220);
    REQUIRE_EQUALS(c(2, 1), 244);
    REQUIRE_EQUALS(c(2, 2), 268);
}

GEMM_TN_TEST_CASE("gemm_tn/6", "[gemm_tn]") {
    etl::fast_matrix<T, 19, 19> a(etl::magic(19));
    etl::fast_matrix<T, 19, 19> b(etl::magic(19));
    etl::fast_matrix<T, 19, 19> c;

    a = transpose(a);

    Impl::apply(a, b, c);

    REQUIRE_EQUALS(c(0, 0), 828343);
    REQUIRE_EQUALS(c(1, 1), 825360);
    REQUIRE_EQUALS(c(2, 2), 826253);
    REQUIRE_EQUALS(c(3, 3), 824524);
    REQUIRE_EQUALS(c(18, 18), 828343);
}
