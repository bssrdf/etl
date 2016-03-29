//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include "test_light.hpp"
#include "cpp_utils/algorithm.hpp"

#define CZ(a, b) std::complex<Z>(a, b)
#define ECZ(a, b) etl::complex<Z>(a, b)

TEMPLATE_TEST_CASE_2("sparse/complex/init/1", "[mat][init][sparse]", Z, double, float) {
    etl::sparse_matrix<std::complex<Z>> a(3, 2, std::initializer_list<std::complex<Z>>({CZ(1.0, 0.0), CZ(0.0, 0.0), CZ(0.0, 0.0), CZ(0.0, 0.0), CZ(-1.0, 2.0), CZ(0.0, 1.0)}));

    REQUIRE(a.rows() == 3);
    REQUIRE(a.columns() == 2);
    REQUIRE(a.size() == 6);
    REQUIRE(a.non_zeros() == 3);

    REQUIRE(a.get(0, 0).real() == Z(1.0));
    REQUIRE(a.get(0, 0).imag() == Z(0.0));
    REQUIRE(a.get(0, 1).real() == Z(0.0));
    REQUIRE(a.get(0, 1).imag() == Z(0.0));
    REQUIRE(a.get(1, 0).real() == Z(0.0));
    REQUIRE(a.get(1, 0).imag() == Z(0.0));
    REQUIRE(a.get(1, 1).real() == Z(0.0));
    REQUIRE(a.get(1, 1).imag() == Z(0.0));
    REQUIRE(a.get(2, 0).real() == Z(-1.0));
    REQUIRE(a.get(2, 0).imag() == Z(2.0));
    REQUIRE(a.get(2, 1).real() == Z(0.0));
    REQUIRE(a.get(2, 1).imag() == Z(1.0));
}

TEMPLATE_TEST_CASE_2("sparse/complex/set/1", "[mat][set][sparse]", Z, double, float) {
    etl::sparse_matrix<std::complex<Z>> a(3, 3);

    REQUIRE(a.rows() == 3);
    REQUIRE(a.columns() == 3);
    REQUIRE(a.size() == 9);
    REQUIRE(a.non_zeros() == 0);

    a.set(1, 1, CZ(42.0, 0.0));

    REQUIRE(a.get(1, 1) == CZ(42.0, 0.0));
    REQUIRE(a.non_zeros() == 1);

    a.set(2, 2, CZ(1.0, 0.0));
    a.set(0, 0, CZ(2.0, 0.0));

    REQUIRE(a.get(0, 0) == CZ(2.0, 0.0));
    REQUIRE(a.get(1, 1) == CZ(42.0, 0.0));
    REQUIRE(a.get(2, 2) == CZ(1.0, 0.0));
    REQUIRE(a.non_zeros() == 3);

    a.set(2, 2, CZ(0.0, -2.0));

    REQUIRE(a.get(0, 0) == CZ(2.0, 0.0));
    REQUIRE(a.get(1, 1) == CZ(42.0, 0.0));
    REQUIRE(a.get(2, 2) == CZ(0.0, -2.0));
    REQUIRE(a.non_zeros() == 3);
}

TEMPLATE_TEST_CASE_2("sparse/complex/set/2", "[mat][set][sparse]", Z, double, float) {
    etl::sparse_matrix<std::complex<Z>> a(3, 3);

    REQUIRE(a.rows() == 3);
    REQUIRE(a.columns() == 3);
    REQUIRE(a.size() == 9);
    REQUIRE(a.non_zeros() == 0);

    a.set(0, 0, CZ(1.0, 0.0));
    a.set(1, 1, CZ(0.0, 42));
    a.set(2, 2, CZ(2.2, 3.2));

    REQUIRE(a.get(0, 0) == CZ(1.0, 0.0));
    REQUIRE(a.get(0, 1) == CZ(0.0, 0.0));
    REQUIRE(a.get(1, 1) == CZ(0.0, 42.0));
    REQUIRE(a.get(2, 2) == CZ(2.2, 3.2));
    REQUIRE(a.non_zeros() == 3);

    a.set(0, 0, CZ(0.0, 0.0));

    REQUIRE(a.get(0, 0) == CZ(0.0, 0.0));
    REQUIRE(a.get(0, 1) == CZ(0.0, 0.0));
    REQUIRE(a.get(1, 1) == CZ(0.0, 42.0));
    REQUIRE(a.get(2, 2) == CZ(2.2, 3.2));
    REQUIRE(a.non_zeros() == 2);
}

TEMPLATE_TEST_CASE_2("sparse/complex/reference/1", "[mat][reference][sparse]", Z, double, float) {
    etl::sparse_matrix<std::complex<Z>> a(3, 3);

    REQUIRE(a.rows() == 3);
    REQUIRE(a.columns() == 3);
    REQUIRE(a.size() == 9);
    REQUIRE(a.non_zeros() == 0);

    a(1, 1) = CZ(1.0, 2.0);

    REQUIRE(a.get(1, 1) == CZ(1.0, 2.0));
    REQUIRE(a.non_zeros() == 1);

    a(0, 0) = CZ(1.0, 1.0);
    a(2, 2) = CZ(2.0, 2.0);

    REQUIRE(a.get(0, 0) == CZ(1.0, 1.0));
    REQUIRE(a.get(1, 1) == CZ(1.0, 2.0));
    REQUIRE(a.get(2, 2) == CZ(2.0, 2.0));
    REQUIRE(a.non_zeros() == 3);

    a(2, 2) = CZ(-2.0, 2.0);

    REQUIRE(a.get(0, 0) == CZ(1.0, 1.0));
    REQUIRE(a.get(1, 1) == CZ(1.0, 2.0));
    REQUIRE(a.get(2, 2) == CZ(-2.0, 2.0));
    REQUIRE(a.non_zeros() == 3);
}

TEMPLATE_TEST_CASE_2("sparse/complex/reference/2", "[mat][reference][sparse]", Z, double, float) {
    etl::sparse_matrix<std::complex<Z>> a(3, 3);

    REQUIRE(a.rows() == 3);
    REQUIRE(a.columns() == 3);
    REQUIRE(a.size() == 9);
    REQUIRE(a.non_zeros() == 0);

    a(0, 0) = CZ(1.0, 1.01);
    a(1, 1) = CZ(42.0, 32.0);
    a(2, 2) = CZ(2.0, 1.0);

    REQUIRE(a.get(0, 0) == CZ(1.0, 1.01));
    REQUIRE(a.get(0, 1) == CZ(0.0, 0.0));
    REQUIRE(a.get(1, 1) == CZ(42, 32.0));
    REQUIRE(a.get(2, 2) == CZ(2.0, 1.0));
    REQUIRE(a.non_zeros() == 3);

    a(0, 0) = 0.0;

    REQUIRE(a.get(0, 0) == CZ(0.0, 0.0));
    REQUIRE(a.get(0, 1) == CZ(0.0, 0.0));
    REQUIRE(a.get(1, 1) == CZ(42, 32.0));
    REQUIRE(a.get(2, 2) == CZ(2.0, 1.0));
    REQUIRE(a.non_zeros() == 2);

    a(2, 2) = 0.0;

    REQUIRE(a.get(0, 0) == CZ(0.0, 0.0));
    REQUIRE(a.get(0, 1) == CZ(0.0, 0.0));
    REQUIRE(a.get(1, 1) == CZ(42, 32.0));
    REQUIRE(a.get(2, 2) == CZ(0.0, 0.0));
    REQUIRE(a.non_zeros() == 1);
}

TEMPLATE_TEST_CASE_2("sparse/complex/erase/1", "[mat][erase][sparse]", Z, double, float) {
    etl::sparse_matrix<std::complex<Z>> a(3, 2, std::initializer_list<std::complex<Z>>({CZ(1.0, 0.0), CZ(0.0, 0.0), CZ(0.0, 0.0), CZ(0.0, 0.0), CZ(-1.0, 2.0), CZ(0.0, 1.0)}));

    REQUIRE(a.non_zeros() == 3);

    a.erase(0, 0);

    REQUIRE(a.get(0, 0) == CZ(0.0, 0.0));
    REQUIRE(a.get(0, 1) == CZ(0.0, 0.0));
    REQUIRE(a.get(2, 0) == CZ(-1.0, 2.0));
    REQUIRE(a.non_zeros() == 2);

    a.erase(0, 0);

    REQUIRE(a.get(0, 0) == CZ(0.0, 0.0));
    REQUIRE(a.get(0, 1) == CZ(0.0, 0.0));
    REQUIRE(a.get(2, 0) == CZ(-1.0, 2.0));
    REQUIRE(a.non_zeros() == 2);

    a.erase(2, 0);
    a.erase(2, 1);

    REQUIRE(a.get(0, 0) == CZ(0.0, 0.0));
    REQUIRE(a.get(0, 1) == CZ(0.0, 0.0));
    REQUIRE(a.get(2, 0) == CZ(0.0, 0.0));
    REQUIRE(a.non_zeros() == 0);

    a.set(2, 0, CZ(3.0, 3.3));

    REQUIRE(a.get(2, 0) == CZ(3.0, 3.3));
    REQUIRE(a.non_zeros() == 1);
}

TEMPLATE_TEST_CASE_2("sparse/complex/add/1", "[mat][add][sparse]", Z, double, float) {
    etl::sparse_matrix<std::complex<Z>> a(2, 2, std::initializer_list<std::complex<Z>>({CZ(1.0, 0.0), CZ(0.0, 0.0), CZ(0.0, 0.0), CZ(2.0, 0.0)}));
    etl::sparse_matrix<std::complex<Z>> b(2, 2, std::initializer_list<std::complex<Z>>({CZ(2.0, -2.0), CZ(1.0, 2.0), CZ(0.0, 0.0), CZ(2.0, 0.0)}));
    etl::sparse_matrix<std::complex<Z>> c(2, 2);

    c = a + b;

    REQUIRE(c.get(0, 0) == CZ(3.0, -2.0));
    REQUIRE(c.get(0, 1) == CZ(1.0, 2.0));
    REQUIRE(c.get(1, 0) == CZ(0.0, 0.0));
    REQUIRE(c.get(1, 1) == CZ(4.0, 0.0));
    REQUIRE(c.non_zeros() == 3);
}

TEMPLATE_TEST_CASE_2("sparse/complex/sub/1", "[mat][add][sparse]", Z, double, float) {
    etl::sparse_matrix<std::complex<Z>> a(2, 2, std::initializer_list<std::complex<Z>>({CZ(1.0, 0.0), CZ(0.0, 0.0), CZ(0.0, 0.0), CZ(2.0, 0.0)}));
    etl::sparse_matrix<std::complex<Z>> b(2, 2, std::initializer_list<std::complex<Z>>({CZ(2.0, -2.0), CZ(1.0, 2.0), CZ(0.0, 0.0), CZ(2.0, 0.0)}));
    etl::sparse_matrix<std::complex<Z>> c(2, 2);

    c = a - b;

    REQUIRE(c.get(0, 0) == CZ(-1.0, 2.0));
    REQUIRE(c.get(0, 1) == CZ(-1.0, -2.0));
    REQUIRE(c.get(1, 0) == CZ(0.0, 0.0));
    REQUIRE(c.get(1, 1) == CZ(0.0, 0.0));
    REQUIRE(c.non_zeros() == 2);
}

TEMPLATE_TEST_CASE_2("sparse/complex/mul/1", "[mat][add][sparse]", Z, double, float) {
    etl::sparse_matrix<std::complex<Z>> a(2, 2, std::initializer_list<std::complex<Z>>({CZ(1.0, 0.0), CZ(0.0, 0.0), CZ(0.0, 0.0), CZ(2.0, 0.0)}));
    etl::sparse_matrix<std::complex<Z>> b(2, 2, std::initializer_list<std::complex<Z>>({CZ(2.0, -2.0), CZ(1.0, 2.0), CZ(0.0, 0.0), CZ(2.0, 0.0)}));
    etl::sparse_matrix<std::complex<Z>> c(2, 2);

    c = a >> b;

    REQUIRE(c.get(0, 0) == CZ(2.0, -2.0));
    REQUIRE(c.get(0, 1) == CZ(0.0, 0.0));
    REQUIRE(c.get(1, 0) == CZ(0.0, 0.0));
    REQUIRE(c.get(1, 1) == CZ(4.0, 0.0));
    REQUIRE(c.non_zeros() == 2);
}

TEMPLATE_TEST_CASE_2("sparse/complex/div/1", "[mat][div][sparse]", Z, double, float) {
    etl::sparse_matrix<std::complex<Z>> a(2, 2, std::initializer_list<std::complex<Z>>({CZ(1.0, 0.0), CZ(0.0, 0.0), CZ(0.0, 0.0), CZ(2.0, 0.0)}));
    etl::sparse_matrix<std::complex<Z>> b(2, 2, std::initializer_list<std::complex<Z>>({CZ(2.0, -2.0), CZ(1.0, 2.0), CZ(1.0, 0.0), CZ(2.0, 0.0)}));
    etl::sparse_matrix<std::complex<Z>> c(2, 2);

    c = a / b;

    REQUIRE(c.get(0, 0) == CZ(0.25, 0.25));
    REQUIRE(c.get(0, 1) == CZ(0.0, 0.0));
    REQUIRE(c.get(1, 0) == CZ(0.0, 0.0));
    REQUIRE(c.get(1, 1) == CZ(1.0, 0.0));
    REQUIRE(c.non_zeros() == 2);
}
