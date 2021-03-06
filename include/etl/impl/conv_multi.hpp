//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file
 * \brief Contains descriptors for "multi" convolution operations
 */

#pragma once

namespace etl {

/*!
 * \brief Constexpr min between two values
 */
template<size_t A, size_t B>
struct c_min {
    static constexpr size_t value = A < B ? A : B; ///< The resulting value
};

/*!
 * \brief Return safely the D dimension of E.
 *
 * Once C++ offers a real static_if, this needs to be removed
 *
 * \return the Dth dimension of E
 */
template <size_t D, typename E>
constexpr size_t safe_dim() noexcept {
    return decay_traits<E>::template dim<c_min<D, etl::dimensions<E>() - 1>::value>();
}

namespace detail {

/*!
 * \brief The functor impl for 2D valid conv, with multiple kernels
 */
template<size_t S1 = 1, size_t S2 = 1, size_t P1 = 0, size_t P2 = 0>
struct conv2_valid_multi_impl {
    /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    static void apply(I&& input, K&& kernel, C&& conv) {
        auto impl = select_conv_valid_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::BLAS_VEC) {
            impl::vec::blas_conv2_valid_multi(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::BLAS_MKL) {
            impl::blas::blas_conv2_valid_multi(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::VALID_FFT_MKL) {
            impl::blas::fft_conv2_valid_multi(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::CUDNN) {
            impl::cudnn::conv2_valid_multi(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::VEC) {
            impl::vec::conv2_valid_multi(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_valid_multi(input, kernel, conv, S1, S2, P1, P2);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_valid_multi";
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    static void check(I&& input, K&& kernel, const C& conv){
        static_assert(etl::dimensions<I>() == 2, "Invalid number of dimensions for input of conv2_valid_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_valid_multi");
        static_assert(etl::dimensions<C>() == 3, "Invalid number of dimensions for conv of conv2_valid_multi");

        cpp_assert(etl::dim(conv, 0) == etl::dim(kernel, 0), "Invalid dimensions for conv2_valid_multi");
        cpp_assert(etl::dim(conv, 1) == (etl::dim(input, 0) - etl::dim(kernel, 1) + 2 * P1) / S1 + 1, "Invalid dimensions for conv2_valid_multi");
        cpp_assert(etl::dim(conv, 2) == (etl::dim(input, 1) - etl::dim(kernel, 2) + 2 * P2) / S2 + 1, "Invalid dimensions for conv2_valid_multi");
        cpp_assert(etl::dim(input, 0) >= etl::dim(kernel, 1), "Invalid dimensions for conv2_valid_multi");
        cpp_assert(etl::dim(input, 1) >= etl::dim(kernel, 2), "Invalid dimensions for conv2_valid_multi");

        cpp_unused(input);
        cpp_unused(kernel);
        cpp_unused(conv);
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    static void check(){
        static_assert(etl::dimensions<I>() == 2, "Invalid number of dimensions for input of conv2_valid_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_valid_multi");
        static_assert(etl::dimensions<C>() == 3, "Invalid number of dimensions for conv of conv2_valid_multi");

        static_assert(etl::dim<0,C>() == etl::dim<0,K>(), "Invalid dimensions for conv2_valid_multi");
        static_assert(etl::dim<1,C>() == (etl::dim<0,I>() - etl::dim<1,K>() + 2 * P1) / S1 + 1, "Invalid dimensions for conv2_valid_multi");
        static_assert(etl::dim<2,C>() == (etl::dim<1,I>() - etl::dim<2,K>() + 2 * P2) / S2 + 1, "Invalid dimensions for conv2_valid_multi");
        static_assert(etl::dim<0,I>() >= etl::dim<1,K>(), "Invalid dimensions for conv2_valid_multi");
        static_assert(etl::dim<1,I>() >= etl::dim<2,K>(), "Invalid dimensions for conv2_valid_multi");
    }

    /*!
     * \brief Returns the dth dimension of the result of the convolution
     */
    template <typename I, typename K>
    static size_t dim(size_t d, I&& input, K&& kernel){
        cpp_assert(d < 3, "Invalid dimensions access");

        if(d == 0){
            return etl::dim(kernel, 0);
        } else if(d == 1){
            return (etl::dim(input, d - 1) - etl::dim(kernel, d) + 2 * P1) / S1  + 1;
        } else {
            return (etl::dim(input, d - 1) - etl::dim(kernel, d) + 2 * P2) / S2  + 1;
        }
    }

    /*!
     * \brief Returns the Dth dimension of the result of the convolution
     */
    template <size_t D, typename I, typename K>
    static constexpr size_t dim(){
        static_assert(D < 3, "Invalid dimension access");

        return D == 0 ? etl::dim<0,K>()
            : D == 1 ? (etl::safe_dim<0,I>() - etl::dim<1,K>() + 2 * P1) / S1 + 1
            : (etl::safe_dim<1,I>() - etl::dim<2,K>() + 2 * P2) / S2 + 1;
    }
};

/*!
 * \brief The functor impl for 2D valid conv, with multiple kernels
 */
template<size_t S1 = 1, size_t S2 = 1, size_t P1 = 0, size_t P2 = 0>
struct conv2_valid_multi_flipped_impl : conv2_valid_multi_impl<S1, S2, P1, P2> {
    /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    static void apply(I&& input, K&& kernel, C&& conv) {
        auto impl = select_conv_valid_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::BLAS_VEC) {
            impl::vec::blas_conv2_valid_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::BLAS_MKL) {
            impl::blas::blas_conv2_valid_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::VALID_FFT_MKL) {
            impl::blas::fft_conv2_valid_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::CUDNN) {
            impl::cudnn::conv2_valid_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::VEC) {
            impl::vec::conv2_valid_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_valid_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_valid_multi_flipped";
    }
};

/*!
 * \brief The functor impl for 2D valid conv, with multiple kernels
 */
template<size_t S1 = 1, size_t S2 = 1, size_t P1 = 0, size_t P2 = 0>
struct conv2_valid_multi_multi_impl {
    /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    static void apply(I&& input, K&& kernel, C&& conv) {
        auto impl = select_conv_valid_multi_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::BLAS_VEC) {
            impl::vec::blas_conv2_valid_multi_multi(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::BLAS_MKL) {
            impl::blas::blas_conv2_valid_multi_multi(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::VALID_FFT_MKL) {
            impl::blas::fft_conv2_valid_multi_multi(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::VEC) {
            impl::vec::conv2_valid_multi_multi(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_valid_multi_multi(input, kernel, conv, S1, S2, P1, P2);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_valid_multi_multi";
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    static void check(I&& input, K&& kernel, const C& conv){
        static_assert(etl::dimensions<I>() == 3, "Invalid number of dimensions for input of conv2_valid_multi_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_valid_multi_multi");
        static_assert(etl::dimensions<C>() == 4, "Invalid number of dimensions for conv of conv2_valid_multi_multi");

        cpp_assert(etl::dim(conv, 0) == etl::dim(kernel, 0), "Invalid dimensions for conv2_valid_multi_multi");
        cpp_assert(etl::dim(conv, 1) == etl::dim(input, 0), "Invalid dimensions for conv2_valid_multi_multi");
        cpp_assert(etl::dim(conv, 2) == (etl::dim(input, 1) - etl::dim(kernel, 1) + 2 * P1) / S1 + 1, "Invalid dimensions for conv2_valid_multi_multi");
        cpp_assert(etl::dim(conv, 3) == (etl::dim(input, 2) - etl::dim(kernel, 2) + 2 * P2) / S2 + 1, "Invalid dimensions for conv2_valid_multi_multi");
        cpp_assert(etl::dim(input, 1) >= etl::dim(kernel, 1), "Invalid dimensions for conv2_valid_multi_multi");
        cpp_assert(etl::dim(input, 2) >= etl::dim(kernel, 2), "Invalid dimensions for conv2_valid_multi_multi");

        cpp_unused(input);
        cpp_unused(kernel);
        cpp_unused(conv);
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    static void check(){
        static_assert(etl::dimensions<I>() == 3, "Invalid number of dimensions for input of conv2_valid_multi_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_valid_multi_multi");
        static_assert(etl::dimensions<C>() == 4, "Invalid number of dimensions for conv of conv2_valid_multi_multi");

        static_assert(etl::dim<0,C>() == etl::dim<0,K>(), "Invalid dimensions for conv2_valid_multi_multi");
        static_assert(etl::dim<1,C>() == etl::dim<0,I>(), "Invalid dimensions for conv2_valid_multi_multi");
        static_assert(etl::dim<2,C>() == (etl::dim<1,I>() - etl::dim<1,K>() + 2 * P1) / S1 + 1, "Invalid dimensions for conv2_valid_multi_multi");
        static_assert(etl::dim<3,C>() == (etl::dim<2,I>() - etl::dim<2,K>() + 2 * P2) / S2 + 1, "Invalid dimensions for conv2_valid_multi_multi");
        static_assert(etl::dim<1,I>() >= etl::dim<1,K>(), "Invalid dimensions for conv2_valid_multi_multi");
        static_assert(etl::dim<2,I>() >= etl::dim<2,K>(), "Invalid dimensions for conv2_valid_multi_multi");
    }

    /*!
     * \brief Returns the dth dimension of the result of the convolution
     */
    template <typename I, typename K>
    static size_t dim(size_t d, I&& input, K&& kernel){
        cpp_assert(d < 4, "Invalid dimensions access");

        if(d == 0){
            return etl::dim(kernel, 0);
        } else if(d == 1){
            return etl::dim(input, 0);
        } else if(d == 2){
            return (etl::dim(input, d - 1) - etl::dim(kernel, d - 1 ) + 2 * P1) / S1  + 1;
        } else {
            return (etl::dim(input, d - 1) - etl::dim(kernel, d - 1 ) + 2 * P2) / S2  + 1;
        }
    }

    /*!
     * \brief Returns the Dth dimension of the result of the convolution
     */
    template <size_t D, typename I, typename K>
    static constexpr size_t dim(){
        static_assert(D < 4, "Invalid dimension access");

        return D == 0 ? etl::dim<0,K>()
            : D == 1 ? etl::dim<0, I>()
            : D == 2 ? (etl::safe_dim<1,I>() - etl::dim<1,K>() + 2 * P1) / S1 + 1
            : (etl::safe_dim<2,I>() - etl::dim<2,K>() + 2 * P2) / S2 + 1;
    }
};

/*!
 * \brief The functor impl for 2D valid conv, with multiple kernels
 */
template<size_t S1 = 1, size_t S2 = 1, size_t P1 = 0, size_t P2 = 0>
struct conv2_valid_multi_multi_flipped_impl : conv2_valid_multi_multi_impl<S1, S2, P1, P2> {
    /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    static void apply(I&& input, K&& kernel, C&& conv) {
        auto impl = select_conv_valid_multi_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::BLAS_VEC) {
            impl::vec::blas_conv2_valid_multi_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::BLAS_MKL) {
            impl::blas::blas_conv2_valid_multi_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::VALID_FFT_MKL) {
            impl::blas::fft_conv2_valid_multi_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::VEC) {
            impl::vec::conv2_valid_multi_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_valid_multi_multi_flipped(input, kernel, conv, S1, S2, P1, P2);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_valid_multi_multi_flipped";
    }
};

/*!
 * \brief The functor impl for 2D valid conv, with multiple kernels
 */
struct dyn_conv2_valid_multi_impl {
    const size_t s1; ///< The first dimension stride
    const size_t s2; ///< The second dimension stride
    const size_t p1; ///< The first dimension padding
    const size_t p2; ///< The second dimension padding

    /*!
     * \brief Construct a new dyn_conv2_valid_multi_impl
     * \param s1 The first dimension stride
     * \param s2 The second dimension stride
     * \param p1 The first dimension padding
     * \param p2 The second dimension padding
     */
    dyn_conv2_valid_multi_impl(size_t s1, size_t s2, size_t p1, size_t p2) : s1(s1), s2(s2), p1(p1), p2(p2) {
        //Nothing else to init
    }

    /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    void apply(I&& input, K&& kernel, C&& conv) const {
        auto impl = select_conv_valid_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::BLAS_VEC) {
            impl::vec::blas_conv2_valid_multi(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::BLAS_MKL) {
            impl::blas::blas_conv2_valid_multi(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::VALID_FFT_MKL) {
            impl::blas::fft_conv2_valid_multi(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::CUDNN) {
            impl::cudnn::conv2_valid_multi(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::VEC) {
            impl::vec::conv2_valid_multi(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_valid_multi(input, kernel, conv, s1, s2, p1, p2);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_valid_multi";
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    void check(I&& input, K&& kernel, const C& conv) const {
        static_assert(etl::dimensions<I>() == 2, "Invalid number of dimensions for input of conv2_valid_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_valid_multi");
        static_assert(etl::dimensions<C>() == 3, "Invalid number of dimensions for conv of conv2_valid_multi");

        cpp_assert(etl::dim(conv, 0) == etl::dim(kernel, 0), "Invalid dimensions for conv2_valid_multi");
        cpp_assert(etl::dim(conv, 1) == (etl::dim(input, 0) - etl::dim(kernel, 1) + 2 * p1) / s1 + 1, "Invalid dimensions for conv2_valid_multi");
        cpp_assert(etl::dim(conv, 2) == (etl::dim(input, 1) - etl::dim(kernel, 2) + 2 * p2) / s2 + 1, "Invalid dimensions for conv2_valid_multi");
        cpp_assert(etl::dim(input, 0) >= etl::dim(kernel, 1), "Invalid dimensions for conv2_valid_multi");
        cpp_assert(etl::dim(input, 1) >= etl::dim(kernel, 2), "Invalid dimensions for conv2_valid_multi");

        cpp_unused(input);
        cpp_unused(kernel);
        cpp_unused(conv);
    }

    /*!
     * \brief Returns the dth dimension of the result of the convolution
     */
    template <typename I, typename K>
    size_t dim(size_t d, I&& input, K&& kernel) const {
        cpp_assert(d < 3, "Invalid dimensions access");

        if(d == 0){
            return etl::dim(kernel, 0);
        } else if(d == 1){
            return (etl::dim(input, d - 1) - etl::dim(kernel, d) + 2 * p1) / s1  + 1;
        } else {
            return (etl::dim(input, d - 1) - etl::dim(kernel, d) + 2 * p2) / s2  + 1;
        }
    }
};

/*!
 * \brief The functor impl for 2D valid conv, with multiple kernels
 */
struct dyn_conv2_valid_multi_flipped_impl : dyn_conv2_valid_multi_impl {
    /*!
     * \brief Construct a new dyn_conv2_valid_multi_flipped_impl
     * \param s1 The first dimension stride
     * \param s2 The second dimension stride
     * \param p1 The first dimension padding
     * \param p2 The second dimension padding
     */
    dyn_conv2_valid_multi_flipped_impl(size_t s1, size_t s2, size_t p1, size_t p2) : dyn_conv2_valid_multi_impl(s1, s2, p1, p2) {
        //Nothing else to init
    }

     /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    void apply(I&& input, K&& kernel, C&& conv) const {
        auto impl = select_conv_valid_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::BLAS_VEC) {
            impl::vec::blas_conv2_valid_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::BLAS_MKL) {
            impl::blas::blas_conv2_valid_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::VALID_FFT_MKL) {
            impl::blas::fft_conv2_valid_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::CUDNN) {
            impl::cudnn::conv2_valid_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::VEC) {
            impl::vec::conv2_valid_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_valid_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_valid_multi_flipped";
    }
};

/*!
 * \brief The functor impl for 2D valid conv, with multiple kernels
 */
struct dyn_conv2_valid_multi_multi_impl {
    const size_t s1; ///< The first dimension stride
    const size_t s2; ///< The second dimension stride
    const size_t p1; ///< The first dimension padding
    const size_t p2; ///< The second dimension padding

    /*!
     * \brief Construct a new dyn_conv2_valid_multi_multi_impl
     * \param s1 The first dimension stride
     * \param s2 The second dimension stride
     * \param p1 The first dimension padding
     * \param p2 The second dimension padding
     */
    dyn_conv2_valid_multi_multi_impl(size_t s1, size_t s2, size_t p1, size_t p2) : s1(s1), s2(s2), p1(p1), p2(p2) {
        //Nothing else to init
    }

    /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    void apply(I&& input, K&& kernel, C&& conv) const {
        auto impl = select_conv_valid_multi_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::BLAS_VEC) {
            impl::vec::blas_conv2_valid_multi_multi(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::BLAS_MKL) {
            impl::blas::blas_conv2_valid_multi_multi(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::VALID_FFT_MKL) {
            impl::blas::fft_conv2_valid_multi_multi(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::VEC) {
            impl::vec::conv2_valid_multi_multi(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_valid_multi_multi(input, kernel, conv, s1, s2, p1, p2);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_valid_multi_multi";
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    void check(I&& input, K&& kernel, const C& conv) const {
        static_assert(etl::dimensions<I>() == 3, "Invalid number of dimensions for input of conv2_valid_multi_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_valid_multi_multi");
        static_assert(etl::dimensions<C>() == 4, "Invalid number of dimensions for conv of conv2_valid_multi_multi");

        cpp_assert(etl::dim(conv, 0) == etl::dim(kernel, 0), "Invalid dimensions for conv2_valid_multi_multi");
        cpp_assert(etl::dim(conv, 1) == etl::dim(input, 0), "Invalid dimensions for conv2_valid_multi_multi");
        cpp_assert(etl::dim(conv, 2) == (etl::dim(input, 1) - etl::dim(kernel, 1) + 2 * p1) / s1 + 1, "Invalid dimensions for conv2_valid_multi_multi");
        cpp_assert(etl::dim(conv, 3) == (etl::dim(input, 2) - etl::dim(kernel, 2) + 2 * p2) / s2 + 1, "Invalid dimensions for conv2_valid_multi_multi");
        cpp_assert(etl::dim(input, 1) >= etl::dim(kernel, 1), "Invalid dimensions for conv2_valid_multi_multi");
        cpp_assert(etl::dim(input, 2) >= etl::dim(kernel, 2), "Invalid dimensions for conv2_valid_multi_multi");

        cpp_unused(input);
        cpp_unused(kernel);
        cpp_unused(conv);
    }

    /*!
     * \brief Returns the dth dimension of the result of the convolution
     */
    template <typename I, typename K>
    size_t dim(size_t d, I&& input, K&& kernel) const {
        cpp_assert(d < 3, "Invalid dimensions access");

        if(d == 0){
            return etl::dim(kernel, 0);
        } else if(d == 1){
            return (etl::dim(input, d - 1) - etl::dim(kernel, d) + 2 * p1) / s1  + 1;
        } else {
            return (etl::dim(input, d - 1) - etl::dim(kernel, d) + 2 * p2) / s2  + 1;
        }
    }
};

/*!
 * \brief The functor impl for 2D valid conv, with multiple kernels
 */
struct dyn_conv2_valid_multi_multi_flipped_impl : dyn_conv2_valid_multi_multi_impl {
    /*!
     * \brief Construct a new dyn_conv2_valid_multi_multi_flipped_impl
     * \param s1 The first dimension stride
     * \param s2 The second dimension stride
     * \param p1 The first dimension padding
     * \param p2 The second dimension padding
     */
    dyn_conv2_valid_multi_multi_flipped_impl(size_t s1, size_t s2, size_t p1, size_t p2) : dyn_conv2_valid_multi_multi_impl(s1, s2, p1, p2) {
        //Nothing else to init
    }

     /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    void apply(I&& input, K&& kernel, C&& conv) const {
        auto impl = select_conv_valid_multi_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::BLAS_VEC) {
            impl::vec::blas_conv2_valid_multi_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::BLAS_MKL) {
            impl::blas::blas_conv2_valid_multi_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::VALID_FFT_MKL) {
            impl::blas::fft_conv2_valid_multi_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::VEC) {
            impl::vec::conv2_valid_multi_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_valid_multi_multi_flipped(input, kernel, conv, s1, s2, p1, p2);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_valid_multi_multi_flipped";
    }
};

/*!
 * \brief The functor impl for 2D full conv, with multiple kernels
 */
struct conv2_full_multi_impl {
    /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    static void apply(I&& input, K&& kernel, C&& conv) {
        auto impl = select_conv_full_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::CUDNN) {
            impl::cudnn::conv2_full_multi(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::VEC){
            impl::vec::conv2_full_multi(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_full_multi(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::FFT_STD) {
            impl::standard::conv2_full_multi_fft(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::FFT_MKL) {
            impl::blas::conv2_full_multi(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::FFT_CUFFT) {
            impl::cufft::conv2_full_multi(input, kernel, conv);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_full_multi";
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    static void check(I&& input, K&& kernel, const C& conv){
        static_assert(etl::dimensions<I>() == 2, "Invalid number of dimensions for input of conv2_full_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_full_multi");
        static_assert(etl::dimensions<C>() == 3, "Invalid number of dimensions for conv of conv2_full_multi");

        cpp_assert(etl::dim(conv, 0) == etl::dim(kernel, 0), "Invalid dimensions for conv2_full_multi");
        cpp_assert(etl::dim(conv, 1) == etl::dim(input, 0) + etl::dim(kernel, 1) - 1, "Invalid dimensions for conv2_full_multi");
        cpp_assert(etl::dim(conv, 2) == etl::dim(input, 1) + etl::dim(kernel, 2) - 1, "Invalid dimensions for conv2_full_multi");
        cpp_assert(etl::dim(input, 0) >= etl::dim(kernel, 1), "Invalid dimensions for conv2_full_multi");
        cpp_assert(etl::dim(input, 1) >= etl::dim(kernel, 2), "Invalid dimensions for conv2_full_multi");

        cpp_unused(input);
        cpp_unused(kernel);
        cpp_unused(conv);
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    static void check(){
        static_assert(etl::dimensions<I>() == 2, "Invalid number of dimensions for input of conv2_full_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_full_multi");
        static_assert(etl::dimensions<C>() == 3, "Invalid number of dimensions for conv of conv2_full_multi");

        static_assert(etl::dim<0,C>() == etl::dim<0,K>(), "Invalid dimensions for conv2_full_multi");
        static_assert(etl::dim<1,C>() == etl::dim<0,I>() + etl::dim<1,K>() - 1, "Invalid dimensions for conv2_full_multi");
        static_assert(etl::dim<2,C>() == etl::dim<1,I>() + etl::dim<2,K>() - 1, "Invalid dimensions for conv2_full_multi");
        static_assert(etl::dim<0,I>() >= etl::dim<1,K>(), "Invalid dimensions for conv2_full_multi");
        static_assert(etl::dim<1,I>() >= etl::dim<2,K>(), "Invalid dimensions for conv2_full_multi");
    }

    /*!
     * \brief Returns the dth dimension of the result of the convolution
     */
    template <typename I, typename K>
    static size_t dim(size_t d, I&& input, K&& kernel){
        cpp_assert(d < 3, "Invalid dimensions access");

        if(d == 0){
            return etl::dim(kernel, 0);
        } else {
            return etl::dim(input, d - 1) + etl::dim(kernel, d) - 1;
        }
    }

    /*!
     * \brief Returns the Dth dimension of the result of the convolution
     */
    template <size_t D, typename I, typename K>
    static constexpr size_t dim(){
        static_assert(D < 3, "Invalid dimension access");

        return D == 0 ? etl::dim<0,K>()
            : etl::safe_dim<D - 1,I>() + etl::dim<D,K>() - 1;
    }
};

/*!
 * \brief The functor impl for 2D full conv, with multiple kernels
 */
struct conv2_full_multi_flipped_impl {
    /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    static void apply(I&& input, K&& kernel, C&& conv) {
        auto impl = select_conv_full_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::CUDNN) {
            impl::cudnn::conv2_full_multi_flipped(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::VEC){
            impl::vec::conv2_full_multi_flipped(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_full_multi_flipped(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::FFT_STD) {
            impl::standard::conv2_full_multi_flipped_fft(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::FFT_MKL) {
            impl::blas::conv2_full_multi_flipped(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::FFT_CUFFT) {
            impl::cufft::conv2_full_multi_flipped(input, kernel, conv);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_full_multi_flipped";
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    static void check(I&& input, K&& kernel, const C& conv){
        conv2_full_multi_impl::check(input, kernel, conv);
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    static void check(){
        conv2_full_multi_impl::template check<I, K, C>();
    }

    /*!
     * \brief Returns the dth dimension of the result of the convolution
     */
    template <typename I, typename K>
    static size_t dim(size_t d, I&& input, K&& kernel){
        cpp_assert(d < 3, "Invalid dimensions access");

        if(d == 0){
            return etl::dim(kernel, 0);
        } else {
            return etl::dim(input, d - 1) + etl::dim(kernel, d) - 1;
        }
    }

    /*!
     * \brief Returns the Dth dimension of the result of the convolution
     */
    template <size_t D, typename I, typename K>
    static constexpr size_t dim(){
        static_assert(D < 3, "Invalid dimension access");

        return D == 0 ? etl::dim<0,K>()
            : etl::safe_dim<D - 1,I>() + etl::dim<D,K>() - 1;
    }
};

/*!
 * \brief The functor impl for 2D same conv, with multiple kernels
 */
struct conv2_same_multi_impl {
    /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    static void apply(I&& input, K&& kernel, C&& conv) {
        auto impl = select_conv_same_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::VEC){
            impl::vec::conv2_same_multi(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_same_multi(input, kernel, conv);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_same_multi";
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    static void check(I&& input, K&& kernel, const C& conv){
        static_assert(etl::dimensions<I>() == 2, "Invalid number of dimensions for input of conv2_same_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_same_multi");
        static_assert(etl::dimensions<C>() == 3, "Invalid number of dimensions for conv of conv2_same_multi");

        cpp_assert(etl::dim(conv, 0) == etl::dim(kernel, 0), "Invalid dimensions for conv2_same_multi");
        cpp_assert(etl::dim(conv, 1) == etl::dim(input, 0), "Invalid dimensions for conv2_same_multi");
        cpp_assert(etl::dim(conv, 2) == etl::dim(input, 1), "Invalid dimensions for conv2_same_multi");
        cpp_assert(etl::dim(input, 0) >= etl::dim(kernel, 1), "Invalid dimensions for conv2_same_multi");
        cpp_assert(etl::dim(input, 1) >= etl::dim(kernel, 2), "Invalid dimensions for conv2_same_multi");

        cpp_unused(input);
        cpp_unused(kernel);
        cpp_unused(conv);
    }

    /*!
     * \brief Assert that the convolution is done on correct dimensions
     */
    template <typename I, typename K, typename C>
    static void check(){
        static_assert(etl::dimensions<I>() == 2, "Invalid number of dimensions for input of conv2_same_multi");
        static_assert(etl::dimensions<K>() == 3, "Invalid number of dimensions for kernel of conv2_same_multi");
        static_assert(etl::dimensions<C>() == 3, "Invalid number of dimensions for conv of conv2_same_multi");

        static_assert(etl::dim<0,C>() == etl::dim<0,K>(), "Invalid dimensions for conv2_same_multi");
        static_assert(etl::dim<1,C>() == etl::dim<0,I>(), "Invalid dimensions for conv2_same_multi");
        static_assert(etl::dim<2,C>() == etl::dim<1,I>(), "Invalid dimensions for conv2_same_multi");
        static_assert(etl::dim<0,I>() >= etl::dim<1,K>(), "Invalid dimensions for conv2_same_multi");
        static_assert(etl::dim<1,I>() >= etl::dim<2,K>(), "Invalid dimensions for conv2_same_multi");
    }

    /*!
     * \brief Returns the dth dimension of the result of the convolution
     */
    template <typename I, typename K>
    static size_t dim(size_t d, I&& input, K&& kernel){
        cpp_assert(d < 3, "Invalid dimensions access");

        if(d == 0){
            return etl::dim(kernel, 0);
        } else {
            return etl::dim(input, d - 1);
        }
    }

    /*!
     * \brief Returns the Dth dimension of the result of the convolution
     */
    template <size_t D, typename I, typename K>
    static constexpr size_t dim(){
        static_assert(D < 3, "Invalid dimension access");

        return D == 0 ? etl::dim<0,K>()
            : etl::safe_dim<D - 1,I>();
    }
};

/*!
 * \brief The functor impl for 2D same conv, with multiple flipped kernels
 */
struct conv2_same_multi_flipped_impl : conv2_same_multi_impl {
    /*!
     * \brief Apply the convolution
     * \param input The input expression
     * \param kernel The kernel expression
     * \param conv The output expression
     */
    template <typename I, typename K, typename C>
    static void apply(I&& input, K&& kernel, C&& conv) {
        auto impl = select_conv_same_multi_impl<I, K, C>();

        if (impl == etl::conv_multi_impl::VEC){
            impl::vec::conv2_same_multi_flipped(input, kernel, conv);
        } else if (impl == etl::conv_multi_impl::STD){
            impl::standard::conv2_same_multi_flipped(input, kernel, conv);
        } else {
            cpp_unreachable("Invalid conv implementation selection");
        }
    }

    /*!
     * \brief Returns the description of the operation
     */
    static constexpr const char* desc(){
        return "conv2_same_multi";
    }
};

} //end of namespace detail

} //end of namespace etl
