//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

//The implementations
#include "etl/impl/std/mmul.hpp"
#include "etl/impl/std/strassen_mmul.hpp"
#include "etl/impl/blas/gemm.hpp"
#include "etl/impl/vec/gemm.hpp"
#include "etl/impl/eblas/gemm.hpp"
#include "etl/impl/cublas/gemm.hpp"

namespace etl {

namespace detail {

/*!
 * \brief Select an implementation of GEMM, not considering local context
 * \param n1 The left dimension of the  multiplication
 * \param n2 The inner dimension of the  multiplication
 * \param n3 The right dimension of the  multiplication
 * \return The implementation to use
 */
template <bool DMA, typename T>
inline cpp14_constexpr gemm_impl select_default_gemm_impl(const std::size_t n1, const std::size_t n2, const std::size_t n3) {
    cpp_unused(n2);

    //Only std implementation is able to handle non-dma expressions
    if (!DMA) {
        return gemm_impl::STD;
    }

    //Note since these boolean will be known at compile time, the conditions will be a lot simplified
    constexpr bool blas   = is_cblas_enabled;
    constexpr bool cublas = is_cublas_enabled;

    if (cublas) {
        if (n1 * n3 < gemm_cublas_min) {
            if (blas) {
                return gemm_impl::BLAS;
            }

            if (n1 * n3 < gemm_std_max) {
                return gemm_impl::STD;
            }
        }

        return gemm_impl::CUBLAS;
    } else if (blas) {
        return gemm_impl::BLAS;
    }

    //EBLAS has too much overhead for small matrices and does not handle complex numbers
    if (n1 * n3 < gemm_std_max || is_complex_t<T>::value) {
        return gemm_impl::STD;
    } else {
        return gemm_impl::FAST;
    }
}

/*!
 * \brief Select an implementation of GEMM
 * \param n1 The left dimension of the  multiplication
 * \param n2 The inner dimension of the  multiplication
 * \param n3 The right dimension of the  multiplication
 * \return The implementation to use
 */
template <bool DMA, typename T>
inline gemm_impl select_gemm_impl(const std::size_t n1, const std::size_t n2, const std::size_t n3) {
    if (local_context().gemm_selector.forced) {
        auto forced = local_context().gemm_selector.impl;

        switch (forced) {
            //CUBLAS cannot always be used
            case gemm_impl::CUBLAS:
                if (!is_cublas_enabled || !DMA) {                                                                                     //COVERAGE_EXCLUDE_LINE
                    std::cerr << "Forced selection to CUBLAS gemm implementation, but not possible for this expression" << std::endl; //COVERAGE_EXCLUDE_LINE
                    return select_default_gemm_impl<DMA, T>(n1, n2, n3);                                                              //COVERAGE_EXCLUDE_LINE
                }                                                                                                                     //COVERAGE_EXCLUDE_LINE

                return forced;

            //BLAS cannot always be used
            case gemm_impl::BLAS:
                if (!is_cblas_enabled || !DMA) {                                                                                    //COVERAGE_EXCLUDE_LINE
                    std::cerr << "Forced selection to BLAS gemm implementation, but not possible for this expression" << std::endl; //COVERAGE_EXCLUDE_LINE
                    return select_default_gemm_impl<DMA, T>(n1, n2, n3);                                                            //COVERAGE_EXCLUDE_LINE
                }                                                                                                                   //COVERAGE_EXCLUDE_LINE

                return forced;

            //EBLAS cannot always be used
            case gemm_impl::FAST:
                if (!DMA || is_complex_t<T>::value) {                                                                                //COVERAGE_EXCLUDE_LINE
                    std::cerr << "Forced selection to EBLAS gemm implementation, but not possible for this expression" << std::endl; //COVERAGE_EXCLUDE_LINE
                    return select_default_gemm_impl<DMA, T>(n1, n2, n3);                                                             //COVERAGE_EXCLUDE_LINE
                }                                                                                                                    //COVERAGE_EXCLUDE_LINE

                return forced;

            //In other cases, simply use the forced impl
            default:
                return forced;
        }
    }

    return select_default_gemm_impl<DMA, T>(n1, n2, n3);
}

/*!
 * \brief Select an implementation of GEMV, not considering local context
 * \param n1 The left dimension of the  multiplication
 * \param n2 The right dimension of the  multiplication
 * \return The implementation to use
 */
template <bool DMA, typename T>
inline cpp14_constexpr gemm_impl select_default_gemv_impl(const std::size_t n1, const std::size_t n2) {
    //Only std implementation is able to handle non-dma expressions
    if (!DMA) {
        return gemm_impl::STD;
    }

    //Note since these boolean will be known at compile time, the conditions will be a lot simplified
    constexpr bool blas   = is_cblas_enabled;
    constexpr bool cublas = is_cublas_enabled;

    if (blas) {
        return gemm_impl::BLAS;
    }

    if (cublas) {
        if (is_complex_single_t<T>::value && n1 * n2 > 1000 * 1000) {
            return gemm_impl::CUBLAS;
        }
    }

    //TODO Add selection for VEC

    return gemm_impl::STD;
}

/*!
 * \brief Select an implementation of GEMV
 * \param n1 The left dimension of the  multiplication
 * \param n2 The right dimension of the  multiplication
 * \return The implementation to use
 */
template <bool DMA, typename T>
inline gemm_impl select_gemv_impl(const std::size_t n1, const std::size_t n2) {
    if (local_context().gemm_selector.forced) {
        auto forced = local_context().gemm_selector.impl;

        switch (forced) {
            //CUBLAS cannot always be used
            case gemm_impl::CUBLAS:
                if (!is_cublas_enabled || !DMA) {                                                                                     //COVERAGE_EXCLUDE_LINE
                    std::cerr << "Forced selection to CUBLAS gemv implementation, but not possible for this expression" << std::endl; //COVERAGE_EXCLUDE_LINE
                    return select_default_gemv_impl<DMA, T>(n1, n2);                                                                  //COVERAGE_EXCLUDE_LINE
                }                                                                                                                     //COVERAGE_EXCLUDE_LINE

                return forced;

            //BLAS cannot always be used
            case gemm_impl::BLAS:
                if (!is_cblas_enabled || !DMA) {                                                                                    //COVERAGE_EXCLUDE_LINE
                    std::cerr << "Forced selection to BLAS gemv implementation, but not possible for this expression" << std::endl; //COVERAGE_EXCLUDE_LINE
                    return select_default_gemv_impl<DMA, T>(n1, n2);                                                                //COVERAGE_EXCLUDE_LINE
                }                                                                                                                   //COVERAGE_EXCLUDE_LINE

                return forced;

            //EBLAS cannot always be used
            case gemm_impl::FAST:
                std::cerr << "Forced selection to EBLAS gemv implementation, but there is no such implementation" << std::endl; //COVERAGE_EXCLUDE_LINE
                return select_default_gemv_impl<DMA, T>(n1, n2);                                                                //COVERAGE_EXCLUDE_LINE

            //In other cases, simply use the forced impl
            default:
                return forced;
        }
    }

    return select_default_gemv_impl<DMA, T>(n1, n2);
}

/*!
 * \brief Select an implementation of GEVM, not considering local context
 * \param n1 The left dimension of the  multiplication
 * \param n2 The right dimension of the  multiplication
 * \return The implementation to use
 */
template <bool DMA, typename T>
inline cpp14_constexpr gemm_impl select_default_gevm_impl(const std::size_t n1, const std::size_t n2) {
    //Only std implementation is able to handle non-dma expressions
    if (!DMA) {
        return gemm_impl::STD;
    }

    //Note since these boolean will be known at compile time, the conditions will be a lot simplified
    constexpr bool blas   = is_cblas_enabled;
    constexpr bool cublas = is_cublas_enabled;

    if (blas) {
        return gemm_impl::BLAS;
    }

    if (cublas) {
        if (is_complex_single_t<T>::value && n1 * n2 > 1000 * 1000) {
            return gemm_impl::CUBLAS;
        }
    }

    return gemm_impl::STD;
}

/*!
 * \brief Select an implementation of GEVM
 * \param n1 The left dimension of the  multiplication
 * \param n2 The right dimension of the  multiplication
 * \return The implementation to use
 */
template <bool DMA, typename T>
inline gemm_impl select_gevm_impl(const std::size_t n1, const std::size_t n2) {
    if (local_context().gemm_selector.forced) {
        auto forced = local_context().gemm_selector.impl;

        switch (forced) {
            //CUBLAS cannot always be used
            case gemm_impl::CUBLAS:
                if (!is_cublas_enabled || !DMA) {                                                                                     //COVERAGE_EXCLUDE_LINE
                    std::cerr << "Forced selection to CUBLAS gevm implementation, but not possible for this expression" << std::endl; //COVERAGE_EXCLUDE_LINE
                    return select_default_gevm_impl<DMA, T>(n1, n2);                                                                  //COVERAGE_EXCLUDE_LINE
                }                                                                                                                     //COVERAGE_EXCLUDE_LINE

                return forced;

            //BLAS cannot always be used
            case gemm_impl::BLAS:
                if (!is_cblas_enabled || !DMA) {                                                                                    //COVERAGE_EXCLUDE_LINE
                    std::cerr << "Forced selection to BLAS gevm implementation, but not possible for this expression" << std::endl; //COVERAGE_EXCLUDE_LINE
                    return select_default_gevm_impl<DMA, T>(n1, n2);                                                                //COVERAGE_EXCLUDE_LINE
                }                                                                                                                   //COVERAGE_EXCLUDE_LINE

                return forced;

            //EBLAS cannot always be used
            case gemm_impl::FAST:
                std::cerr << "Forced selection to EBLAS gevm implementation, but there is no such implementation" << std::endl; //COVERAGE_EXCLUDE_LINE
                return select_default_gevm_impl<DMA, T>(n1, n2);                                                                //COVERAGE_EXCLUDE_LINE

            //In other cases, simply use the forced impl
            default:
                return forced;
        }
    }

    return select_default_gevm_impl<DMA, T>(n1, n2);
}

/*!
 * \brief Functor for matrix-matrix multiplication
 */
struct mm_mul_impl {
    /*!
     * \brief Apply the function C = A * B
     * \param a The lhs of the multiplication
     * \param b The rhs of the multiplication
     * \param c The target of the multiplication
     */
    template <typename A, typename B, typename C>
    static void apply(A&& a, B&& b, C&& c) {
        gemm_impl impl = select_gemm_impl<all_dma<A, B, C>::value, value_t<A>>(etl::dim<0>(a), etl::dim<1>(a), etl::dim<1>(c));

        if (impl == gemm_impl::STD) {
            etl::impl::standard::mm_mul(a, b, c);
        } else if (impl == gemm_impl::FAST) {
            etl::impl::eblas::gemm(a, b, c);
        } else if (impl == gemm_impl::VEC) {
            etl::impl::vec::gemm(a, b, c);
        } else if (impl == gemm_impl::BLAS) {
            etl::impl::blas::gemm(a, b, c);
        } else if (impl == gemm_impl::CUBLAS) {
            etl::impl::cublas::gemm(a, b, c);
        }
    }
};

/*!
 * \brief Functor for vector-matrix multiplication
 */
struct vm_mul_impl {
    /*!
     * \brief Apply the function C = A * B
     * \param a The lhs of the multiplication
     * \param b The rhs of the multiplication
     * \param c The target of the multiplication
     */
    template <typename A, typename B, typename C>
    static void apply(A&& a, B&& b, C&& c) {
        gemm_impl impl = select_gevm_impl<all_dma<A, B, C>::value, value_t<A>>(etl::dim<0>(b), etl::dim<1>(b));

        if (impl == gemm_impl::STD) {
            etl::impl::standard::vm_mul(a, b, c);
        } else if (impl == gemm_impl::BLAS) {
            etl::impl::blas::gevm(a, b, c);
        } else if (impl == gemm_impl::VEC) {
            etl::impl::vec::gevm(a, b, c);
        } else if (impl == gemm_impl::CUBLAS) {
            etl::impl::cublas::gevm(a, b, c);
        }
    }
};

/*!
 * \brief Functor for matrix-vector multiplication
 */
struct mv_mul_impl {
    /*!
     * \brief Apply the function C = A * B
     * \param a The lhs of the multiplication
     * \param b The rhs of the multiplication
     * \param c The target of the multiplication
     */
    template <typename A, typename B, typename C>
    static void apply(A&& a, B&& b, C&& c) {
        gemm_impl impl = select_gemv_impl<all_dma<A, B, C>::value, value_t<A>>(etl::dim<0>(a), etl::dim<1>(a));

        if (impl == gemm_impl::STD) {
            etl::impl::standard::mv_mul(a, b, c);
        } else if (impl == gemm_impl::BLAS) {
            etl::impl::blas::gemv(a, b, c);
        } else if (impl == gemm_impl::VEC) {
            etl::impl::vec::gemv(a, b, c);
        } else if (impl == gemm_impl::CUBLAS) {
            etl::impl::cublas::gemv(a, b, c);
        }
    }
};

/*!
 * \brief Functor for Strassen matrix-matrix multiplication
 */
struct strassen_mm_mul_impl {
    /*!
     * \brief Apply the function C = A * B
     * \param a The lhs of the multiplication
     * \param b The rhs of the multiplication
     * \param c The target of the multiplication
     */
    template <typename A, typename B, typename C>
    static void apply(A&& a, B&& b, C&& c) {
        etl::impl::standard::strassen_mm_mul(a, b, c);
    }
};

} //end of namespace detail

} //end of namespace etl
