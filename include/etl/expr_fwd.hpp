//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

namespace etl {

template <typename Expr>
struct optimizable;

template <typename Expr>
struct optimizer;

template <typename Expr>
struct transformer;

struct identity_op;

struct transform_op;

template <typename Sub>
struct stateful_op;

template <typename T, typename Expr, typename UnaryOp>
struct unary_expr;

template <typename T, typename LeftExpr, typename BinaryOp, typename RightExpr>
struct binary_expr;

template <typename Generator>
class generator_expr;

template <typename Expr>
struct optimized_expr;

template <typename Expr>
struct serial_expr;

template <typename Selector, Selector V, typename Expr>
struct selected_expr;

template <typename Expr>
struct parallel_expr;

template <typename Expr, typename R = etl::nanoseconds>
struct timed_expr;

template <typename D, typename T, typename A, typename B, typename R>
struct temporary_expr_bin;

template <typename T, typename AExpr, typename BExpr, typename Op>
struct temporary_binary_expr;

template <typename T, typename AExpr, typename BExpr, typename Op>
struct temporary_binary_expr_state;

template <typename T, size_t D>
struct dim_view;

template <typename T, bool Aligned, typename Enable = void>
struct sub_view;

template <typename T, bool Aligned, typename Enable = void>
struct sub_matrix_2d;

template <typename T, typename Enable = void>
struct slice_view;

template <typename T, bool Aligned>
struct memory_slice_view;

template <typename T, bool DMA, size_t... Dims>
struct fast_matrix_view;

template <typename T, size_t D, typename Enable = void>
struct dyn_matrix_view;

template <typename A>
struct transpose_expr;

} //end of namespace etl
