//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

// Handle master flags first

#ifdef ETL_CUDA
static_assert(false, "ETL_CUDA should never be set directly");
#endif

#ifdef ETL_VECTORIZE_FULL

//VECTORIZE_FULL enables VECTORIZE_EXPR
#ifndef ETL_VECTORIZE_EXPR
#define ETL_VECTORIZE_EXPR
#endif

//VECTORIZE_FULL enables VECTORIZE_IMPL
#ifndef ETL_VECTORIZE_IMPL
#define ETL_VECTORIZE_IMPL
#endif

#endif //ETL_VECTORIZE_FULL

//MKL mode enables BLAS mode
#ifdef ETL_MKL_MODE
#ifndef ETL_BLAS_MODE
#define ETL_BLAS_MODE
#endif
#endif

// ETL_GPU enabled all GPU flags
#ifdef ETL_GPU

#ifndef ETL_CUBLAS_MODE
#define ETL_CUBLAS_MODE
#endif

#ifndef ETL_CUFFT_MODE
#define ETL_CUFFT_MODE
#endif

#ifndef ETL_CUDNN_MODE
#define ETL_CUDNN_MODE
#endif

#endif

// Convert all the defines to booleans

#ifdef ETL_VECTORIZE_EXPR
#define ETL_VECTORIZE_EXPR_BOOL true
#else
#define ETL_VECTORIZE_EXPR_BOOL false
#endif

#ifdef ETL_VECTORIZE_IMPL
#define ETL_VECTORIZE_IMPL_BOOL true
#else
#define ETL_VECTORIZE_IMPL_BOOL false
#endif

#ifdef ETL_CONV_VALID_FFT
#define ETL_CONV_VALID_FFT_BOOL true
#else
#define ETL_CONV_VALID_FFT_BOOL false
#endif

#ifdef ETL_PARALLEL
#define ETL_PARALLEL_BOOL true
#else
#define ETL_PARALLEL_BOOL false
#endif

#ifdef ETL_MKL_MODE
#define ETL_MKL_MODE_BOOL true
#else
#define ETL_MKL_MODE_BOOL false
#endif

#ifdef ETL_BLAS_MODE
#define ETL_BLAS_MODE_BOOL true
#else
#define ETL_BLAS_MODE_BOOL false
#endif

#ifdef ETL_BLAS_THREADS
#define ETL_BLAS_THREADS_BOOL true
#else
#define ETL_BLAS_THREADS_BOOL false
#endif

#ifdef ETL_CUBLAS_MODE
#define ETL_CUDA
#define ETL_CUBLAS_MODE_BOOL true
#else
#define ETL_CUBLAS_MODE_BOOL false
#endif

#ifdef ETL_CUFFT_MODE
#define ETL_CUDA
#define ETL_CUFFT_MODE_BOOL true
#else
#define ETL_CUFFT_MODE_BOOL false
#endif

#ifdef ETL_CUDNN_MODE
#define ETL_CUDA
#define ETL_CUDNN_MODE_BOOL true
#else
#define ETL_CUDNN_MODE_BOOL false
#endif

#ifdef ETL_ELEMENT_WISE_MULTIPLICATION
#define ETL_ELEMENT_WISE_MULTIPLICATION_BOOL true
#else
#define ETL_ELEMENT_WISE_MULTIPLICATION_BOOL false
#endif

#ifdef ETL_STRICT_DIV
#define ETL_STRICT_DIV_BOOL true
#else
#define ETL_STRICT_DIV_BOOL false
#endif

#ifdef ETL_NO_STREAMING
#define ETL_NO_STREAMING_BOOL true
#else
#define ETL_NO_STREAMING_BOOL false
#endif

#ifdef ETL_NO_PADDING
#define ETL_NO_PADDING_BOOL true
#else
#define ETL_NO_PADDING_BOOL false
#endif

#ifdef ETL_ADVANCED_PADDING
#define ETL_ADVANCED_PADDING_BOOL true
#else
#define ETL_ADVANCED_PADDING_BOOL false
#endif

#ifdef ETL_NO_PADDING_IMPL
#define ETL_NO_PADDING_IMPL_BOOL true
#else
#define ETL_NO_PADDING_IMPL_BOOL false
#endif

#ifdef ETL_NO_UNROLL_NON_VECT
#define ETL_NO_UNROLL_NON_VECT_BOOL true
#else
#define ETL_NO_UNROLL_NON_VECT_BOOL false
#endif

#ifdef __INTEL_COMPILER
#define ETL_INTEL_COMPILER_BOOL true
#else
#define ETL_INTEL_COMPILER_BOOL false
#endif

#ifdef ETL_CONV4_PREFER_BLAS
#define ETL_CONV4_PREFER_BLAS_BOOL true
#else
#define ETL_CONV4_PREFER_BLAS_BOOL false
#endif

// Vectorization detection

#ifdef __AVX512F__
#define ETL_VECTOR_MODE vector_mode_t::AVX512
#elif defined(__AVX__)
#define ETL_VECTOR_MODE vector_mode_t::AVX
#elif defined(__SSE3__)
#define ETL_VECTOR_MODE vector_mode_t::SSE3
#else
#define ETL_VECTOR_MODE vector_mode_t::NONE
#endif

#ifdef __AVX512F__
#define ETL_AVX512_BOOL true
#else
#define ETL_AVX512_BOOL false
#endif

#ifdef __AVX2__
#define ETL_AVX2_BOOL true
#else
#define ETL_AVX2_BOOL false
#endif

#ifdef __AVX__
#define ETL_AVX_BOOL true
#else
#define ETL_AVX_BOOL false
#endif

#ifdef __SSE3__
#define ETL_SSE3_BOOL true
#else
#define ETL_SSE3_BOOL false
#endif

// Configuration flags with values

#define ETL_DEFAULT_CACHE_SIZE 3UL * 1024 * 1024
#define ETL_DEFAULT_MAX_WORKSPACE 2UL * 1024 * 1024 * 1024
#define ETL_DEFAULT_CUDNN_MAX_WORKSPACE 2UL * 1024 * 1024 * 1024
#define ETL_DEFAULT_PARALLEL_THREADS std::thread::hardware_concurrency()

#ifndef ETL_CACHE_SIZE
#define ETL_CACHE_SIZE ETL_DEFAULT_CACHE_SIZE
#endif

#ifndef ETL_MAX_WORKSPACE
#define ETL_MAX_WORKSPACE ETL_DEFAULT_MAX_WORKSPACE
#endif

#ifndef ETL_CUDNN_MAX_WORKSPACE
#define ETL_CUDNN_MAX_WORKSPACE ETL_DEFAULT_CUDNN_MAX_WORKSPACE
#endif

#ifndef ETL_PARALLEL_THREADS
#define ETL_PARALLEL_THREADS ETL_DEFAULT_PARALLEL_THREADS
#endif
