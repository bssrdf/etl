//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file conv_expression_builder.hpp
 * \brief Contains all the operators and functions to build convolution expressions.
*/

#pragma once

#include "etl/expression_helpers.hpp"

namespace etl {

/*!
 * \brief Creates an expression representing the valid 1D convolution of a and b
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the valid 1D convolution of a and b
 */
template <typename A, typename B>
auto conv_1d_valid(A&& a, B&& b) -> detail::temporary_binary_helper<A, B, conv1_valid_expr> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing the valid 1D convolution of a and b, the result will be stored in c
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the valid 1D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_1d_valid(A&& a, B&& b, C&& c){
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = conv_1d_valid(a, b);
}

/*!
 * \brief Creates an expression representing the same 1D convolution of a and b
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the same 1D convolution of a and b
 */
template <typename A, typename B>
auto conv_1d_same(A&& a, B&& b) -> detail::temporary_binary_helper<A, B, conv1_same_expr> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing the same 1D convolution of a and b, the result will be stored in c
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the same 1D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_1d_same(A&& a, B&& b, C&& c){
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = conv_1d_same(a, b);
}

/*!
 * \brief Creates an expression representing the full 1D convolution of a and b
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the full 1D convolution of a and b
 */
template <typename A, typename B>
auto conv_1d_full(A&& a, B&& b) -> detail::temporary_binary_helper<A, B, conv1_full_expr> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing the full 1D convolution of a and b, the result will be stored in c
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the full 1D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_1d_full(A&& a, B&& b, C&& c) {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = conv_1d_full(a, b);
}

/*!
 * \brief Creates an expression representing the full 1D convolution of a and b, implemented by FFT
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the full 1D convolution of a and b
 */
template <typename A, typename B>
auto fft_conv_1d_full(A&& a, B&& b) -> detail::temporary_binary_helper<A, B, fft_conv1_full_expr> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing the full 1D convolution of a and b, the result will be stored in c, implemented by FFT
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the full 1D convolution of a and b
 */
template <typename A, typename B, typename C>
auto fft_conv_1d_full(A&& a, B&& b, C&& c){
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = fft_conv_1d_full(a, b);
}

/*!
 * \brief Creates an expression representing the full 1D convolution of a and b, the convolution is done with the faster available implementation.
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the full 1D convolution of a and b
 */
template <typename A, typename B, cpp_enable_if_cst(has_fast_fft)>
auto fast_conv_1d_full(A&& a, B&& b) {
    return fft_conv_1d_full(std::forward<A>(a), std::forward<B>(b));
}

/*!
 * \brief Creates an expression representing the full 1D convolution of a and b, the convolution is done with the faster available implementation.
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the full 1D convolution of a and b
 */
template <typename A, typename B, cpp_disable_if_cst(has_fast_fft)>
auto fast_conv_1d_full(A&& a, B&& b) {
    return conv_1d_full(std::forward<A>(a), std::forward<B>(b));
}

/*!
 * \brief Creates an expression representing the full 1D convolution of a and b, the result is stored in c, the convolution is done with the faster available implementation.
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the full 1D convolution of a and b
 */
template <typename A, typename B, typename C, cpp_enable_if_cst(has_fast_fft)>
auto fast_conv_1d_full(A&& a, B&& b, C&& c) {
    return fft_conv_1d_full(std::forward<A>(a), std::forward<B>(b), std::forward<C>(c));
}

/*!
 * \brief Creates an expression representing the full 1D convolution of a and b, the result is stored in c, the convolution is done with the faster available implementation.
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the full 1D convolution of a and b
 */
template <typename A, typename B, typename C, cpp_disable_if_cst(has_fast_fft)>
auto fast_conv_1d_full(A&& a, B&& b, C&& c) {
    return conv_1d_full(std::forward<A>(a), std::forward<B>(b), std::forward<C>(c));
}

/*!
 * \brief Creates an expression representing the valid 2D convolution of a and b
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the valid 2D convolution of a and b
 */
template <typename A, typename B>
auto conv_2d_valid(A&& a, B&& b) -> detail::temporary_binary_helper<A, B, conv2_valid_expr> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing the valid 2D convolution of a and b, the result will be stored in c
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the valid 2D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_2d_valid(A&& a, B&& b, C&& c) {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = conv_2d_valid(a, b);
}

/*!
 * \brief Creates an expression representing the same 2D convolution of a and b
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the same 2D convolution of a and b
 */
template <typename A, typename B>
auto conv_2d_same(A&& a, B&& b) -> detail::temporary_binary_helper<A, B, conv2_same_expr> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing the same 2D convolution of a and b, the result will be stored in c
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the same 2D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_2d_same(A&& a, B&& b, C&& c) {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = conv_2d_same(a, b);
}

/*!
 * \brief Creates an expression representing the full 2D convolution of a and b
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the full 2D convolution of a and b
 */
template <typename A, typename B>
auto conv_2d_full(A&& a, B&& b) -> detail::temporary_binary_helper<A, B, conv2_full_expr> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing the full 2D convolution of a and b, the result will be stored in c
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the full 2D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_2d_full(A&& a, B&& b, C&& c) {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = conv_2d_full(a, b);
}

/*!
 * \brief Creates an expression representing the full 2D convolution of a and b, computed with a FFT.
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the full 2D convolution of a and b
 */
template <typename A, typename B>
auto fft_conv_2d_full(A&& a, B&& b) -> detail::temporary_binary_helper<A, B, fft_conv2_full_expr> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing the full 2D convolution of a and b, the result will be stored in c, computed with a FFT.
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the full 2D convolution of a and b
 */
template <typename A, typename B, typename C>
auto fft_conv_2d_full(A&& a, B&& b, C&& c) {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = fft_conv_2d_full(a, b);
}

/*!
 * \brief Creates an expression representing the full 2D convolution of a and b, the convolution is done with the faster available implementation.
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the full 2D convolution of a and b
 */
template <typename A, typename B, cpp_enable_if_cst(has_fast_fft)>
auto fast_conv_2d_full(A&& a, B&& b) {
    return fft_conv_2d_full(std::forward<A>(a), std::forward<B>(b));
}

/*!
 * \brief Creates an expression representing the full 2D convolution of a and b, the convolution is done with the faster available implementation.
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing the full 2D convolution of a and b
 */
template <typename A, typename B, cpp_disable_if_cst(has_fast_fft)>
auto fast_conv_2d_full(A&& a, B&& b) {
    return conv_2d_full(std::forward<A>(a), std::forward<B>(b));
}

/*!
 * \brief Creates an expression representing the full 2D convolution of a and b, the result is stored in c, the convolution is done with the faster available implementation.
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the full 2D convolution of a and b
 */
template <typename A, typename B, typename C, cpp_enable_if_cst(has_fast_fft)>
auto fast_conv_2d_full(A&& a, B&& b, C&& c) {
    return fft_conv_2d_full(std::forward<A>(a), std::forward<B>(b), std::forward<C>(c));
}

/*!
 * \brief Creates an expression representing the full 2D convolution of a and b, the result is stored in c, the convolution is done with the faster available implementation.
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing the full 2D convolution of a and b
 */
template <typename A, typename B, typename C, cpp_disable_if_cst(has_fast_fft)>
auto fast_conv_2d_full(A&& a, B&& b, C&& c) {
    return conv_2d_full(std::forward<A>(a), std::forward<B>(b), std::forward<C>(c));
}

/*!
 * \brief Creates an expression representing many valid 2D convolution of a and b.
 *
 * Only the last two dimensions are used for the convolution itself, the first dimensions are used as containers to perform multiple FFT.
 *
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing many valid 2D convolution of a and b
 */
template <typename A, typename B>
auto conv_deep_valid(A&& a, B&& b) -> detail::dim_temporary_binary_helper<A, B, conv_deep_valid_expr, decay_traits<A>::dimensions()> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing many valid 2D convolution of a and b, the result is stored in c
 *
 * Only the last two dimensions are used for the convolution itself, the first dimensions are used as containers to perform multiple FFT.
 *
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing many valid 2D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_deep_valid(A&& a, B&& b, C&& c) {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = conv_deep_valid(a, b);
}

/*!
 * \brief Creates an expression representing many same 2D convolution of a and b.
 *
 * Only the last two dimensions are used for the convolution itself, the first dimensions are used as containers to perform multiple FFT.
 *
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing many same 2D convolution of a and b
 */
template <typename A, typename B>
auto conv_deep_same(A&& a, B&& b) -> detail::dim_temporary_binary_helper<A, B, conv_deep_same_expr, decay_traits<A>::dimensions()> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing many same 2D convolution of a and b, the result is stored in c
 *
 * Only the last two dimensions are used for the convolution itself, the first dimensions are used as containers to perform multiple FFT.
 *
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing many same 2D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_deep_same(A&& a, B&& b, C&& c) {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = conv_deep_same(a, b);
}

/*!
 * \brief Creates an expression representing many full 2D convolution of a and b.
 *
 * Only the last two dimensions are used for the convolution itself, the first dimensions are used as containers to perform multiple FFT.
 *
 * \param a The input expression
 * \param b The kernel expression
 * \return an expression representing many full 2D convolution of a and b
 */
template <typename A, typename B>
auto conv_deep_full(A&& a, B&& b) -> detail::dim_temporary_binary_helper<A, B, conv_deep_full_expr, decay_traits<A>::dimensions()> {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value, "Convolution only supported for ETL expressions");

    return {a, b};
}

/*!
 * \brief Creates an expression representing many full 2D convolution of a and b, the result is stored in c
 *
 * Only the last two dimensions are used for the convolution itself, the first dimensions are used as containers to perform multiple FFT.
 *
 * \param a The input expression
 * \param b The kernel expression
 * \param c The result
 * \return an expression representing many full 2D convolution of a and b
 */
template <typename A, typename B, typename C>
auto conv_deep_full(A&& a, B&& b, C&& c) {
    static_assert(is_etl_expr<A>::value && is_etl_expr<B>::value && is_etl_expr<C>::value, "Convolution only supported for ETL expressions");

    return c = conv_deep_full(a, b);
}

//Special convolutions

/*!
 * \brief Pad the input matrix in the output matrix for convolution as multiplication
 * \param in The input matrix
 * \param out The output matrix
 */
template <typename F1, typename F2>
void complex_pad_4d(const F1& in, F2& out) {
    for (std::size_t outer1 = 0; outer1 < etl::dim<0>(in); ++outer1) {
        for (std::size_t outer2 = 0; outer2 < etl::dim<1>(in); ++outer2) {
            auto* direct = out(outer1)(outer2).memory_start();
            for (std::size_t i = 0; i < etl::dim<2>(in); ++i) {
                for (std::size_t j = 0; j < etl::dim<3>(in); ++j) {
                    direct[i * out.template dim<3>() + j] = in(outer1, outer2, i, j);
                }
            }
        }
    }
}

/*!
 * \brief Pad the input matrix in the output matrix for convolution as multiplication
 * \param in The input matrix
 * \param out The output matrix
 */
template <typename F1, typename F2>
void complex_pad_3d(const F1& in, F2& out) {
    for (std::size_t outer = 0; outer < etl::dim<0>(in); ++outer) {
        auto* direct = out(outer).memory_start();
        for (std::size_t i = 0; i < etl::dim<1>(in); ++i) {
            for (std::size_t j = 0; j < etl::dim<2>(in); ++j) {
                direct[i * out.template dim<2>() + j] = in(outer, i, j);
            }
        }
    }
}

/*!
 * \brief Pad the input matrix in the output matrix for convolution as multiplication
 * \param in The input matrix
 * \param out The output matrix
 */
template <typename F1, typename F2>
void complex_pad_2d(const F1& in, F2& out) {
    auto* direct = out.memory_start();
    for (std::size_t i = 0; i < etl::dim<0>(in); ++i) {
        for (std::size_t j = 0; j < etl::dim<1>(in); ++j) {
            direct[i * out.template dim<1>() + j] = in(i, j);
        }
    }
}

//TODO This should be moved
//TODO This should be adapted to an expression

template <typename A, typename B, typename C>
void conv_2d_valid_multi(A&& input, B&& kernels, C&& features) {
    //TODO Validate inputs

    if (is_mkl_enabled && conv_valid_fft) {
        const std::size_t K = etl::dim<0>(kernels);

        const std::size_t i1 = etl::dim<0>(input);
        const std::size_t i2 = etl::dim<1>(input);

        const std::size_t k1 = etl::dim<1>(kernels);
        const std::size_t k2 = etl::dim<2>(kernels);

        const std::size_t v1 = i1 - k1 + 1;
        const std::size_t v2 = i2 - k2 + 1;
        const std::size_t t1 = i1 + k1 - 1;
        const std::size_t t2 = i2 + k2 - 1;
        const std::size_t b1 = (t1 - v1) / 2;
        const std::size_t b2 = (t2 - v2) / 2;

        etl::dyn_matrix<std::complex<value_t<A>>> input_padded(t1, t2);
        etl::dyn_matrix<std::complex<value_t<A>>, 3> kernels_padded(K, t1, t2);
        etl::dyn_matrix<std::complex<value_t<A>>, 3> tmp_result(K, t1, t2);

        complex_pad_2d(input, input_padded);
        complex_pad_3d(kernels, kernels_padded);

        input_padded.fft2_inplace();
        kernels_padded.fft2_many_inplace();

        for (std::size_t k = 0; k < K; ++k) {
            tmp_result(k) = input_padded >> kernels_padded(k);
        }

        tmp_result.ifft2_many_inplace();

        for (std::size_t k = 0; k < K; ++k) {
            for (std::size_t i = 0; i < v1; ++i) {
                for (std::size_t j = 0; j < v2; ++j) {
                    features(k, i, j) = tmp_result(k, i + b1, j + b2).real();
                }
            }
        }
    } else if (is_cblas_enabled || is_cublas_enabled) {
        const std::size_t K  = etl::dim<0>(kernels);
        const std::size_t v1 = etl::dim<0>(input);
        const std::size_t v2 = etl::dim<1>(input);
        const std::size_t k1 = etl::dim<1>(kernels);
        const std::size_t k2 = etl::dim<2>(kernels);
        const std::size_t f1 = etl::dim<1>(features);
        const std::size_t f2 = etl::dim<2>(features);

        auto prepared_k = force_temporary(kernels);

        for (std::size_t i = 0; i < K; ++i) {
            prepared_k(i).fflip_inplace();
        }

        etl::dyn_matrix<value_t<A>, 2> input_col(k1 * k2, (v1 - k1 + 1) * (v2 - k2 + 1));
        im2col_direct_tr(input_col, input, k1, k2);

        *mul(
            etl::reshape(prepared_k, K, k1 * k2),
            input_col,
            etl::reshape(features, K, f1 * f2));
    } else {
        //Standard version
        for (size_t k = 0; k < etl::dim<0>(kernels); ++k) {
            features(k) = conv_2d_valid(input, kernels(k));
        }
    }
}

template <typename A, typename B, typename C>
void conv_2d_valid_multi_flipped(A&& input, B&& kernels, C&& features) {
    //TODO Validate inputs

    if (is_mkl_enabled && conv_valid_fft) {
        auto kernels_f = etl::force_temporary(kernels);

        for (std::size_t i = 0; i < etl::dim<0>(kernels_f); ++i) {
            kernels_f(i).fflip_inplace();
        }

        conv_2d_valid_multi(input, kernels_f, features);
    } else if (is_cblas_enabled || is_cublas_enabled) {
        const std::size_t K  = etl::dim<0>(kernels);
        const std::size_t v1 = etl::dim<0>(input);
        const std::size_t v2 = etl::dim<1>(input);
        const std::size_t f1 = etl::dim<1>(features);
        const std::size_t f2 = etl::dim<2>(features);
        const std::size_t k1 = etl::dim<1>(kernels);
        const std::size_t k2 = etl::dim<2>(kernels);

        etl::dyn_matrix<value_t<A>, 2> input_col(k1 * k2, (v1 - k1 + 1) * (v2 - k2 + 1));
        im2col_direct_tr(input_col, input, k1, k2);

        *mul(
            etl::reshape(kernels, K, k1 * k2),
            input_col,
            etl::reshape(features, K, f1 * f2));
    } else {
        //TODO Here we should maybe display a warning because it is suboptimal to flip twice

        //Standard version
        for (size_t k = 0; k < etl::dim<0>(kernels); ++k) {
            features(k) = conv_2d_valid(input, fflip(kernels(k)));
        }
    }
}

template <typename A, typename B, typename C>
void conv_3d_valid_multi(A&& input, B&& kernels, C&& features) {
    for (size_t c = 0; c < etl::dim<0>(input); ++c) {
        conv_2d_valid_multi(input(c), kernels(c), features(c));
    }
}

template <typename A, typename B, typename T_C>
void conv_3d_valid_multi_flipped(A&& input, B&& kernels, T_C&& features) {
    if (is_mkl_enabled && conv_valid_fft) {
        auto kernels_f = etl::force_temporary(kernels);

        for (std::size_t i = 0; i < etl::dim<0>(kernels_f); ++i) {
            for (std::size_t j = 0; j < etl::dim<1>(kernels_f); ++j) {
                kernels_f(i)(j).fflip_inplace();
            }
        }

        const std::size_t C  = etl::dim<0>(kernels);
        const std::size_t K  = etl::dim<1>(kernels);
        const std::size_t k1 = etl::dim<2>(kernels);
        const std::size_t k2 = etl::dim<3>(kernels);

        const std::size_t i1 = etl::dim<1>(input);
        const std::size_t i2 = etl::dim<2>(input);

        const std::size_t v1 = i1 - k1 + 1;
        const std::size_t v2 = i2 - k2 + 1;
        const std::size_t t1 = i1 + k1 - 1;
        const std::size_t t2 = i2 + k2 - 1;
        const std::size_t b1 = (t1 - v1) / 2;
        const std::size_t b2 = (t2 - v2) / 2;

        etl::dyn_matrix<std::complex<value_t<A>>, 3> input_padded(C, t1, t2);
        etl::dyn_matrix<std::complex<value_t<A>>, 4> kernels_padded(C, K, t1, t2);
        etl::dyn_matrix<std::complex<value_t<A>>, 4> tmp_result(C, K, t1, t2);

        complex_pad_3d(input, input_padded);
        complex_pad_4d(kernels, kernels_padded);

        input_padded.fft2_many_inplace();
        kernels_padded.fft2_many_inplace();

        for (std::size_t c = 0; c < C; ++c) {
            for (std::size_t k = 0; k < K; ++k) {
                tmp_result(c)(k) = input_padded(c) >> kernels_padded(c)(k);
            }
        }

        tmp_result.ifft2_many_inplace();

        for (std::size_t c = 0; c < C; ++c) {
            for (std::size_t k = 0; k < K; ++k) {
                for (std::size_t i = 0; i < v1; ++i) {
                    for (std::size_t j = 0; j < v2; ++j) {
                        features(c, k, i, j) = tmp_result(c, k, i + b1, j + b2).real();
                    }
                }
            }
        }
    } else {
        for (size_t c = 0; c < etl::dim<0>(input); ++c) {
            conv_2d_valid_multi_flipped(input(c), kernels(c), features(c));
        }
    }
}

template <typename A>
auto convmtx(A&& a, std::size_t h) -> detail::stable_transform_helper<A, dyn_convmtx_transformer> {
    static_assert(is_etl_expr<A>::value, "Convolution matrices only supported for ETL expressions");
    static_assert(decay_traits<A>::dimensions() == 1, "Convolutional matrix only works in 1D");

    return detail::stable_transform_helper<A, dyn_convmtx_transformer>{dyn_convmtx_transformer<detail::build_type<A>>(a, h)};
}

template <typename A>
auto convmtx2(A&& a, std::size_t k1, std::size_t k2) -> detail::stable_transform_helper<A, dyn_convmtx2_transformer> {
    static_assert(is_etl_expr<A>::value, "Convolution matrices only supported for ETL expressions");
    static_assert(decay_traits<A>::dimensions() == 2, "Convolutional matrix only works in 2D");

    return detail::stable_transform_helper<A, dyn_convmtx2_transformer>{dyn_convmtx2_transformer<detail::build_type<A>>(a, k1, k2)};
}

template <std::size_t K1, std::size_t K2, typename A>
auto convmtx2_direct(A&& a) -> temporary_unary_expr<value_t<A>, detail::build_type<A>, direct_convmtx2_expr<value_t<A>, K1, K2>, void> {
    static_assert(is_etl_expr<A>::value, "Convolution matrices only supported for ETL expressions");
    static_assert(decay_traits<A>::dimensions() == 2, "Convolutional matrix only works in 2D");

    return temporary_unary_expr<value_t<A>, detail::build_type<A>, direct_convmtx2_expr<value_t<A>, K1, K2>, void>{a};
}

} //end of namespace etl
