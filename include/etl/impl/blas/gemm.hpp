//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

#ifdef ETL_BLAS_MODE

#include "etl/impl/common/conv.hpp"

extern "C" {
#include "cblas.h"
}

#endif
namespace etl {

namespace impl {

namespace blas {

#ifdef ETL_BLAS_MODE

// GEMM overloads

/*!
 * \brief Compute the Matrix-Matrix multiplication.
 * \param Layout The memory layout
 * \param TransA The operation on A
 * \param TransB The operation on B
 * \param M The first dimension of A
 * \param N The second dimension of B
 * \param K The second dimension of A
 * \param alpha The multiplicator on op(A) * op(B)
 * \param A The A matrix
 * \param lda The leading dimension of A
 * \param B The B matrix
 * \param ldb The leading dimension of B
 * \param beta The multiplicator on C
 * \param C The C matrix
 * \param ldc The leading dimension of C
 */
inline void cblas_gemm(CBLAS_LAYOUT Layout, CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, size_t M, size_t N, size_t K,
        const float alpha, const float* A, size_t lda, const float* B, size_t ldb, const float beta, float* C, size_t ldc) {
    cblas_sgemm(Layout, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
}

/*!
 * \copydoc cblas_gemm
 */
inline void cblas_gemm(CBLAS_LAYOUT Layout, CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, size_t M, size_t N, size_t K,
        const double alpha, const double* A, size_t lda, const double* B, size_t ldb, const double beta, double* C, size_t ldc) {
    cblas_dgemm(Layout, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
}

/*!
 * \copydoc cblas_gemm
 */
inline void cblas_gemm(CBLAS_LAYOUT Layout, CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, size_t M, size_t N, size_t K,
        const etl::complex<float> alpha, const etl::complex<float>* A, size_t lda, const etl::complex<float>* B, size_t ldb, const etl::complex<float> beta, etl::complex<float>* C, size_t ldc) {
    cblas_cgemm(Layout, TransA, TransB, M, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

/*!
 * \copydoc cblas_gemm
 */
inline void cblas_gemm(CBLAS_LAYOUT Layout, CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, size_t M, size_t N, size_t K,
        const etl::complex<double> alpha, const etl::complex<double>* A, size_t lda, const etl::complex<double>* B, size_t ldb, const etl::complex<double> beta, etl::complex<double>* C, size_t ldc) {
    cblas_zgemm(Layout, TransA, TransB, M, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

/*!
 * \copydoc cblas_gemm
 */
inline void cblas_gemm(CBLAS_LAYOUT Layout, CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, size_t M, size_t N, size_t K,
        const std::complex<float> alpha, const std::complex<float>* A, size_t lda, const std::complex<float>* B, size_t ldb, const std::complex<float> beta, std::complex<float>* C, size_t ldc) {
    cblas_cgemm(Layout, TransA, TransB, M, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

/*!
 * \copydoc cblas_gemm
 */
inline void cblas_gemm(CBLAS_LAYOUT Layout, CBLAS_TRANSPOSE TransA, CBLAS_TRANSPOSE TransB, size_t M, size_t N, size_t K,
        const std::complex<double> alpha, const std::complex<double>* A, size_t lda, const std::complex<double>* B, size_t ldb, const std::complex<double> beta, std::complex<double>* C, size_t ldc) {
    cblas_zgemm(Layout, TransA, TransB, M, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
}

// GEMV overloads

/*!
 * \brief Compute the Matrix-Vector multiplication.
 * \param Layout The memory layout
 * \param TransA The operation on A
 * \param M The first dimension of A
 * \param N The second dimension of A
 * \param alpha The multiplicator on op(A) * B
 * \param A The A matrix
 * \param lda The leading dimension of A
 * \param X The X vector
 * \param incX The stride of X
 * \param beta The multiplicator on C
 * \param Y The Y vector
 * \param incY The stride of Y
 */
inline void cblas_gemv(const CBLAS_LAYOUT Layout, const CBLAS_TRANSPOSE TransA, size_t M, size_t N, const float alpha, const float *A, size_t lda,
        const float *X, size_t incX, const float beta, float *Y, size_t incY){
    cblas_sgemv(Layout, TransA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
}

/*!
 * \copydoc cblas_gemv
 */
inline void cblas_gemv(const CBLAS_LAYOUT Layout, const CBLAS_TRANSPOSE TransA, size_t M, size_t N, const double alpha, const double *A, size_t lda,
        const double *X, size_t incX, const double beta, double *Y, size_t incY){
    cblas_dgemv(Layout, TransA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
}

/*!
 * \copydoc cblas_gemv
 */
inline void cblas_gemv(const CBLAS_LAYOUT Layout, const CBLAS_TRANSPOSE TransA, size_t M, size_t N, const std::complex<float> alpha, const std::complex<float> *A, size_t lda,
        const std::complex<float> *X, size_t incX, const std::complex<float> beta, std::complex<float> *Y, size_t incY){
    cblas_cgemv(Layout, TransA, M, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

/*!
 * \copydoc cblas_gemv
 */
inline void cblas_gemv(const CBLAS_LAYOUT Layout, const CBLAS_TRANSPOSE TransA, size_t M, size_t N, const std::complex<double> alpha, const std::complex<double> *A, size_t lda,
        const std::complex<double> *X, size_t incX, const std::complex<double> beta, std::complex<double> *Y, size_t incY){
    cblas_zgemv(Layout, TransA, M, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

/*!
 * \copydoc cblas_gemv
 */
inline void cblas_gemv(const CBLAS_LAYOUT Layout, const CBLAS_TRANSPOSE TransA, size_t M, size_t N, const etl::complex<float> alpha, const etl::complex<float> *A, size_t lda,
        const etl::complex<float> *X, size_t incX, const etl::complex<float> beta, etl::complex<float> *Y, size_t incY){
    cblas_cgemv(Layout, TransA, M, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

/*!
 * \copydoc cblas_gemv
 */
inline void cblas_gemv(const CBLAS_LAYOUT Layout, const CBLAS_TRANSPOSE TransA, size_t M, size_t N, const etl::complex<double> alpha, const etl::complex<double> *A, size_t lda,
        const etl::complex<double> *X, size_t incX, const etl::complex<double> beta, etl::complex<double> *Y, size_t incY){
    cblas_zgemv(Layout, TransA, M, N, &alpha, A, lda, X, incX, &beta, Y, incY);
}

/*!
 * \brief Compute the matrix mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gemm(A&& a, B&& b, C&& c) {
    using T = value_t<A>;

    static constexpr bool row_major = decay_traits<A>::storage_order == order::RowMajor;

    T alpha(1.0);
    T beta(0.0);

    a.ensure_cpu_up_to_date();
    b.ensure_cpu_up_to_date();

    cblas_gemm(
        row_major ? CblasRowMajor : CblasColMajor,
        CblasNoTrans, CblasNoTrans,
        etl::rows(a), etl::columns(b), etl::columns(a),
        alpha,
        a.memory_start(), major_stride(a),
        b.memory_start(), major_stride(b),
        beta,
        c.memory_start(), major_stride(c));

    c.invalidate_gpu();
}

/*!
 * \brief Compute the matrix mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gemm_nt(A&& a, B&& b, C&& c) {
    using T = value_t<A>;

    static constexpr bool row_major = decay_traits<A>::storage_order == order::RowMajor;

    T alpha(1.0);
    T beta(0.0);

    a.ensure_cpu_up_to_date();
    b.ensure_cpu_up_to_date();

    cblas_gemm(
        row_major ? CblasRowMajor : CblasColMajor,
        CblasNoTrans, CblasTrans,
        etl::rows(a), etl::rows(b), etl::columns(a),
        alpha,
        a.memory_start(), major_stride(a),
        b.memory_start(), major_stride(b),
        beta,
        c.memory_start(), major_stride(c));

    c.invalidate_gpu();
}

/*!
 * \brief Compute the matrix mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gemm_tn(A&& a, B&& b, C&& c) {
    using T = value_t<A>;

    static constexpr bool row_major = decay_traits<A>::storage_order == order::RowMajor;

    T alpha(1.0);
    T beta(0.0);

    a.ensure_cpu_up_to_date();
    b.ensure_cpu_up_to_date();

    cblas_gemm(
        row_major ? CblasRowMajor : CblasColMajor,
        CblasTrans, CblasNoTrans,
        etl::columns(a), etl::columns(b), etl::rows(a),
        alpha,
        a.memory_start(), major_stride(a),
        b.memory_start(), major_stride(b),
        beta,
        c.memory_start(), major_stride(c));

    c.invalidate_gpu();
}

/*!
 * \brief Compute the matrix mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gemm_tt(A&& a, B&& b, C&& c) {
    using T = value_t<A>;

    static constexpr bool row_major = decay_traits<A>::storage_order == order::RowMajor;

    T alpha(1.0);
    T beta(0.0);

    a.ensure_cpu_up_to_date();
    b.ensure_cpu_up_to_date();

    cblas_gemm(
        row_major ? CblasRowMajor : CblasColMajor,
        CblasTrans, CblasTrans,
        etl::columns(a), etl::rows(b), etl::rows(a),
        alpha,
        a.memory_start(), major_stride(a),
        b.memory_start(), major_stride(b),
        beta,
        c.memory_start(), major_stride(c));

    c.invalidate_gpu();
}

/*!
 * \brief Compute the matrix-vector mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gemv(A&& a, B&& b, C&& c) {
    using T = value_t<A>;

    static constexpr bool row_major = decay_traits<A>::storage_order == order::RowMajor;

    T alpha(1.0);
    T beta(0.0);

    a.ensure_cpu_up_to_date();
    b.ensure_cpu_up_to_date();

    cblas_gemv(
        row_major ? CblasRowMajor : CblasColMajor,
        CblasNoTrans,
        etl::rows(a), etl::columns(a),
        alpha,
        a.memory_start(), major_stride(a),
        b.memory_start(), 1,
        beta,
        c.memory_start(), 1);

    c.invalidate_gpu();
}

/*!
 * \brief Compute the vector-matrix mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gevm(A&& a, B&& b, C&& c) {
    using T = value_t<A>;

    static constexpr bool row_major = decay_traits<B>::storage_order == order::RowMajor;

    T alpha(1.0);
    T beta(0.0);

    a.ensure_cpu_up_to_date();
    b.ensure_cpu_up_to_date();

    cblas_gemv(
        row_major ? CblasRowMajor : CblasColMajor,
        CblasTrans,
        etl::rows(b), etl::columns(b),
        alpha,
        b.memory_start(), major_stride(b),
        a.memory_start(), 1,
        beta,
        c.memory_start(), 1);

    c.invalidate_gpu();
}

/*!
 * \brief BLAS implementation of a 2D 'valid' convolution C = I * K, with multiple kernels
 * \param input The input matrix
 * \param kernels The kernel matrix
 * \param conv The output matrix
 */
template <typename I, typename K_T, typename C>
void blas_conv2_valid_multi(const I& input, const K_T& kernels, C&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    using T = value_t<I>;

    const size_t K  = etl::dim<0>(kernels);
    const size_t i1 = etl::dim<0>(input);
    const size_t i2 = etl::dim<1>(input);
    const size_t k1 = etl::dim<1>(kernels);
    const size_t k2 = etl::dim<2>(kernels);

    // unit-strided result dimensions
    const size_t c1 = (i1 - k1 + 2 * p1) + 1;
    const size_t c2 = (i2 - k2 + 2 * p2) + 1;

    // real final dimensions
    const size_t f1 = etl::dim<1>(conv);
    const size_t f2 = etl::dim<2>(conv);

    input.ensure_cpu_up_to_date();
    kernels.ensure_cpu_up_to_date();

    auto prepared_k = force_temporary(kernels);

    // Flip the kernels
    prepared_k.deep_fflip_inplace();

    etl::dyn_matrix<T, 2> input_col(k1 * k2, c1 * c2);

    if(p1 || p2){
        etl::dyn_matrix<T, 2> input_padded(i1 + 2 * p1, i2 + 2 * p2);
        input_padded = T(0);

        impl::common::pad_2d_input(input, input_padded, p1, p2);

        im2col_direct_tr(input_col, input_padded, k1, k2);
    } else {
        im2col_direct_tr(input_col, input, k1, k2);
    }

    if(s1 > 1 || s2 > 1){
        etl::dyn_matrix<T, 3> tmp_result(K, c1, c2);

        // tmp_result = prepared_k * input_col
        cblas_gemm(
            CblasRowMajor,
            CblasNoTrans, CblasNoTrans,
            K, c1 * c2, k1 * k2,
            T(1.0),
            prepared_k.memory_start(), k1 * k2,
            input_col.memory_start(), c1 * c2,
            T(0.0),
            tmp_result.memory_start(), c1 * c2);

        // Strided copy of the large result into the small result
        for (size_t k = 0; k < K; ++k) {
            for (size_t i = 0; i < f1; ++i) {
                for (size_t j = 0; j < f2; ++j) {
                    conv(k, i, j) = tmp_result(k, i * s1, j * s2);
                }
            }
        }
    } else {
        // conv = prepared_k * input_col
        cblas_gemm(
            CblasRowMajor,
            CblasNoTrans, CblasNoTrans,
            K, c1 * c2, k1 * k2,
            T(1.0),
            prepared_k.memory_start(), k1 * k2,
            input_col.memory_start(), c1 * c2,
            T(0.0),
            conv.memory_start(), f1 * f2);
    }

    conv.invalidate_gpu();
}

/*!
 * \brief BLAS implementation of a 2D 'valid' convolution C = I * K, with multiple flipped kernels
 * \param input The input matrix
 * \param kernels The kernel matrix
 * \param conv The output matrix
 */
template <typename I, typename K_T, typename C>
void blas_conv2_valid_multi_flipped(I&& input, K_T&& kernels, C&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    using T = value_t<I>;

    const size_t K  = etl::dim<0>(kernels);
    const size_t i1 = etl::dim<0>(input);
    const size_t i2 = etl::dim<1>(input);
    const size_t k1 = etl::dim<1>(kernels);
    const size_t k2 = etl::dim<2>(kernels);

    // unit-strided result dimensions
    const size_t c1 = (i1 - k1 + 2 * p1) + 1;
    const size_t c2 = (i2 - k2 + 2 * p2) + 1;

    // real final dimensions
    const size_t f1 = etl::dim<1>(conv);
    const size_t f2 = etl::dim<2>(conv);

    input.ensure_cpu_up_to_date();
    kernels.ensure_cpu_up_to_date();

    etl::dyn_matrix<T, 2> input_col(k1 * k2, c1 * c2);

    if(p1 || p2){
        etl::dyn_matrix<T, 2> input_padded(i1 + 2 * p1, i2 + 2 * p2);
        input_padded = T(0);

        impl::common::pad_2d_input(input, input_padded, p1, p2);

        im2col_direct_tr(input_col, input_padded, k1, k2);
    } else {
        im2col_direct_tr(input_col, input, k1, k2);
    }

    if(s1 > 1 || s2 > 1){
        etl::dyn_matrix<T, 3> tmp_result(K, c1, c2);

        // tmp_result = kernels * input_col
        cblas_gemm(
            CblasRowMajor,
            CblasNoTrans, CblasNoTrans,
            K, c1 * c2, k1 * k2,
            T(1.0),
            kernels.memory_start(), k1 * k2,
            input_col.memory_start(), c1 * c2,
            T(0.0),
            tmp_result.memory_start(), c1 * c2);

        // Strided copy of the large result into the small result
        for (size_t k = 0; k < K; ++k) {
            for (size_t i = 0; i < f1; ++i) {
                for (size_t j = 0; j < f2; ++j) {
                    conv(k, i, j) = tmp_result(k, i * s1, j * s2);
                }
            }
        }
    } else {
        // conv = kernels * input_col
        cblas_gemm(
            CblasRowMajor,
            CblasNoTrans, CblasNoTrans,
            K, c1 * c2, k1 * k2,
            T(1.0),
            kernels.memory_start(), k1 * k2,
            input_col.memory_start(), c1 * c2,
            T(0.0),
            conv.memory_start(), f1 * f2);
    }

    conv.invalidate_gpu();
}

/*!
 * \brief BLAS implementation of a 2D 'valid' convolution C = I * K, with multiple images and multiple kernels
 * \param input The input matrix
 * \param kernels The kernel matrix
 * \param conv The output matrix
 */
template <typename I, typename K_T, typename C>
void blas_conv2_valid_multi_multi(const I& input, const K_T& kernels, C&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    using T = value_t<I>;

    const size_t N  = etl::dim<0>(input);
    const size_t i1 = etl::dim<1>(input);
    const size_t i2 = etl::dim<2>(input);

    const size_t K  = etl::dim<0>(kernels);
    const size_t k1 = etl::dim<1>(kernels);
    const size_t k2 = etl::dim<2>(kernels);

    // unit-strided result dimensions
    const size_t c1 = (i1 - k1 + 2 * p1) + 1;
    const size_t c2 = (i2 - k2 + 2 * p2) + 1;

    // real final dimensions
    const size_t f1 = etl::dim<2>(conv);
    const size_t f2 = etl::dim<3>(conv);

    input.ensure_cpu_up_to_date();
    kernels.ensure_cpu_up_to_date();

    auto prepared_k = force_temporary(kernels);

    // Flip the kernels
    prepared_k.deep_fflip_inplace();

    etl::dyn_matrix<T, 2> input_col(k1 * k2, N * c1 * c2);

    if(p1 || p2){
        etl::dyn_matrix<T, 3> input_padded(N, i1 + 2 * p1, i2 + 2 * p2);
        input_padded = T(0);

        for(size_t i = 0; i < N; ++i){
            impl::common::pad_2d_input(input(i), input_padded(i), p1, p2);
        }

        im2col_direct_tr_multi(input_col, input_padded, k1, k2);
    } else {
        im2col_direct_tr_multi(input_col, input, k1, k2);
    }

    if(s1 > 1 || s2 > 1){
        etl::dyn_matrix<T, 4> tmp_result(K, N, c1, c2);

        cblas_gemm(
            CblasRowMajor,
            CblasNoTrans, CblasNoTrans,
            K, N * c1 * c2, k1 * k2,
            T(1.0),
            prepared_k.memory_start(), k1 * k2,
            input_col.memory_start(), N * c1 * c2,
            T(0.0),
            tmp_result.memory_start(), N * c1 * c2);

        // Strided copy of the large result into the small result
        for (size_t k = 0; k < K; ++k) {
            for (size_t i = 0; i < N; ++i) {
                for (size_t ii = 0; ii < f1; ++ii) {
                    for (size_t j = 0; j < f2; ++j) {
                        conv(k, i, ii, j) = tmp_result(k, i, ii * s1, j * s2);
                    }
                }
            }
        }
    } else {
        cblas_gemm(
            CblasRowMajor,
            CblasNoTrans, CblasNoTrans,
            K, N * c1 * c2, k1 * k2,
            T(1.0),
            prepared_k.memory_start(), k1 * k2,
            input_col.memory_start(), N * c1 * c2,
            T(0.0),
            conv.memory_start(), N * f1 * f2);
    }

    conv.invalidate_gpu();
}

/*!
 * \brief BLAS implementation of a 2D 'valid' convolution C = I * K, with multiple images and multiple kernels
 * \param input The input matrix
 * \param kernels The kernel matrix
 * \param conv The output matrix
 */
template <typename I, typename K_T, typename C>
void blas_conv2_valid_multi_multi_flipped(const I& input, const K_T& kernels, C&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    using T = value_t<I>;

    const size_t N  = etl::dim<0>(input);
    const size_t i1 = etl::dim<1>(input);
    const size_t i2 = etl::dim<2>(input);

    const size_t K  = etl::dim<0>(kernels);
    const size_t k1 = etl::dim<1>(kernels);
    const size_t k2 = etl::dim<2>(kernels);

    // unit-strided result dimensions
    const size_t c1 = (i1 - k1 + 2 * p1) + 1;
    const size_t c2 = (i2 - k2 + 2 * p2) + 1;

    // real final dimensions
    const size_t f1 = etl::dim<2>(conv);
    const size_t f2 = etl::dim<3>(conv);

    input.ensure_cpu_up_to_date();
    kernels.ensure_cpu_up_to_date();

    etl::dyn_matrix<T, 2> input_col(k1 * k2, N * c1 * c2);

    if(p1 || p2){
        etl::dyn_matrix<T, 3> input_padded(N, i1 + 2 * p1, i2 + 2 * p2);
        input_padded = T(0);

        for(size_t i = 0; i < N; ++i){
            impl::common::pad_2d_input(input(i), input_padded(i), p1, p2);
        }

        im2col_direct_tr_multi(input_col, input_padded, k1, k2);
    } else {
        im2col_direct_tr_multi(input_col, input, k1, k2);
    }

    if(s1 > 1 || s2 > 1){
        etl::dyn_matrix<T, 4> tmp_result(K, N, c1, c2);

        cblas_gemm(
            CblasRowMajor,
            CblasNoTrans, CblasNoTrans,
            K, N * c1 * c2, k1 * k2,
            T(1.0),
            kernels.memory_start(), k1 * k2,
            input_col.memory_start(), N * c1 * c2,
            T(0.0),
            tmp_result.memory_start(), N * c1 * c2);

        // Strided copy of the large result into the small result
        for (size_t k = 0; k < K; ++k) {
            for (size_t i = 0; i < N; ++i) {
                for (size_t ii = 0; ii < f1; ++ii) {
                    for (size_t j = 0; j < f2; ++j) {
                        conv(k, i, ii, j) = tmp_result(k, i, ii * s1, j * s2);
                    }
                }
            }
        }
    } else {
        cblas_gemm(
            CblasRowMajor,
            CblasNoTrans, CblasNoTrans,
            K, N * c1 * c2, k1 * k2,
            T(1.0),
            kernels.memory_start(), k1 * k2,
            input_col.memory_start(), N * c1 * c2,
            T(0.0),
            conv.memory_start(), N * f1 * f2);
    }

    conv.invalidate_gpu();
}

/*!
 * \brief Compute a 4D valid convolution using a BLAS matrix multiplication kernel
 * \param input The input matrix
 * \param kernel The kernel matrix, with flipped kernels
 * \param conv The output matrix
 * \param s1 The stride of the first dimension
 * \param s2 The stride of the second dimension
 * \param p1 The padding of the first dimension
 * \param p2 The padding of the second dimension
 */
template <typename I_T, typename K_T, typename KS_T, typename C_T>
void blas_conv4_valid_prepared(I_T&& input, K_T&& kernel, KS_T&& kernels, C_T&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    using T = value_t<I_T>;

    const auto N = etl::dim<0>(input);  // The number of images
    const auto K = etl::dim<0>(kernel); // The number of kernels
    const auto C = etl::dim<1>(input);  // The number of channels

    const auto n1 = etl::dim<2>(input);
    const auto n2 = etl::dim<3>(input);

    const auto m1 = etl::dim<2>(kernel);
    const auto m2 = etl::dim<3>(kernel);

    const auto c1 = etl::dim<2>(conv);
    const auto c2 = etl::dim<3>(conv);

    input.ensure_cpu_up_to_date();
    kernels.ensure_cpu_up_to_date();

    conv = T(0.0);

    auto batch_fun_n = [&](const size_t first, const size_t last) {
        if (last - first) {
            SERIAL_SECTION {
                // unit-strided result dimensions
                const size_t sc1 = (n1 - m1 + 2 * p1) + 1;
                const size_t sc2 = (n2 - m2 + 2 * p2) + 1;

                etl::dyn_matrix<T, 2> input_col(m1 * m2, sc1 * sc2);

                // Optimize for the most common case
                if (cpp_likely(!p1 && !p2 && s1 == 1 && s2 == 1)) {
                    for (size_t i = first; i < last; ++i) {
                        for (size_t c = 0; c < C; ++c) {
                            im2col_direct_tr(input_col, input(i)(c), m1, m2);

                            cblas_gemm(
                                CblasRowMajor,
                                CblasNoTrans, CblasNoTrans,
                                K, sc1 * sc2, m1 * m2,
                                T(1.0),
                                kernels(c).memory_start(), m1 * m2,
                                input_col.memory_start(), sc1 * sc2,
                                T(1.0),
                                conv(i).memory_start(), sc1 * sc2);

                        }
                    }
                } else {
                    etl::dyn_matrix<T, 2> input_padded(n1 + 2 * p1, n2 + 2 * p2);
                    etl::dyn_matrix<T, 3> tmp_result(K, sc1, sc2);

                    for (size_t i = first; i < last; ++i) {
                        for (size_t c = 0; c < C; ++c) {
                            if (p1 || p2) {
                                input_padded = T(0.0);

                                impl::common::pad_2d_input(input(i)(c), input_padded, p1, p2);

                                im2col_direct_tr(input_col, input_padded, m1, m2);
                            } else {
                                im2col_direct_tr(input_col, input(i)(c), m1, m2);
                            }

                            if (s1 > 1 || s2 > 1) {
                                cblas_gemm(
                                    CblasRowMajor,
                                    CblasNoTrans, CblasNoTrans,
                                    K, sc1 * sc2, m1 * m2,
                                    T(1.0),
                                    kernels(c).memory_start(), m1 * m2,
                                    input_col.memory_start(), sc1 * sc2,
                                    T(0.0),
                                    tmp_result.memory_start(), sc1 * sc2);

                                // Strided copy of the large result into the small result
                                for (size_t k = 0; k < K; ++k) {
                                    for (size_t ii = 0; ii < c1; ++ii) {
                                        for (size_t j = 0; j < c2; ++j) {
                                            conv(i, k, ii, j) += tmp_result(k, ii * s1, j * s2);
                                        }
                                    }
                                }
                            } else {
                                cblas_gemm(
                                    CblasRowMajor,
                                    CblasNoTrans, CblasNoTrans,
                                    K, c1 * c2, m1 * m2,
                                    T(1.0),
                                    kernels(c).memory_start(), m1 * m2,
                                    input_col.memory_start(), sc1 * sc2,
                                    T(1.0),
                                    conv(i).memory_start(), c1 * c2);
                            }
                        }
                    }
                }
            }
        }
    };

    dispatch_1d_any(select_parallel(N, 2), batch_fun_n, 0, N);

    conv.invalidate_gpu();
}

/*!
 * \brief Compute a 4D valid convolution using a BLAS matrix multiplication kernel
 * \param input The input matrix
 * \param kernel The kernel matrix, with flipped kernels
 * \param conv The output matrix
 * \param s1 The stride of the first dimension
 * \param s2 The stride of the second dimension
 * \param p1 The padding of the first dimension
 * \param p2 The padding of the second dimension
 */
template <typename I_T, typename K_T, typename C_T>
void blas_conv4_valid(I_T&& input, K_T&& kernel, C_T&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    const auto K = etl::dim<0>(kernel); // The number of kernels
    const auto C = etl::dim<1>(input);  // The number of channels

    const auto m1 = etl::dim<2>(kernel);
    const auto m2 = etl::dim<3>(kernel);

    etl::dyn_matrix<value_t<I_T>, 4> kernels(C, K, m1, m2);

    for(size_t c = 0; c < C; ++c){
        for(size_t k = 0; k < K; ++k){
            kernels(c)(k) = fflip(kernel(k)(c));
        }
    }

    blas_conv4_valid_prepared(input, kernel, kernels, conv, s1, s2, p1, p2);
}

/*!
 * \brief Compute a 4D valid convolution using a BLAS matrix multiplication kernel
 * \param input The input matrix
 * \param kernel The kernel matrix, with flipped kernels
 * \param conv The output matrix
 * \param s1 The stride of the first dimension
 * \param s2 The stride of the second dimension
 * \param p1 The padding of the first dimension
 * \param p2 The padding of the second dimension
 */
template <typename I_T, typename K_T, typename C_T>
void blas_conv4_valid_flipped(I_T&& input, K_T&& kernel, C_T&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    const auto K = etl::dim<0>(kernel); // The number of kernels
    const auto C = etl::dim<1>(input);  // The number of channels

    const auto m1 = etl::dim<2>(kernel);
    const auto m2 = etl::dim<3>(kernel);

    etl::dyn_matrix<value_t<I_T>, 4> kernels(C, K, m1, m2);

    for(size_t c = 0; c < C; ++c){
        for(size_t k = 0; k < K; ++k){
            kernels(c)(k) = kernel(k)(c);
        }
    }

    blas_conv4_valid_prepared(input, kernel, kernels, conv, s1, s2, p1, p2);
}

/*!
 * \brief Compute a 4D valid convolution using a BLAS matrix multiplication kernel
 * \param input The input matrix
 * \param kernel The kernel matrix, with flipped kernels
 * \param conv The output matrix
 * \param s1 The stride of the first dimension
 * \param s2 The stride of the second dimension
 * \param p1 The padding of the first dimension
 * \param p2 The padding of the second dimension
 */
template <typename I_T, typename K_T, typename C_T>
void blas_conv4_valid_filter_prepared(I_T&& input, K_T&& kernel, C_T&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    using T = value_t<I_T>;

    const auto I = etl::dim<0>(input);
    const auto K = etl::dim<0>(conv);
    const auto C = etl::dim<1>(conv);

    const auto f1 = etl::dim<2>(conv);
    const auto f2 = etl::dim<3>(conv);

    const auto i1 = etl::dim<2>(input);
    const auto i2 = etl::dim<3>(input);

    const auto k1 = etl::dim<2>(kernel);
    const auto k2 = etl::dim<3>(kernel);

    // unit-strided result dimensions
    const size_t c1 = (i1 - k1 + 2 * p1) + 1;
    const size_t c2 = (i2 - k2 + 2 * p2) + 1;

    input.ensure_cpu_up_to_date();
    kernel.ensure_cpu_up_to_date();

    etl::dyn_matrix<T, 4> conv_temp(C, K, f1, f2);
    conv_temp = T(0);

    auto batch_fun_c = [&](const size_t first, const size_t last) {
        if (last - first) {
            SERIAL_SECTION {
                for (size_t c = first; c < last; ++c) {
                    etl::dyn_matrix<T, 2> input_col(k1 * k2, c1 * c2);

                    for (size_t i = 0; i < I; ++i) {
                        // Optimize for the most common case
                        if (cpp_likely(!p1 && !p2 && s1 == 1 && s2 == 1)) {
                            im2col_direct_tr(input_col, input(i)(c), k1, k2);
                            cblas_gemm(
                                CblasRowMajor,
                                CblasNoTrans, CblasNoTrans,
                                K, c1 * c2, k1 * k2,
                                T(1.0),
                                kernel(i).memory_start(), k1 * k2,
                                input_col.memory_start(), c1 * c2,
                                T(1.0),
                                conv_temp(c).memory_start(), f1 * f2);
                        } else {
                            if (p1 || p2) {
                                etl::dyn_matrix<T, 2> input_padded(i1 + 2 * p1, i2 + 2 * p2);
                                input_padded = T(0);

                                impl::common::pad_2d_input(input(i)(c), input_padded, p1, p2);

                                im2col_direct_tr(input_col, input_padded, k1, k2);
                            } else {
                                im2col_direct_tr(input_col, input(i)(c), k1, k2);
                            }

                            if (s1 > 1 || s2 > 1) {
                                etl::dyn_matrix<T, 3> tmp_result(K, c1, c2);

                                cblas_gemm(
                                    CblasRowMajor,
                                    CblasNoTrans, CblasNoTrans,
                                    K, c1 * c2, k1 * k2,
                                    T(1.0),
                                    kernel(i).memory_start(), k1 * k2,
                                    input_col.memory_start(), c1 * c2,
                                    T(0.0),
                                    tmp_result.memory_start(), c1 * c2);

                                // Strided copy of the large result into the small result
                                for (size_t k = 0; k < K; ++k) {
                                    for (size_t ii = 0; ii < f1; ++ii) {
                                        for (size_t j = 0; j < f2; ++j) {
                                            conv_temp(c, k, ii, j) += tmp_result(k, ii * s1, j * s2);
                                        }
                                    }
                                }
                            } else {
                                cblas_gemm(
                                    CblasRowMajor,
                                    CblasNoTrans, CblasNoTrans,
                                    K, c1 * c2, k1 * k2,
                                    T(1.0),
                                    kernel(i).memory_start(), k1 * k2,
                                    input_col.memory_start(), c1 * c2,
                                    T(1.0),
                                    conv_temp(c).memory_start(), f1 * f2);
                            }
                        }
                    }
                }

                for (size_t c = 0; c < C; ++c) {
                    for (size_t k = 0; k < K; ++k) {
                        conv(k)(c) = conv_temp(c)(k);
                    }
                }
            }
        }
    };

    dispatch_1d_any(select_parallel(C, 2), batch_fun_c, 0, C);

    conv.invalidate_gpu();
}

/*!
 * \brief Compute a 4D valid convolution using a BLAS matrix multiplication kernel
 * \param input The input matrix
 * \param kernel The kernel matrix, with flipped kernels
 * \param conv The output matrix
 * \param s1 The stride of the first dimension
 * \param s2 The stride of the second dimension
 * \param p1 The padding of the first dimension
 * \param p2 The padding of the second dimension
 */
template <typename I_T, typename K_T, typename C_T>
void blas_conv4_valid_filter(I_T&& input, K_T&& kernel, C_T&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    auto prepared_k = force_temporary(kernel);

    // Flip the kernels
    prepared_k.deep_fflip_inplace();

    blas_conv4_valid_filter_prepared(input, prepared_k, conv, s1, s2, p1, p2);
}

/*!
 * \brief Compute a 4D valid convolution using a BLAS matrix multiplication kernel
 * \param input The input matrix
 * \param kernel The kernel matrix, with flipped kernels
 * \param conv The output matrix
 * \param s1 The stride of the first dimension
 * \param s2 The stride of the second dimension
 * \param p1 The padding of the first dimension
 * \param p2 The padding of the second dimension
 */
template <typename I_T, typename K_T, typename C_T>
void blas_conv4_valid_filter_flipped(I_T&& input, K_T&& kernel, C_T&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    blas_conv4_valid_filter_prepared(input, kernel, conv, s1, s2, p1, p2);
}

#else

//COVERAGE_EXCLUDE_BEGIN

/*!
 * \brief Compute the matrix mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gemm(A&& a, B&& b, C&& c) {
    cpp_unused(a);
    cpp_unused(b);
    cpp_unused(c);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief Compute the matrix mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gemm_nt(A&& a, B&& b, C&& c) {
    cpp_unused(a);
    cpp_unused(b);
    cpp_unused(c);
    cpp_unreachable("Unsupported feature called: blas gemm_nt");
}

/*!
 * \brief Compute the matrix mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gemm_tn(A&& a, B&& b, C&& c) {
    cpp_unused(a);
    cpp_unused(b);
    cpp_unused(c);
    cpp_unreachable("Unsupported feature called: blas gemm_tn");
}

/*!
 * \brief Compute the matrix mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gemm_tt(A&& a, B&& b, C&& c) {
    cpp_unused(a);
    cpp_unused(b);
    cpp_unused(c);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief Compute the matrix-vector mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gemv(A&& a, B&& b, C&& c) {
    cpp_unused(a);
    cpp_unused(b);
    cpp_unused(c);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief Compute the vector-matrix mutplication of a and b and store the result in c
 * param a The lhs of the multiplication
 * param b The rhs of the multiplication
 * param c The result
 */
template <typename A, typename B, typename C>
void gevm(A&& a, B&& b, C&& c) {
    cpp_unused(a);
    cpp_unused(b);
    cpp_unused(c);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief BLAS implementation of a 2D 'valid' convolution C = I * K, with multiple kernels
 * \param input The input matrix
 * \param kernels The kernel matrix
 * \param conv The output matrix
 */
template <typename I, typename K_T, typename C>
void blas_conv2_valid_multi(const I& input, const K_T& kernels, C&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    cpp_unused(input);
    cpp_unused(kernels);
    cpp_unused(conv);
    cpp_unused(s1);
    cpp_unused(s2);
    cpp_unused(p1);
    cpp_unused(p2);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief BLAS implementation of a 2D 'valid' convolution C = I * K, with multiple kernels
 * \param input The input matrix
 * \param kernels The kernel matrix
 * \param conv The output matrix
 */
template <typename I, typename K_T, typename C>
void blas_conv2_valid_multi_flipped(const I& input, const K_T& kernels, C&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    cpp_unused(input);
    cpp_unused(kernels);
    cpp_unused(conv);
    cpp_unused(s1);
    cpp_unused(s2);
    cpp_unused(p1);
    cpp_unused(p2);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief BLAS implementation of a 2D 'valid' convolution C = I * K, with multiple images and multiple kernels
 * \param input The input matrix
 * \param kernels The kernel matrix
 * \param conv The output matrix
 */
template <typename I, typename K_T, typename C>
void blas_conv2_valid_multi_multi(const I& input, const K_T& kernels, C&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    cpp_unused(input);
    cpp_unused(kernels);
    cpp_unused(conv);
    cpp_unused(s1);
    cpp_unused(s2);
    cpp_unused(p1);
    cpp_unused(p2);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief BLAS implementation of a 2D 'valid' convolution C = I * K, with multiple images and multiple kernels
 * \param input The input matrix
 * \param kernels The kernel matrix
 * \param conv The output matrix
 */
template <typename I, typename K_T, typename C>
void blas_conv2_valid_multi_multi_flipped(const I& input, const K_T& kernels, C&& conv, size_t s1, size_t s2, size_t p1, size_t p2) {
    cpp_unused(input);
    cpp_unused(kernels);
    cpp_unused(conv);
    cpp_unused(s1);
    cpp_unused(s2);
    cpp_unused(p1);
    cpp_unused(p2);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief Compute a 4D valid convolution using a BLAS matrix multiplication kernel
 * \param input The input matrix
 * \param kernel The kernel matrix, with flipped kernels
 * \param conv The output matrix
 * \param s1 The stride of the first dimension
 * \param s2 The stride of the second dimension
 * \param p1 The padding of the first dimension
 * \param p2 The padding of the second dimension
 */
template <typename I_T, typename K_T, typename C_T>
void blas_conv4_valid(I_T&& input, K_T&& kernel, C_T&& conv, size_t s1, size_t s2, size_t p1, size_t p2){
    cpp_unused(input);
    cpp_unused(kernel);
    cpp_unused(conv);
    cpp_unused(s1);
    cpp_unused(s2);
    cpp_unused(p1);
    cpp_unused(p2);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief Compute a 4D valid convolution, with flipped kernels, using a BLAS matrix multiplication kernel
 * \param input The input matrix
 * \param kernel The kernel matrix, with flipped kernels
 * \param conv The output matrix
 * \param s1 The stride of the first dimension
 * \param s2 The stride of the second dimension
 * \param p1 The padding of the first dimension
 * \param p2 The padding of the second dimension
 */
template <typename I_T, typename K_T, typename C_T>
void blas_conv4_valid_flipped(I_T&& input, K_T&& kernel, C_T&& conv, size_t s1, size_t s2, size_t p1, size_t p2){
    cpp_unused(input);
    cpp_unused(kernel);
    cpp_unused(conv);
    cpp_unused(s1);
    cpp_unused(s2);
    cpp_unused(p1);
    cpp_unused(p2);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief Compute a 4D valid filter convolution using a BLAS matrix multiplication kernel
 * \param input The input matrix
 * \param kernel The kernel matrix, with flipped kernels
 * \param conv The output matrix
 * \param s1 The stride of the first dimension
 * \param s2 The stride of the second dimension
 * \param p1 The padding of the first dimension
 * \param p2 The padding of the second dimension
 */
template <typename I_T, typename K_T, typename C_T>
void blas_conv4_valid_filter(I_T&& input, K_T&& kernel, C_T&& conv, size_t s1, size_t s2, size_t p1, size_t p2){
    cpp_unused(input);
    cpp_unused(kernel);
    cpp_unused(conv);
    cpp_unused(s1);
    cpp_unused(s2);
    cpp_unused(p1);
    cpp_unused(p2);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

/*!
 * \brief Compute a 4D valid filter convolution, with flipped kernels,  using a BLAS matrix multiplication kernel
 * \param input The input matrix
 * \param kernel The kernel matrix, with flipped kernels
 * \param conv The output matrix
 * \param s1 The stride of the first dimension
 * \param s2 The stride of the second dimension
 * \param p1 The padding of the first dimension
 * \param p2 The padding of the second dimension
 */
template <typename I_T, typename K_T, typename C_T>
void blas_conv4_valid_filter_flipped(I_T&& input, K_T&& kernel, C_T&& conv, size_t s1, size_t s2, size_t p1, size_t p2){
    cpp_unused(input);
    cpp_unused(kernel);
    cpp_unused(conv);
    cpp_unused(s1);
    cpp_unused(s2);
    cpp_unused(p1);
    cpp_unused(p2);
    cpp_unreachable("Unsupported feature called: blas gemm");
}

//COVERAGE_EXCLUDE_END

#endif

} //end of namespace blas

} //end of namespace impl

} //end of namespace etl
