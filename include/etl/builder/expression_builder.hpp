//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

/*!
 * \file expression_builder.hpp
 * \brief Contains all the operators and functions to build expressions.
*/

#pragma once

//Include implementations
#include "etl/impl/dot.hpp"
#include "etl/impl/scalar_op.hpp"
#include "etl/impl/sum.hpp"
#include "etl/impl/norm.hpp"

namespace etl {

/*!
 * \brief Builds an expression representing the subtraction of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the subtraction of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<LE>::value, is_etl_expr<RE>::value)>
auto operator-(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, minus_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the addition of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the addition of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<LE>::value, is_etl_expr<RE>::value)>
auto operator+(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, plus_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

#ifdef ETL_ELEMENT_WISE_MULTIPLICATION

/*!
 * \brief Builds an expression representing the scalar multiplication of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the scalar multiplication of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<LE>::value, is_etl_expr<RE>::value)>
auto operator*(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, mul_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

#endif

/*!
 * \brief Builds an expression representing the scalar multipliation of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the scalar multipliation of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<LE>::value, is_etl_expr<RE>::value)>
auto operator>>(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, mul_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the scalar multiplication of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the scalar multiplication of lhs and rhs
 */
template <typename LE, typename RE>
auto scale(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, mul_binary_op> {
    validate_expression(lhs, rhs);

    return detail::left_binary_helper<LE, RE, mul_binary_op>{lhs, rhs};
}

/*!
 * \brief Builds an expression representing the division of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the division of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<LE>::value, is_etl_expr<RE>::value)>
auto operator/(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, div_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

/*!
 * \brief Builds an expression representing the modulo of lhs and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the modulo of lhs and rhs
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<LE>::value, is_etl_expr<RE>::value)>
auto operator%(LE&& lhs, RE&& rhs) -> detail::left_binary_helper<LE, RE, mod_binary_op> {
    validate_expression(lhs, rhs);

    return {lhs, rhs};
}

// Mix scalars and ETL expressions (vector,matrix,binary,unary)

/*!
 * \brief Builds an expression representing the subtraction of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the subtraction of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value, is_etl_expr<LE>::value)>
auto operator-(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, minus_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the subtraction of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the subtraction of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value, is_etl_expr<RE>::value)>
auto operator-(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, minus_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the addition of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the addition of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value, is_etl_expr<LE>::value)>
auto operator+(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, plus_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the addition of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the addition of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value, is_etl_expr<RE>::value)>
auto operator+(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, plus_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the multiplication of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the multiplication of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value, is_etl_expr<LE>::value)>
auto operator*(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, mul_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the multiplication of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the multiplication of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value, is_etl_expr<RE>::value)>
auto operator*(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, mul_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the multiplication of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the multiplication of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value, is_etl_expr<LE>::value)>
auto operator>>(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, mul_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the multiplication of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the multiplication of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value, is_etl_expr<RE>::value)>
auto operator>>(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, mul_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the division of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the division of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value, is_etl_expr<LE>::value, (is_div_strict || !std::is_floating_point<RE>::value))>
auto operator/(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, div_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the division of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the division of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value, is_etl_expr<LE>::value, !is_div_strict, std::is_floating_point<RE>::value)>
auto operator/(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, mul_binary_op> {
    return {lhs, scalar<value_t<LE>>(value_t<LE>(1.0) / rhs)};
}

/*!
 * \brief Builds an expression representing the division of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the division of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value, is_etl_expr<RE>::value)>
auto operator/(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, div_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

/*!
 * \brief Builds an expression representing the modulo of lhs and rhs (scalar)
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the modulo of lhs and rhs (scalar)
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<RE, value_t<LE>>::value, is_etl_expr<LE>::value)>
auto operator%(LE&& lhs, RE rhs) -> detail::left_binary_helper<LE, scalar<value_t<LE>>, mod_binary_op> {
    return {lhs, scalar<value_t<LE>>(rhs)};
}

/*!
 * \brief Builds an expression representing the modulo of lhs (scalar) and rhs
 * \param lhs The left hand side expression
 * \param rhs The right hand side expression
 * \return An expression representing the modulo of lhs (scalar) and rhs
 */
template <typename LE, typename RE, cpp_enable_if(std::is_convertible<LE, value_t<RE>>::value, is_etl_expr<RE>::value)>
auto operator%(LE lhs, RE&& rhs) -> detail::right_binary_helper<scalar<value_t<RE>>, RE, mod_binary_op> {
    return {scalar<value_t<RE>>(lhs), rhs};
}

// Compound operators

/*!
 * \brief Compound addition of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value, is_simple_lhs<LE>::value)>
LE& operator+=(LE&& lhs, RE rhs) {
    detail::scalar_add::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound addition of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE>::value, is_simple_lhs<LE>::value)>
LE& operator+=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_add_to(lhs);
    return lhs;
}

/*!
 * \brief Compound subtraction of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value, is_simple_lhs<LE>::value)>
LE& operator-=(LE&& lhs, RE rhs) {
    detail::scalar_sub::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound subtraction of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE>::value, is_simple_lhs<LE>::value)>
LE& operator-=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_sub_to(lhs);
    return lhs;
}

/*!
 * \brief Compound multiplication of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value, is_simple_lhs<LE>::value)>
LE& operator*=(LE&& lhs, RE rhs) {
    detail::scalar_mul::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound multiplication of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE>::value, is_simple_lhs<LE>::value)>
LE& operator*=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_mul_to(lhs);
    return lhs;
}

/*!
 * \brief Compound multiplication of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value, is_simple_lhs<LE>::value)>
LE& operator>>=(LE&& lhs, RE rhs) {
    detail::scalar_mul::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound multiplication of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE>::value, is_simple_lhs<LE>::value)>
LE& operator>>=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_mul_to(lhs);
    return lhs;
}

/*!
 * \brief Compound division of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value, is_simple_lhs<LE>::value)>
LE& operator/=(LE&& lhs, RE rhs) {
    detail::scalar_div::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound division of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE>::value, is_simple_lhs<LE>::value)>
LE& operator/=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_div_to(lhs);
    return lhs;
}

/*!
 * \brief Compound modulo of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(std::is_arithmetic<RE>::value, is_simple_lhs<LE>::value)>
LE& operator%=(LE&& lhs, RE rhs) {
    detail::scalar_mod::apply(lhs, rhs);
    return lhs;
}

/*!
 * \brief Compound modulo of the right hand side to the left hand side
 * \param lhs The left hand side, will be changed
 * \param rhs The right hand side
 * \return the left hand side
 */
template <typename LE, typename RE, cpp_enable_if(is_etl_expr<RE>::value, is_simple_lhs<LE>::value)>
LE& operator%=(LE&& lhs, RE&& rhs) {
    validate_expression(lhs, rhs);
    rhs.assign_mod_to(lhs);
    return lhs;
}

// Apply an unary expression on an ETL expression (vector,matrix,binary,unary)

/*!
 * \Apply Unary minus on the expression
 * \param value The expression on which to apply the operator
 * \return an expression representing the unary minus of the given expression
 */
template <typename E>
auto operator-(E&& value) -> detail::unary_helper<E, minus_unary_op> {
    return detail::unary_helper<E, minus_unary_op>{value};
}

/*!
 * \Apply Unary plus on the expression
 * \param value The expression on which to apply the operator
 * \return an expression representing the unary plus of the given expression
 */
template <typename E>
auto operator+(E&& value) -> detail::unary_helper<E, plus_unary_op> {
    return detail::unary_helper<E, plus_unary_op>{value};
}

/*!
 * \brief Apply absolute on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the absolute of each value of the given expression
 */
template <typename E>
auto abs(E&& value) -> detail::unary_helper<E, abs_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::abs can only be used on ETL expressions");
    return detail::unary_helper<E, abs_unary_op>{value};
}

/*!
 * \brief Apply max(x, v) on each element x of the ETL expression
 * \param value The ETL expression
 * \param v The maximum
 * \return an expression representing the max(x, v) of each value x of the given expression
 */
template <typename E, typename T, cpp_enable_if(std::is_arithmetic<T>::value)>
auto max(E&& value, T v) {
    static_assert(is_etl_expr<E>::value, "etl::max can only be used on ETL expressions");
    return detail::make_stateful_unary_expr<E, max_scalar_op<value_t<E>, value_t<E>>>(value, value_t<E>(v));
}

/*!
 * \brief Create an expression with the max value of lhs or rhs
 * \param lhs The left hand side ETL expression
 * \param rhs The right hand side ETL expression
 * \return an expression representing the max values from lhs and rhs
 */
template <typename L, typename R, cpp_disable_if(std::is_arithmetic<R>::value)>
auto max(L&& lhs, R&& rhs) -> detail::left_binary_helper_op<L, R, max_binary_op<value_t<L>, value_t<R>>> {
    static_assert(is_etl_expr<L>::value, "etl::max can only be used on ETL expressions");
    return {lhs, rhs};
}

/*!
 * \brief Apply min(x, v) on each element x of the ETL expression
 * \param value The ETL expression
 * \param v The minimum
 * \return an expression representing the min(x, v) of each value x of the given expression
 */
template <typename E, typename T, cpp_enable_if(std::is_arithmetic<T>::value)>
auto min(E&& value, T v) {
    static_assert(is_etl_expr<E>::value, "etl::min can only be used on ETL expressions");
    return detail::make_stateful_unary_expr<E, min_scalar_op<value_t<E>, value_t<E>>>(value, value_t<E>(v));
}

/*!
 * \brief Create an expression with the min value of lhs or rhs
 * \param lhs The left hand side ETL expression
 * \param rhs The right hand side ETL expression
 * \return an expression representing the min values from lhs and rhs
 */
template <typename L, typename R, cpp_disable_if(std::is_arithmetic<R>::value)>
auto min(L&& lhs, R&& rhs) -> detail::left_binary_helper_op<L, R, min_binary_op<value_t<L>, value_t<R>>> {
    static_assert(is_etl_expr<L>::value, "etl::max can only be used on ETL expressions");
    return {lhs, rhs};
}

/*!
 * \brief Round down each values of the ETL expression
 * \param value The ETL expression
 * \return an expression representing the values of the ETL expression rounded down.
 */
template <typename E>
auto floor(E&& value) {
    static_assert(is_etl_expr<E>::value, "etl::floor can only be used on ETL expressions");
    return detail::unary_helper<E, floor_unary_op>{value};
}

/*!
 * \brief Round up each values of the ETL expression
 * \param value The ETL expression
 * \return an expression representing the values of the ETL expression rounded up.
 */
template <typename E>
auto ceil(E&& value) {
    static_assert(is_etl_expr<E>::value, "etl::ceil can only be used on ETL expressions");
    return detail::unary_helper<E, ceil_unary_op>{value};
}

/*!
 * \brief Clip each values of the ETL expression between min and max
 * \param value The ETL expression
 * \param min The minimum
 * \param max The maximum
 * \return an expression representing the values of the ETL expression clipped between min and max
 */
template <typename E, typename T>
auto clip(E&& value, T min, T max) {
    static_assert(is_etl_expr<E>::value, "etl::clip can only be used on ETL expressions");
    static_assert(std::is_arithmetic<T>::value, "etl::clip can only be used with arithmetic values");
    return detail::make_stateful_unary_expr<E, clip_scalar_op<value_t<E>, value_t<E>>>(value, value_t<E>(min), value_t<E>(max));
}

/*!
 * \brief Apply pow(x, v) on each element x of the ETL expression
 * \param value The ETL expression
 * \param v The power
 * \return an expression representing the pow(x, v) of each value x of the given expression
 */
template <typename E, typename T>
auto pow(E&& value, T v) -> detail::left_binary_helper_op<E, scalar<value_t<E>>, pow_binary_op<value_t<E>, value_t<E>>> {
    static_assert(is_etl_expr<E>::value, "etl::pow can only be used on ETL expressions");
    static_assert(std::is_arithmetic<T>::value, "etl::pow can only be used with arithmetic values");
    return {value, scalar<value_t<E>>(v)};
}

/*!
 * \brief Creates an expression with values of 1 where the ETL expression has a value of v
 * \param value The ETL expression
 * \param v The value to test
 * \return an expression representing the values of 1 where the ETL expression has a value of v
 */
template <typename E, typename T>
auto one_if(E&& value, T v) -> detail::left_binary_helper_op<E, scalar<value_t<E>>, one_if_binary_op<value_t<E>, value_t<E>>> {
    static_assert(is_etl_expr<E>::value, "etl::one_if can only be used on ETL expressions");
    static_assert(std::is_arithmetic<T>::value, "etl::one_if can only be used with arithmetic values");
    return {value, scalar<value_t<E>>(v)};
}

/*!
 * \brief Creates an expression with a value of 1 where the max value is and all zeroes other places
 * \param value The ETL expression
 * \return an expression with a value of 1 where the max value is and all zeroes other places
 */
template <typename E>
auto one_if_max(E&& value) -> detail::left_binary_helper_op<E, scalar<value_t<E>>, one_if_binary_op<value_t<E>, value_t<E>>> {
    static_assert(is_etl_expr<E>::value, "etl::one_if_max can only be used on ETL expressions");
    return {value, scalar<value_t<E>>(max(value))};
}

/*!
 * \brief Apply square root on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the square root of each value of the given expression
 */
template <typename E>
auto sqrt(E&& value) -> detail::unary_helper<E, sqrt_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::sqrt can only be used on ETL expressions");
    return detail::unary_helper<E, sqrt_unary_op>{value};
}

/*!
 * \brief Apply inverse square root on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the inverse square root of each value of the given expression
 */
template <typename E>
auto invsqrt(E&& value) -> detail::unary_helper<E, invsqrt_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::invsqrt can only be used on ETL expressions");
    return detail::unary_helper<E, invsqrt_unary_op>{value};
}

/*!
 * \brief Apply cubic root on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the cubic root of each value of the given expression
 */
template <typename E>
auto cbrt(E&& value) -> detail::unary_helper<E, cbrt_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::cbrt can only be used on ETL expressions");
    return detail::unary_helper<E, cbrt_unary_op>{value};
}

/*!
 * \brief Apply inverse cubic root on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the inverse cubic root of each value of the given expression
 */
template <typename E>
auto invcbrt(E&& value) -> detail::unary_helper<E, invcbrt_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::invcbrt can only be used on ETL expressions");
    return detail::unary_helper<E, invcbrt_unary_op>{value};
}

/*!
 * \brief Apply logarithm on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the logarithm of each value of the given expression
 */
template <typename E>
auto log(E&& value) -> detail::unary_helper<E, log_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::log can only be used on ETL expressions");
    return detail::unary_helper<E, log_unary_op>{value};
}

/*!
 * \brief Apply tangent on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the tangent of each value of the given expression
 */
template <typename E>
auto tan(E&& value) -> detail::unary_helper<E, tan_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::tan can only be used on ETL expressions");
    return detail::unary_helper<E, tan_unary_op>{value};
}

/*!
 * \brief Apply cosinus on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the cosinus of each value of the given expression
 */
template <typename E>
auto cos(E&& value) -> detail::unary_helper<E, cos_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::cos can only be used on ETL expressions");
    return detail::unary_helper<E, cos_unary_op>{value};
}

/*!
 * \brief Apply sinus on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the sinus of each value of the given expression
 */
template <typename E>
auto sin(E&& value) -> detail::unary_helper<E, sin_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::sin can only be used on ETL expressions");
    return detail::unary_helper<E, sin_unary_op>{value};
}

/*!
 * \brief Apply hyperbolic tangent on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the hyperbolic tangent of each value of the given expression
 */
template <typename E>
auto tanh(E&& value) -> detail::unary_helper<E, tanh_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::tanh can only be used on ETL expressions");
    return detail::unary_helper<E, tanh_unary_op>{value};
}

/*!
 * \brief Apply hyperbolic cosinus on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the hyperbolic cosinus of each value of the given expression
 */
template <typename E>
auto cosh(E&& value) -> detail::unary_helper<E, cosh_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::cosh can only be used on ETL expressions");
    return detail::unary_helper<E, cosh_unary_op>{value};
}

/*!
 * \brief Apply hyperbolic sinus on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the hyperbolic sinus of each value of the given expression
 */
template <typename E>
auto sinh(E&& value) -> detail::unary_helper<E, sinh_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::sinh can only be used on ETL expressions");
    return detail::unary_helper<E, sinh_unary_op>{value};
}

/*!
 * \brief Extract the real part of each complex value of the given expression
 * \param value The ETL expression
 * \return an expression representing the real part of each complex of the given expression
 */
template <typename E>
auto real(E&& value) -> unary_expr<typename value_t<E>::value_type, detail::build_type<E>, real_unary_op<value_t<E>>> {
    static_assert(is_etl_expr<E>::value, "etl::real can only be used on ETL expressions");
    static_assert(is_complex_t<value_t<E>>::value, "etl::real can only be used on ETL expressions containing complex numbers");
    return unary_expr<typename value_t<E>::value_type, detail::build_type<E>, real_unary_op<value_t<E>>>{value};
}

/*!
 * \brief Extract the imag part of each complex value of the given expression
 * \param value The ETL expression
 * \return an expression representing the imag part of each complex of the given expression
 */
template <typename E>
auto imag(E&& value) -> unary_expr<typename value_t<E>::value_type, detail::build_type<E>, imag_unary_op<value_t<E>>> {
    static_assert(is_etl_expr<E>::value, "etl::imag can only be used on ETL expressions");
    static_assert(is_complex_t<value_t<E>>::value, "etl::imag can only be used on ETL expressions containing complex numbers");
    return unary_expr<typename value_t<E>::value_type, detail::build_type<E>, imag_unary_op<value_t<E>>>{value};
}

/*!
 * \brief Apply the conjugate operation on each complex value of the given expression
 * \param value The ETL expression
 * \return an expression representing the the conjugate operation of each complex of the given expression
 */
template <typename E>
auto conj(E&& value) -> unary_expr<value_t<E>, detail::build_type<E>, conj_unary_op<value_t<E>>> {
    static_assert(is_etl_expr<E>::value, "etl::conj can only be used on ETL expressions");
    static_assert(is_complex_t<value_t<E>>::value, "etl::conj can only be used on ETL expressions containing complex numbers");
    return unary_expr<value_t<E>, detail::build_type<E>, conj_unary_op<value_t<E>>>{value};
}

/*!
 * \brief Add some uniform noise (0, 1.0) to the given expression
 * \param value The input ETL expression
 * \return an expression representing the input expression plus noise
 */
template <typename E>
auto uniform_noise(E&& value) -> detail::unary_helper<E, uniform_noise_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::uniform_noise can only be used on ETL expressions");
    return detail::unary_helper<E, uniform_noise_unary_op>{value};
}

/*!
 * \brief Add some normal noise (0, 1.0) to the given expression
 * \param value The input ETL expression
 * \return an expression representing the input expression plus noise
 */
template <typename E>
auto normal_noise(E&& value) -> detail::unary_helper<E, normal_noise_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::normal_noise can only be used on ETL expressions");
    return detail::unary_helper<E, normal_noise_unary_op>{value};
}

/*!
 * \brief Add some normal noise (0, sigmoid(x)) to the given expression
 * \param value The input ETL expression
 * \return an expression representing the input expression plus noise
 */
template <typename E>
auto logistic_noise(E&& value) -> detail::unary_helper<E, logistic_noise_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::logistic_noise can only be used on ETL expressions");
    return detail::unary_helper<E, logistic_noise_unary_op>{value};
}

/*!
 * \brief Add some normal noise N(0,1) to x.
 * No noise is added to values equal to zero or to given the value.
 * \param value The value to add noise to
 * \param v The value for the upper range limit
 * \return An expression representing the left value plus the noise
 */
template <typename E, typename T>
auto ranged_noise(E&& value, T v) -> detail::left_binary_helper_op<E, scalar<value_t<E>>, ranged_noise_binary_op<value_t<E>, value_t<E>>> {
    static_assert(is_etl_expr<E>::value, "etl::ranged_noise can only be used on ETL expressions");
    static_assert(std::is_arithmetic<T>::value, "etl::ranged_noise can only be used with arithmetic values");
    return {value, scalar<value_t<E>>(v)};
}

/*!
 * \brief Apply exponential on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the exponential of each value of the given expression
 */
template <typename E>
auto exp(E&& value) -> detail::unary_helper<E, exp_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::exp can only be used on ETL expressions");
    return detail::unary_helper<E, exp_unary_op>{value};
}

/*!
 * \brief Apply sign on each value of the given expression
 * \param value The ETL expression
 * \return an expression representing the sign of each value of the given expression
 */
template <typename E>
auto sign(E&& value) -> detail::unary_helper<E, sign_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::sign can only be used on ETL expressions");
    return detail::unary_helper<E, sign_unary_op>{value};
}

/*!
 * \brief Performs the identiy function on the ETL expression.
 * \param value The ETL expression
 * \return The same value, perfectly forwardd
 */
template <typename E>
decltype(auto) identity(E&& value) {
    return std::forward<E>(value);
}

/*!
 * \brief Return the derivative of the identiy function for the given value.
 * \param value The ETL expression
 * \return 1.0
 */
template <typename E>
auto identity_derivative(E&& value) {
    cpp_unused(value);
    return 1.0;
}

//Note: Use of decltype here should not be necessary, but g++ does
//not like it without it for some reason

/*!
 * \brief Return the logistic sigmoid of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the logistic sigmoid of the input.
 */
template <typename E>
auto sigmoid(E&& value) -> decltype(1.0 / (1.0 + exp(-value))) {
    static_assert(is_etl_expr<E>::value, "etl::sigmoid can only be used on ETL expressions");
    return 1.0 / (1.0 + exp(-value));
}

/*!
 * \brief Return the derivative of the logistic sigmoid of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the derivative of the logistic sigmoid of the input.
 */
template <typename E>
auto sigmoid_derivative(E&& value) -> decltype(value >> (1.0 - value)) {
    static_assert(is_etl_expr<E>::value, "etl::sigmoid_derivative can only be used on ETL expressions");
    return value >> (1.0 - value);
}

/*!
 * \brief Return a fast approximation of the logistic sigmoid of the given ETL expression.
 *
 * This function is faster than the sigmoid function and has an acceptable precision.
 *
 * \param value The ETL expression
 * \return An ETL expression representing a fast approximation of the logistic sigmoid of the input.
 */
template <typename E>
auto fast_sigmoid(const E& value) -> detail::unary_helper<E, fast_sigmoid_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::fast_sigmoid can only be used on ETL expressions");
    return detail::unary_helper<E, fast_sigmoid_unary_op>{value};
}

/*!
 * \brief Return an hard approximation of the logistic sigmoid of the given ETL expression.
 *
 * This function is much faster than the sigmoid, but it's precision is very low.
 *
 * \param x The ETL expression
 * \return An ETL expression representing an hard approximation of the logistic sigmoid of the input.
 */
template <typename E>
auto hard_sigmoid(E&& x) -> decltype(etl::clip(x * 0.2 + 0.5, 0.0, 1.0)) {
    static_assert(is_etl_expr<E>::value, "etl::hard_sigmoid can only be used on ETL expressions");
    return etl::clip(x * 0.2 + 0.5, 0.0, 1.0);
}

/*!
 * \brief Return the softmax function of the given ETL expression.
 * \param e The ETL expression
 * \return An ETL expression representing the softmax function of the input.
 */
template <typename E>
auto softmax(E&& e) {
    static_assert(is_etl_expr<E>::value, "etl::softmax can only be used on ETL expressions");
    return exp(e) / sum(exp(e));
}

/*!
 * \brief Returns the softmax function of the given ETL expression.
 * This version is implemented so that numerical stability is preserved.
 * \param e The ETL expression
 * \return An ETL expression representing the softmax function of the input.
 */
template <typename E>
auto stable_softmax(E&& e) {
    static_assert(is_etl_expr<E>::value, "etl::stable_softmax can only be used on ETL expressions");
    auto m = max(e);
    return exp(e - m) / sum(exp(e - m));
}

/*!
 * \brief Return the derivative of the softmax function of the given ETL expression.
 * \param e The ETL expression
 * \return An ETL expression representing the derivative of the softmax function of the input.
 */
template <typename E>
auto softmax_derivative(E&& e) {
    cpp_unused(e);
    return 1.0;
}

/*!
 * \brief Return the softplus of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the softplus of the input.
 */
template <typename E>
auto softplus(E&& value) -> decltype(log(1.0 + exp(value))) {
    static_assert(is_etl_expr<E>::value, "etl::softplus can only be used on ETL expressions");
    return log(1.0 + exp(value));
}

/*!
 * \brief Apply Bernoulli sampling to the values of the expression
 * \param value the expression to sample
 * \return an expression representing the Bernoulli sampling of the given expression
 */
template <typename E>
auto bernoulli(const E& value) -> detail::unary_helper<E, bernoulli_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::bernoulli can only be used on ETL expressions");
    return detail::unary_helper<E, bernoulli_unary_op>{value};
}

/*!
 * \brief Apply Reverse Bernoulli sampling to the values of the expression
 * \param value the expression to sample
 * \return an expression representing the Reverse Bernoulli sampling of the given expression
 */
template <typename E>
auto r_bernoulli(const E& value) -> detail::unary_helper<E, reverse_bernoulli_unary_op> {
    static_assert(is_etl_expr<E>::value, "etl::r_bernoulli can only be used on ETL expressions");
    return detail::unary_helper<E, reverse_bernoulli_unary_op>{value};
}

/*!
 * \brief Return the derivative of the tanh function of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the derivative of the tanh function of the input.
 */
template <typename E>
auto tanh_derivative(E&& value) -> decltype(1.0 - (value >> value)) {
    static_assert(is_etl_expr<E>::value, "etl::tanh_derivative can only be used on ETL expressions");
    return 1.0 - (value >> value);
}

/*!
 * \brief Return the relu activation of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the relu activation of the input.
 */
template <typename E>
auto relu(E&& value) -> decltype(max(value, 0.0)) {
    static_assert(is_etl_expr<E>::value, "etl::relu can only be used on ETL expressions");
    return max(value, 0.0);
}

/*!
 * \brief Return the derivative of the relu function of the given ETL expression.
 * \param value The ETL expression
 * \return An ETL expression representing the derivative of the relu function of the input.
 */
template <typename E>
auto relu_derivative(const E& value) -> detail::unary_helper<E, relu_derivative_op> {
    static_assert(is_etl_expr<E>::value, "etl::relu_derivative can only be used on ETL expressions");
    return detail::unary_helper<E, relu_derivative_op>{value};
}

/*!
 * \brief Return a view representing the ith Dth dimension.
 * \param i The index to consider in the view
 * \tparam D The dimension to consider
 * \return a view representing the ith Dth dimension.
 */
template <size_t D, typename E>
auto dim(E&& value, size_t i) -> detail::identity_helper<E, dim_view<detail::build_identity_type<E>, D>> {
    static_assert(is_etl_expr<E>::value, "etl::dim can only be used on ETL expressions");
    return detail::identity_helper<E, dim_view<detail::build_identity_type<E>, D>>{{value, i}};
}

/*!
 * \brief Returns view representing the ith row of the given expression
 * \param value The ETL expression
 * \param i The row index
 * \return a view expression representing the ith row of the given expression
 */
template <typename E>
auto row(E&& value, size_t i) -> detail::identity_helper<E, dim_view<detail::build_identity_type<E>, 1>> {
    static_assert(is_etl_expr<E>::value, "etl::row can only be used on ETL expressions");
    return detail::identity_helper<E, dim_view<detail::build_identity_type<E>, 1>>{{value, i}};
}

/*!
 * \brief Returns view representing the ith column of the given expression
 * \param value The ETL expression
 * \param i The column index
 * \return a view expression representing the ith column of the given expression
 */
template <typename E>
auto col(E&& value, size_t i) -> detail::identity_helper<E, dim_view<detail::build_identity_type<E>, 2>> {
    static_assert(is_etl_expr<E>::value, "etl::col can only be used on ETL expressions");
    return detail::identity_helper<E, dim_view<detail::build_identity_type<E>, 2>>{{value, i}};
}

/*!
 * \brief Returns view representing a sub dimensional view of the given expression.
 * \param value The ETL expression
 * \param i The first index
 * \return a view expression representing a sub dimensional view of the given expression
 */
template <typename E>
auto sub(E&& value, size_t i) -> sub_view<detail::build_identity_type<E>, false> {
    static_assert(is_etl_expr<E>::value, "etl::sub can only be used on ETL expressions");
    static_assert(etl_traits<std::decay_t<E>>::dimensions() > 1, "Cannot use sub on vector");
    return {value, i};
}

/*!
 * \brief Returns view representing a sub matirx view of the given expression.
 * \param value The ETL expression
 * \param i The first index
 * \param j The second index
 * \param m The first dimension
 * \param n The second dimension
 * \return a view expression representing a sub matrix view of the given expression
 */
template <typename E>
auto sub(E&& value, size_t i, size_t j, size_t m, size_t n) -> sub_matrix_2d<detail::build_identity_type<E>, false> {
    static_assert(is_etl_expr<E>::value, "etl::sub can only be used on ETL expressions");
    static_assert(etl_traits<std::decay_t<E>>::dimensions() == 2, "Cannot use sub on vector");
    return {value, i, j, m, n};
}

/*!
 * \brief Returns view representing a slice view of the given expression.
 * \param value The ETL expression
 * \param first The first index
 * \param last The last index
 * \return a view expression representing a sub dimensional view of the given expression
 */
template <typename E>
auto slice(E&& value, size_t first, size_t last) -> slice_view<detail::build_identity_type<E>> {
    static_assert(is_etl_expr<E>::value, "etl::slice can only be used on ETL expressions");
    return {value, first, last};
}

/*!
 * \brief Returns view representing the reshape of another expression
 * \param value The ETL expression
 * \tparam Dims the reshape dimensions
 * \return a view expression representing the same expression with a different shape
 */
template <size_t... Dims, typename E>
auto reshape(E&& value) -> fast_matrix_view<detail::build_identity_type<E>, all_dma<E>::value, Dims...> {
    static_assert(is_etl_expr<E>::value, "etl::reshape can only be used on ETL expressions");
    cpp_assert(decay_traits<E>::is_generator || size(value) == mul_all<Dims...>::value, "Invalid size for reshape");

    return fast_matrix_view<detail::build_identity_type<E>, all_dma<E>::value, Dims...>{value};
}

/*!
 * \brief Returns view representing the reshape of another expression
 * \param value The ETL expression
 * \param sizes The dimensions of the reshaped expression
 * \return a view expression representing the same expression with a different shape
 */
template <typename E, typename... S>
auto reshape(E&& value, S... sizes) -> dyn_matrix_view<detail::build_identity_type<E>, sizeof...(sizes)> {
    static_assert(is_etl_expr<E>::value, "etl::reshape can only be used on ETL expressions");

    using ret_type = dyn_matrix_view<detail::build_identity_type<E>, sizeof...(sizes)>;

    //TODO This is dum... comput the size directly
    cpp_assert(decay_traits<E>::is_generator || size(value) == size(ret_type{value, size_t(sizes)...}), "Invalid size for reshape");

    return ret_type{value, size_t(sizes)...};
}

// Virtual Views that returns rvalues

/*!
 * \brief Returns a view representing the square magic matrix
 * \param i The size of the matrix (one side)
 * \return a virtual view expression representing the square magic matrix
 */
template <typename D = double>
auto magic(size_t i) -> detail::virtual_helper<D, magic_view<D>> {
    return detail::virtual_helper<D, magic_view<D>>{magic_view<D>{i}};
}

/*!
 * \brief Returns a view representing the square magic matrix
 * \tparam N The size of the matrix (one side)
 * \return a virtual view expression representing the square magic matrix
 */
template <size_t N, typename D = double>
auto magic() -> detail::virtual_helper<D, fast_magic_view<D, N>> {
    return detail::virtual_helper<D, fast_magic_view<D, N>>{{}};
}

// Apply a stable transformation

/*!
 * \brief Repeats the expression to the right (adds dimension after existing)
 * \param value The expression to repeat
 * \tparam D1 The first repeat
 * \tparam D The remaining repeated dimensions
 * \return an expression representing the repeated expression
 */
template <size_t D1, size_t... D, typename E>
auto rep(E&& value) -> unary_expr<value_t<E>, rep_r_transformer<detail::build_type<E>, D1, D...>, transform_op> {
    static_assert(is_etl_expr<E>::value, "etl::rep can only be used on ETL expressions");
    return unary_expr<value_t<E>, rep_r_transformer<detail::build_type<E>, D1, D...>, transform_op>{rep_r_transformer<detail::build_type<E>, D1, D...>(value)};
}

/*!
 * \brief Repeats the expression to the right (adds dimension after existing)
 * \param value The expression to repeat
 * \tparam D1 The first repeat
 * \tparam D The remaining repeated dimensions
 * \return an expression representing the repeated expression
 */
template <size_t D1, size_t... D, typename E>
auto rep_r(E&& value) -> unary_expr<value_t<E>, rep_r_transformer<detail::build_type<E>, D1, D...>, transform_op> {
    static_assert(is_etl_expr<E>::value, "etl::rep_r can only be used on ETL expressions");
    return unary_expr<value_t<E>, rep_r_transformer<detail::build_type<E>, D1, D...>, transform_op>{rep_r_transformer<detail::build_type<E>, D1, D...>(value)};
}

/*!
 * \brief Repeats the expression to the left (adds dimension before existing)
 * \param value The expression to repeat
 * \tparam D1 The first repeat
 * \tparam D The remaining repeated dimensions
 * \return an expression representing the repeated expression
 */
template <size_t D1, size_t... D, typename E>
auto rep_l(E&& value) -> unary_expr<value_t<E>, rep_l_transformer<detail::build_type<E>, D1, D...>, transform_op> {
    static_assert(is_etl_expr<E>::value, "etl::rep_l can only be used on ETL expressions");
    return unary_expr<value_t<E>, rep_l_transformer<detail::build_type<E>, D1, D...>, transform_op>{rep_l_transformer<detail::build_type<E>, D1, D...>(value)};
}

/*!
 * \brief Repeats the expression to the right (adds dimension after existing)
 * \param value The expression to repeat
 * \param d1 The first repeat
 * \param d The remaining repeated dimensions
 * \return an expression representing the repeated expression
 */
template <typename... D, typename E>
auto rep(E&& value, size_t d1, D... d) -> unary_expr<value_t<E>, dyn_rep_r_transformer<detail::build_type<E>, 1 + sizeof...(D)>, transform_op> {
    static_assert(is_etl_expr<E>::value, "etl::rep can only be used on ETL expressions");
    return unary_expr<value_t<E>, dyn_rep_r_transformer<detail::build_type<E>, 1 + sizeof...(D)>, transform_op>{
        dyn_rep_r_transformer<detail::build_type<E>, 1 + sizeof...(D)>(value, {{d1, static_cast<size_t>(d)...}})};
}

/*!
 * \brief Repeats the expression to the right (adds dimension after existing)
 * \param value The expression to repeat
 * \param d1 The first repeat
 * \param d The remaining repeated dimensions
 * \return an expression representing the repeated expression
 */
template <typename... D, typename E>
auto rep_r(E&& value, size_t d1, D... d) -> unary_expr<value_t<E>, dyn_rep_r_transformer<detail::build_type<E>, 1 + sizeof...(D)>, transform_op> {
    static_assert(is_etl_expr<E>::value, "etl::rep_r can only be used on ETL expressions");
    return unary_expr<value_t<E>, dyn_rep_r_transformer<detail::build_type<E>, 1 + sizeof...(D)>, transform_op>{
        dyn_rep_r_transformer<detail::build_type<E>, 1 + sizeof...(D)>(value, {{d1, static_cast<size_t>(d)...}})};
}

/*!
 * \brief Repeats the expression to the left (adds dimension after existing)
 * \param value The expression to repeat
 * \param d1 The first repeat
 * \param d The remaining repeated dimensions
 * \return an expression representing the repeated expression
 */
template <typename... D, typename E>
auto rep_l(E&& value, size_t d1, D... d) -> unary_expr<value_t<E>, dyn_rep_l_transformer<detail::build_type<E>, 1 + sizeof...(D)>, transform_op> {
    static_assert(is_etl_expr<E>::value, "etl::rep_l can only be used on ETL expressions");
    return unary_expr<value_t<E>, dyn_rep_l_transformer<detail::build_type<E>, 1 + sizeof...(D)>, transform_op>{
        dyn_rep_l_transformer<detail::build_type<E>, 1 + sizeof...(D)>(value, {{d1, static_cast<size_t>(d)...}})};
}

/*!
 * \brief Returns the indices of the maximum values in the first axis of the
 * given matrix
 * \param value The value to aggregate
 * \return an expression representing the aggregated expression
 */
template <typename E>
auto argmax(E&& value) -> detail::stable_transform_helper<E, argmax_transformer> {
    static_assert(is_etl_expr<E>::value, "etl::argmax can only be used on ETL expressions");
    static_assert(decay_traits<E>::dimensions() > 1, "Can only use argmax on matrix");
    return detail::make_transform_expr<E, argmax_transformer>(value);
}

/*!
 * \brief Returns the indices of the minimum values in the first axis of the
 * given matrix
 * \param value The value to aggregate
 * \return an expression representing the aggregated expression
 */
template <typename E>
auto argmin(E&& value) -> detail::stable_transform_helper<E, argmin_transformer> {
    static_assert(is_etl_expr<E>::value, "etl::argmax can only be used on ETL expressions");
    static_assert(decay_traits<E>::dimensions() > 1, "Can only use argmax on matrix");
    return detail::make_transform_expr<E, argmin_transformer>(value);
}

/*!
 * \brief Aggregate (sum) a dimension from the right. This effectively removes
 * the last dimension from the expression and sums its values to the left.
 * the last dimension from the expression and sums its values to the left.
 * \param value The value to aggregate
 * \return an expression representing the aggregated expression
 */
template <typename E>
auto sum_r(E&& value) -> detail::stable_transform_helper<E, sum_r_transformer> {
    static_assert(is_etl_expr<E>::value, "etl::sum_r can only be used on ETL expressions");
    static_assert(decay_traits<E>::dimensions() > 1, "Can only use sum_r on matrix");
    return detail::make_transform_expr<E, sum_r_transformer>(value);
}

/*!
 * \brief Aggregate (sum) a dimension from the left. This effectively removes
 * the first dimension from the expression and sums its values to the right.
 * \param value The value to aggregate
 * \return an expression representing the aggregated expression
 */
template <typename E>
auto sum_l(E&& value) -> detail::stable_transform_helper<E, sum_l_transformer> {
    static_assert(is_etl_expr<E>::value, "etl::sum_l can only be used on ETL expressions");
    static_assert(decay_traits<E>::dimensions() > 1, "Can only use sum_l on matrix");
    return detail::make_transform_expr<E, sum_l_transformer>(value);
}

/*!
 * \brief Aggregate (average) a dimension from the right. This effectively removes
 * the last dimension from the expression and averages its values to the left.
 * \param value The value to aggregate
 * \return an expression representing the aggregated expression
 */
template <typename E>
auto mean_r(E&& value) -> detail::stable_transform_helper<E, mean_r_transformer> {
    static_assert(is_etl_expr<E>::value, "etl::mean_r can only be used on ETL expressions");
    static_assert(decay_traits<E>::dimensions() > 1, "Can only use mean_r on matrix");
    return detail::make_transform_expr<E, mean_r_transformer>(value);
}

/*!
 * \brief Aggregate (average) a dimension from the left. This effectively removes
 * the first dimension from the expression and averages its values to the right.
 * \param value The value to aggregate
 * \return an expression representing the aggregated expression
 */
template <typename E>
auto mean_l(E&& value) -> detail::stable_transform_helper<E, mean_l_transformer> {
    static_assert(is_etl_expr<E>::value, "etl::mean_l can only be used on ETL expressions");
    static_assert(decay_traits<E>::dimensions() > 1, "Can only use mean_l on matrix");
    return detail::make_transform_expr<E, mean_l_transformer>(value);
}

/*!
 * \brief Returns the horizontal flipping of the given expression.
 * \param value The expression
 * \return The horizontal flipping of the given expression.
 */
template <typename E>
auto hflip(const E& value) -> detail::stable_transform_helper<E, hflip_transformer> {
    static_assert(is_etl_expr<E>::value, "etl::hflip can only be used on ETL expressions");
    static_assert(etl_traits<std::decay_t<E>>::dimensions() <= 2, "Can only use flips on 1D/2D");
    return detail::make_transform_expr<E, hflip_transformer>(value);
}

/*!
 * \brief Returns the vertical flipping of the given expression.
 * \param value The expression
 * \return The vertical flipping of the given expression.
 */
template <typename E>
auto vflip(const E& value) -> detail::stable_transform_helper<E, vflip_transformer> {
    static_assert(is_etl_expr<E>::value, "etl::vflip can only be used on ETL expressions");
    static_assert(etl_traits<std::decay_t<E>>::dimensions() <= 2, "Can only use flips on 1D/2D");
    return detail::make_transform_expr<E, vflip_transformer>(value);
}

/*!
 * \brief Returns the horizontal and vertical flipping of the given expression.
 * \param value The expression
 * \return The horizontal and vertical flipping of the given expression.
 */
template <typename E>
auto fflip(const E& value) -> detail::stable_transform_helper<E, fflip_transformer> {
    static_assert(is_etl_expr<E>::value, "etl::fflip can only be used on ETL expressions");
    static_assert(etl_traits<std::decay_t<E>>::dimensions() <= 2, "Can only use flips on 1D/2D");
    return detail::make_transform_expr<E, fflip_transformer>(value);
}

/*!
 * \brief Returns the transpose of the given expression.
 * \param value The expression
 * \return The transpose of the given expression.
 */
template <typename E>
auto transpose(const E& value) -> transpose_expr<detail::build_type<E>> {
    static_assert(is_etl_expr<E>::value, "etl::transpose can only be used on ETL expressions");
    static_assert(decay_traits<E>::dimensions() <= 2, "Transpose not defined for matrix > 2D");
    return transpose_expr<detail::build_type<E>>{value};
}

/*!
 * \brief Returns the transpose of the given expression.
 * \param value The expression
 * \return The transpose of the given expression.
 */
template <typename E>
auto trans(const E& value) {
    return transpose(value);
}

/*!
 * \brief Returns the conjugate transpose of the given expression.
 * \param value The expression
 * \return The conjugate transpose of the given expression.
 */
template <typename E>
auto conj_transpose(const E& value) {
    return conj(transpose(value));
}

/*!
 * \brief Returns the conjugate transpose of the given expression.
 * \param value The expression
 * \return The conjugate transpose of the given expression.
 */
template <typename E>
auto ctrans(const E& value) {
    return conj(transpose(value));
}

/*!
 * \brief Returns euclidean norm of the given expression.
 * \param a The expression
 * \return The euclidean norm of the expression
 */
template <typename A>
value_t<A> norm(const A& a) {
    return detail::norm_impl::apply(a);
}

// TODO: Ideally we would like to be able to use a.dot(b) and
// a.cross(b). Unfortunately, this would require one of these options:
// * A new CRTP class, increasing the compilation time overhead
// * Adding the dot template function in each expression
// Note: Need Unified Call Syntax :(

/*!
 * \brief Returns the dot product of the two given expressions.
 * \param a The left expression
 * \param b The right expression
 * \return The dot product of the two expressions
 */
template <typename A, typename B>
value_t<A> dot(const A& a, const B& b) {
    validate_expression(a, b);
    return detail::dot_impl::apply(a, b);
}

/*!
 * \brief Returns the dot product of the two given expressions.
 * \param a The left expression
 * \param b The right expression
 * \return The dot product of the two expressions
 */
template <typename A, typename B, cpp_enable_if_cst(all_fast<A, B>::value)>
etl::fast_vector<value_t<A>, 3> cross(const A& a, const B& b) {
    static_assert(etl::decay_traits<A>::dimensions() == 1, "Cross product is only valid for 1D vectors");
    static_assert(etl::decay_traits<B>::dimensions() == 1, "Cross product is only valid for 1D vectors");
    static_assert(etl::decay_traits<A>::size() == 3, "Cross product is only valid for 1D vectors of size 3");
    static_assert(etl::decay_traits<B>::size() == 3, "Cross product is only valid for 1D vectors of size 3");

    return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
}

/*!
 * \brief Returns the dot product of the two given expressions.
 * \param a The left expression
 * \param b The right expression
 * \return The dot product of the two expressions
 */
template <typename A, typename B, cpp_disable_if_cst(all_fast<A, B>::value)>
etl::dyn_vector<value_t<A>> cross(const A& a, const B& b) {
    static_assert(etl::decay_traits<A>::dimensions() == 1, "Cross product is only valid for 1D vectors");
    static_assert(etl::decay_traits<B>::dimensions() == 1, "Cross product is only valid for 1D vectors");

    cpp_assert(etl::decay_traits<A>::size(a) == 3, "Cross product is only valid for 1D vectors of size 3");
    cpp_assert(etl::decay_traits<B>::size(b) == 3, "Cross product is only valid for 1D vectors of size 3");

    return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0]};
}

/*!
 * \brief Returns the sum of all the values contained in the given expression
 * \param values The expression to reduce
 * \return The sum of the values of the expression
 */
template <typename E>
value_t<E> sum(E&& values) {
    static_assert(is_etl_expr<E>::value, "etl::sum can only be used on ETL expressions");

    //Reduction force evaluation
    force(values);

    return detail::sum_impl::apply(values);
}

/*!
 * \brief Returns the sum of all the absolute values contained in the given expression
 * \param values The expression to reduce
 * \return The sum of the absolute values of the expression
 */
template <typename E>
value_t<E> asum(E&& values) {
    static_assert(is_etl_expr<E>::value, "etl::asum can only be used on ETL expressions");

    //Reduction force evaluation
    force(values);

    return detail::asum_impl::apply(values);
}

/*!
 * \brief Returns the mean of all the values contained in the given expression
 * \param values The expression to reduce
 * \return The mean of the values of the expression
 */
template <typename E>
value_t<E> mean(E&& values) {
    static_assert(is_etl_expr<E>::value, "etl::mean can only be used on ETL expressions");

    return sum(values) / size(values);
}

/*!
 * \brief Returns the mean of all the absolute values contained in the given expression
 * \param values The expression to reduce
 * \return The mean of the absolute values of the expression
 */
template <typename E>
value_t<E> amean(E&& values) {
    static_assert(is_etl_expr<E>::value, "etl::amean can only be used on ETL expressions");

    return asum(values) / size(values);
}

/*!
 * \brief Returns the standard deviation of all the values contained in the given expression
 * \param values The expression to reduce
 * \return The standard deviation of the values of the expression
 */
template <typename E>
value_t<E> stddev(E&& values) {
    static_assert(is_etl_expr<E>::value, "etl::stddev can only be used on ETL expressions");

    auto mean = etl::mean(values);

    double std = 0.0;
    for (auto value : values) {
        std += (value - mean) * (value - mean);
    }

    return std::sqrt(std / etl::size(values));
}

namespace detail {

/*!
 * \brief Helper to compute the return type for max/min operation
 */
template <typename E>
using value_return_t =std::conditional_t<
            decay_traits<E>::is_value,
            std::conditional_t<
                std::is_lvalue_reference<E>::value,
                std::conditional_t<
                    std::is_const<std::remove_reference_t<E>>::value,
                    const value_t<E>&,
                    value_t<E>&>,
                value_t<E>>,
            value_t<E>>;

} //end of namespace detail

/*!
 * \brief Returns the index of the maximum element contained in the expression
 * \param values The expression to search
 * \return The index of the maximum element of the expression
 */
template <typename E>
size_t max_index(E&& values) {
    static_assert(is_etl_expr<E>::value, "etl::max can only be used on ETL expressions");

    //Reduction force evaluation
    force(values);

    size_t m = 0;

    for (size_t i = 1; i < size(values); ++i) {
        if (values[i] > values[m]) {
            m = i;
        }
    }

    return m;
}

/*!
 * \brief Returns the maximum element contained in the expression
 * When possible, this returns a reference to the element.
 * \param values The expression to search
 * \return The maximum element of the expression
 */
template <typename E>
detail::value_return_t<E> max(E&& values) {
    static_assert(is_etl_expr<E>::value, "etl::max can only be used on ETL expressions");

    auto m = max_index(values);
    return values[m];
}

/*!
 * \brief Returns the index of the minimum element contained in the expression
 * \param values The expression to search
 * \return The index of the minimum element of the expression
 */
template <typename E>
size_t min_index(E&& values) {
    static_assert(is_etl_expr<E>::value, "etl::min can only be used on ETL expressions");

    //Reduction force evaluation
    force(values);

    size_t m = 0;

    for (size_t i = 1; i < size(values); ++i) {
        if (values[i] < values[m]) {
            m = i;
        }
    }

    return m;
}

/*!
 * \brief Returns the minimum element contained in the expression
 * When possible, this returns a reference to the element.
 * \param values The expression to search
 * \return The minimum element of the expression
 */
template <typename E>
detail::value_return_t<E> min(E&& values) {
    static_assert(is_etl_expr<E>::value, "etl::min can only be used on ETL expressions");

    auto m = min_index(values);
    return values[m];
}

// Generate data

/*!
 * \brief Create an expression generating numbers from a normal distribution
 * \param mean The mean of the distribution
 * \param stddev The standard deviation of the distribution
 * \return An expression generating numbers from the normal distribution
 */
template <typename T = double>
auto normal_generator(T mean = 0.0, T stddev = 1.0) -> generator_expr<normal_generator_op<T>> {
    return generator_expr<normal_generator_op<T>>{mean, stddev};
}

/*!
 * \brief Create an expression generating numbers from an uniform distribution
 * \param start The beginning of the range
 * \param end The end of the range
 * \return An expression generating numbers from the uniform distribution
 */
template <typename T = double>
auto uniform_generator(T start, T end) -> generator_expr<uniform_generator_op<T>> {
    return generator_expr<uniform_generator_op<T>>{start, end};
}

/*!
 * \brief Create an expression generating numbers from a consecutive sequence
 * \param current The first number to generate
 * \return an expression generating numbers from a consecutive sequence
 */
template <typename T = double>
auto sequence_generator(T current = 0) -> generator_expr<sequence_generator_op<T>> {
    return generator_expr<sequence_generator_op<T>>{current};
}

/*!
 * \brief Create an optimized expression wrapping the given expression.
 *
 * The expression will be optimized before being evaluated.
 * \param expr The expression to be wrapped
 * \return an optimized expression wrapping the given expression
 */
template <typename Expr>
auto opt(Expr&& expr) -> optimized_expr<detail::build_type<Expr>> {
    return optimized_expr<detail::build_type<Expr>>{expr};
}

/*!
 * \brief Create a timed expression wrapping the given expression.
 *
 * The evaluation (and assignment) of the expression will be timed.
 *
 * \param expr The expression to be wrapped
 * \return a timed expression wrapping the given expression
 */
template <typename Expr>
auto timed(Expr&& expr) -> timed_expr<detail::build_type<Expr>> {
    return timed_expr<detail::build_type<Expr>>{expr};
}

/*!
 * \brief Create a timed expression wrapping the given expression with the given resolution.
 *
 * The evaluation (and assignment) of the expression will be timed.
 *
 * \tparam R The clock resolution (std::chrono resolutions)
 * \param expr The expression to be wrapped
 * \return a timed expression wrapping the given expression
 */
template <typename R, typename Expr>
auto timed_res(Expr&& expr) -> timed_expr<detail::build_type<Expr>, R> {
    return timed_expr<detail::build_type<Expr>, R>{expr};
}

/*!
 * \brief Create a serial expression wrapping the given expression.
 *
 * The evaluation (and assignment) of the expression is guaranteed to be evaluated serially.
 *
 * \param expr The expression to be wrapped
 * \return a serial expression wrapping the given expression
 */
template <typename Expr>
auto serial(Expr&& expr) -> serial_expr<detail::build_type<Expr>> {
    return serial_expr<detail::build_type<Expr>>{expr};
}

/*!
 * \brief Create a parallel expression wrapping the given expression.
 *
 * The evaluation (and assignment) of the expression will be done parallelly, if possible.
 *
 * \param expr The expression to be wrapped
 * \return a parallel expression wrapping the given expression
 */
template <typename Expr>
auto parallel(Expr&& expr) -> parallel_expr<detail::build_type<Expr>> {
    return parallel_expr<detail::build_type<Expr>>{expr};
}

/*!
 * \brief Create selectedd expression wrapping the given expression.
 * \param expr The expression to be wrapped
 * \return a selected expression wrapping the given expression
 */
template <typename Selector, Selector V, typename Expr>
auto selected(Expr&& expr) -> selected_expr<Selector, V, detail::build_type<Expr>> {
    return selected_expr<Selector, V, detail::build_type<Expr>>{expr};
}

#define selected_helper(v, expr) etl::selected<decltype(v), v>(expr)

/*!
 * \brief Force evaluation of an expression
 *
 * The temporary sub expressions will be evaluated and all the results are guaranteed to be in CPU memory.
 *
 * \return The expression
 */
template <typename Expr, cpp_enable_if(is_etl_expr<std::decay_t<Expr>>::value)>
decltype(auto) operator*(Expr&& expr) {
    force(expr);
    return std::forward<Expr>(expr);
}

} //end of namespace etl
