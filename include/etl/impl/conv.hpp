//=======================================================================
// Copyright (c) 2014-2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef ETL_IMPL_CONVOLUTION_HPP
#define ETL_IMPL_CONVOLUTION_HPP

#include <algorithm>

//Include the implementations
#include "std/conv.hpp"
#include "sse/conv.hpp"
#include "avx/conv.hpp"
#include "reduc/conv_mmul.hpp"

namespace etl {

enum class conv_type {
    VALID,
    SAME,
    FULL
};

namespace detail {

template<typename I, typename K, typename C, typename Enable = void>
struct conv1_full_impl {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::standard::conv1_full(input, kernel, conv);
    }
};

template<typename I, typename K, typename C, typename Enable = void>
struct conv1_same_impl {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::standard::conv1_same(input, kernel, conv);
    }
};

template<typename I, typename K, typename C, typename Enable = void>
struct conv1_valid_impl {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::standard::conv1_valid(input, kernel, conv);
    }
};

template<typename I, typename K, typename C, typename Enable = void>
struct conv2_full_impl {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::standard::conv2_full(input, kernel, conv);
    }
};

template<typename I, typename K, typename C, typename Enable = void>
struct conv2_same_impl {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::standard::conv2_same(input, kernel, conv);
    }
};

template<typename I, typename K, typename C, typename Enable = void>
struct conv2_valid_impl {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::standard::conv2_valid(input, kernel, conv);
    }
};

template<conv_type TT, typename I, typename K, typename C, typename Enable = void>
struct conv_deep_impl {
    template<conv_type TT2 = TT, typename I2 = I, cpp_enable_if(decay_traits<I2>::dimensions() == 3 && TT2 == conv_type::FULL)>
    static void apply(const I& input, const K& kernel, C&& conv){
        for(std::size_t i = 0; i < dim<0>(input); ++i){
            conv(i) = conv_2d_full(input(i), kernel(i));
        }
    }

    template<conv_type TT2 = TT, typename I2 = I, cpp_enable_if(decay_traits<I2>::dimensions() == 3 && TT2 == conv_type::SAME)>
    static void apply(const I& input, const K& kernel, C&& conv){
        for(std::size_t i = 0; i < dim<0>(input); ++i){
            conv(i) = conv_2d_same(input(i), kernel(i));
        }
    }

    template<conv_type TT2 = TT, typename I2 = I, cpp_enable_if(decay_traits<I2>::dimensions() == 3 && TT2 == conv_type::VALID)>
    static void apply(const I& input, const K& kernel, C&& conv){
        for(std::size_t i = 0; i < dim<0>(input); ++i){
            conv(i) = conv_2d_valid(input(i), kernel(i));
        }
    }

    template<typename I2 = I, cpp_enable_if((decay_traits<I2>::dimensions() > 3))>
    static void apply(const I& input, const K& kernel, C&& conv){
        for(std::size_t i = 0; i < dim<0>(input); ++i){
            conv_deep_impl<TT, decltype(input(i)), decltype(kernel(i)), decltype(conv(i))>::apply(input(i), kernel(i), conv(i));
        }
    }
};

template<typename I, typename K, typename C, typename Enable = void>
using conv_deep_valid_impl = conv_deep_impl<conv_type::VALID, I, K, C>;

template<typename I, typename K, typename C, typename Enable = void>
using conv_deep_same_impl = conv_deep_impl<conv_type::SAME, I, K, C>;

template<typename I, typename K, typename C, typename Enable = void>
using conv_deep_full_impl = conv_deep_impl<conv_type::FULL, I, K, C>;

template<typename A, typename B, typename C>
using is_vector_3s = cpp::and_c<all_single_precision<A, B, C>, all_dma<A, B, C>>;

template<typename A, typename B, typename C>
using is_vector_3d = cpp::and_c<all_double_precision<A, B, C>, all_dma<A, B, C>>;

template<typename A, typename B, typename C>
struct is_sse_dconv : cpp::and_u<vectorize_impl, vector_mode == vector_mode_t::SSE3, is_vector_3d<A, B, C>::value> {};

template<typename A, typename B, typename C>
struct is_sse_sconv : cpp::and_u<vectorize_impl, vector_mode == vector_mode_t::SSE3, is_vector_3s<A, B, C>::value> {};

template<typename A, typename B, typename C>
struct is_avx_dconv : cpp::and_u<vectorize_impl, vector_mode == vector_mode_t::AVX, is_vector_3d<A, B, C>::value> {};

template<typename A, typename B, typename C>
struct is_avx_sconv : cpp::and_u<vectorize_impl, vector_mode == vector_mode_t::AVX, is_vector_3s<A, B, C>::value> {};

template<typename I, typename K, typename C>
struct conv1_full_impl<I, K, C, std::enable_if_t<is_avx_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::avx::dconv1_full(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_same_impl<I, K, C, std::enable_if_t<is_avx_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::avx::dconv1_same(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_valid_impl<I, K, C, std::enable_if_t<is_avx_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::avx::dconv1_valid(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_full_impl<I, K, C, std::enable_if_t<is_avx_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::sconv1_full(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_same_impl<I, K, C, std::enable_if_t<is_avx_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::sconv1_same(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_valid_impl<I, K, C, std::enable_if_t<is_avx_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::avx::sconv1_valid(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_full_impl<I, K, C, std::enable_if_t<is_avx_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::avx::dconv2_full(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_same_impl<I, K, C, std::enable_if_t<is_avx_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::avx::dconv2_same(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_valid_impl<I, K, C, std::enable_if_t<is_avx_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::avx::dconv2_valid(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_full_impl<I, K, C, std::enable_if_t<is_avx_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::avx::sconv2_full(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_same_impl<I, K, C, std::enable_if_t<is_avx_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::avx::sconv2_same(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_valid_impl<I, K, C, std::enable_if_t<is_avx_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::avx::sconv2_valid(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_full_impl<I, K, C, std::enable_if_t<is_sse_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::dconv1_full(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_same_impl<I, K, C, std::enable_if_t<is_sse_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::dconv1_same(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_valid_impl<I, K, C, std::enable_if_t<is_sse_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::dconv1_valid(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_full_impl<I, K, C, std::enable_if_t<is_sse_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::dconv2_full(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_same_impl<I, K, C, std::enable_if_t<is_sse_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::dconv2_same(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_valid_impl<I, K, C, std::enable_if_t<is_sse_dconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::dconv2_valid(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_full_impl<I, K, C, std::enable_if_t<is_sse_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::sconv1_full(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_same_impl<I, K, C, std::enable_if_t<is_sse_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::sconv1_same(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv1_valid_impl<I, K, C, std::enable_if_t<is_sse_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::sconv1_valid(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_valid_impl<I, K, C, std::enable_if_t<is_sse_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::sconv2_valid(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_same_impl<I, K, C, std::enable_if_t<is_sse_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::sconv2_same(input, kernel, conv);
    }
};

template<typename I, typename K, typename C>
struct conv2_full_impl<I, K, C, std::enable_if_t<is_sse_sconv<I,K,C>::value>> {
    static void apply(const I& input, const K& kernel, C&& conv){
        impl::sse::sconv2_full(input, kernel, conv);
    }
};

} //end of namespace detail

} //end of namespace etl

#endif
