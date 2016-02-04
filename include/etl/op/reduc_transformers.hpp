//=======================================================================
// Copyright (c) 2014-2016 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

namespace etl {

/*!
 * \brief Transform (dynamic) that sums the expression from the right, effectively removing the right dimension.
 * \tparam T The type on which the transformer is applied
 */
template <typename T>
struct sum_r_transformer {
    using sub_type   = T;           ///< The type on which the expression works
    using value_type = value_t<T>;  ///< The type of valuie

    sub_type sub; ///< The subexpression

    /*!
     * \brief Construct a new transformer around the given expression
     * \param expr The sub expression
     */
    explicit sum_r_transformer(sub_type expr)
            : sub(expr) {}

    /*!
     * \brief Returns the value at the given index
     * \param i The index
     * \return the value at the given index.
     */
    value_type operator[](std::size_t i) const {
        return sum(sub(i));
    }

    /*!
     * \brief Returns the value at the given index
     * This function never has side effects.
     * \param i The index
     * \return the value at the given index.
     */
    value_type read_flat(std::size_t i) const {
        return sum(sub(i));
    }

    /*!
     * \brief Returns the value at the given position (i, sizes...)
     */
    template <typename... Sizes>
    value_type operator()(std::size_t i, Sizes... /*sizes*/) const {
        return sum(sub(i));
    }

    /*!
     * \brief Returns the value on which the transformer is working.
     * \return A reference  to the value on which the transformer is working.
     */
    sub_type& value() {
        return sub;
    }

    /*!
     * \brief Test if this expression aliases with the given expression
     * \param rhs The other expression to test
     * \return true if the two expressions aliases, false otherwise
     */
    template<typename E>
    bool alias(const E& rhs) const noexcept {
        return sub.alias(rhs);
    }
};

/*!
 * \brief Transform (dynamic) that averages the expression from the right, effectively removing the right dimension.
 * \tparam T The type on which the transformer is applied
 */
template <typename T>
struct mean_r_transformer {
    using sub_type   = T;           ///< The type on which the expression works
    using value_type = value_t<T>;  ///< The type of valuie

    sub_type sub; ///< The subexpression

    /*!
     * \brief Construct a new transformer around the given expression
     * \param expr The sub expression
     */
    explicit mean_r_transformer(sub_type expr)
            : sub(expr) {}

    /*!
     * \brief Returns the value at the given index
     * \param i The index
     * \return the value at the given index.
     */
    value_type operator[](std::size_t i) const {
        return mean(sub(i));
    }

    /*!
     * \brief Returns the value at the given index
     * This function never has side effects.
     * \param i The index
     * \return the value at the given index.
     */
    value_type read_flat(std::size_t i) const {
        return mean(sub(i));
    }

    /*!
     * \brief Returns the value at the given position (i, sizes...)
     */
    template <typename... Sizes>
    value_type operator()(std::size_t i, Sizes... /*sizes*/) const {
        return mean(sub(i));
    }

    /*!
     * \brief Returns the value on which the transformer is working.
     * \return A reference  to the value on which the transformer is working.
     */
    sub_type& value() {
        return sub;
    }

    /*!
     * \brief Test if this expression aliases with the given expression
     * \param rhs The other expression to test
     * \return true if the two expressions aliases, false otherwise
     */
    template<typename E>
    bool alias(const E& rhs) const noexcept {
        return sub.alias(rhs);
    }
};

/*!
 * \brief Transform (dynamic) that sums the expression from the left, effectively removing the left dimension.
 * \tparam T The type on which the transformer is applied
 */
template <typename T>
struct sum_l_transformer {
    using sub_type   = T;           ///< The type on which the expression works
    using value_type = value_t<T>;  ///< The type of valuie

    sub_type sub; ///< The subexpression

    /*!
     * \brief Construct a new transformer around the given expression
     * \param expr The sub expression
     */
    explicit sum_l_transformer(sub_type expr)
            : sub(expr) {}

    /*!
     * \brief Returns the value at the given index
     * \param j The index
     * \return the value at the given index.
     */
    value_type operator[](std::size_t j) const {
        value_type m = 0.0;

        for (std::size_t i = 0; i < dim<0>(sub); ++i) {
            m += sub[j + i * (size(sub) / dim<0>(sub))];
        }

        return m;
    }

    /*!
     * \brief Returns the value at the given index
     * This function never has side effects.
     * \param j The index
     * \return the value at the given index.
     */
    value_type read_flat(std::size_t j) const noexcept {
        value_type m = 0.0;

        for (std::size_t i = 0; i < dim<0>(sub); ++i) {
            m += sub.read_flat(j + i * (size(sub) / dim<0>(sub)));
        }

        return m;
    }

    /*!
     * \brief Access to the value at the given (j, sizes...) position
     * \param j The first index
     * \param sizes The remaining indices
     * \return The value at the position (j, sizes...)
     */
    template <typename... Sizes>
    value_type operator()(std::size_t j, Sizes... sizes) const {
        value_type m = 0.0;

        for (std::size_t i = 0; i < dim<0>(sub); ++i) {
            m += sub(i, j, sizes...);
        }

        return m;
    }

    /*!
     * \brief Returns the value on which the transformer is working.
     * \return A reference  to the value on which the transformer is working.
     */
    sub_type& value() {
        return sub;
    }

    /*!
     * \brief Test if this expression aliases with the given expression
     * \param rhs The other expression to test
     * \return true if the two expressions aliases, false otherwise
     */
    template<typename E>
    bool alias(const E& rhs) const noexcept {
        return sub.alias(rhs);
    }
};

/*!
 * \brief Transform (dynamic) that averages the expression from the left, effectively removing the left dimension.
 * \tparam T The type on which the transformer is applied
 */
template <typename T>
struct mean_l_transformer {
    using sub_type   = T;           ///< The type on which the expression works
    using value_type = value_t<T>;  ///< The type of valuie

    sub_type sub; ///< The subexpression

    /*!
     * \brief Construct a new transformer around the given expression
     * \param expr The sub expression
     */
    explicit mean_l_transformer(sub_type expr)
            : sub(expr) {}

    /*!
     * \brief Returns the value at the given index
     * \param j The index
     * \return the value at the given index.
     */
    value_type operator[](std::size_t j) const {
        value_type m = 0.0;

        for (std::size_t i = 0; i < dim<0>(sub); ++i) {
            m += sub[j + i * (size(sub) / dim<0>(sub))];
        }

        return m / dim<0>(sub);
    }

    /*!
     * \brief Returns the value at the given index
     * This function never has side effects.
     * \param j The index
     * \return the value at the given index.
     */
    value_type read_flat(std::size_t j) const noexcept {
        value_type m = 0.0;

        for (std::size_t i = 0; i < dim<0>(sub); ++i) {
            m += sub.read_flat(j + i * (size(sub) / dim<0>(sub)));
        }

        return m / dim<0>(sub);
    }

    /*!
     * \brief Access to the value at the given (j, sizes...) position
     * \param j The first index
     * \param sizes The remaining indices
     * \return The value at the position (j, sizes...)
     */
    template <typename... Sizes>
    value_type operator()(std::size_t j, Sizes... sizes) const {
        value_type m = 0.0;

        for (std::size_t i = 0; i < dim<0>(sub); ++i) {
            m += sub(i, j, sizes...);
        }

        return m / dim<0>(sub);
    }

    /*!
     * \brief Returns the value on which the transformer is working.
     * \return A reference  to the value on which the transformer is working.
     */
    sub_type& value() {
        return sub;
    }

    /*!
     * \brief Test if this expression aliases with the given expression
     * \param rhs The other expression to test
     * \return true if the two expressions aliases, false otherwise
     */
    template<typename E>
    bool alias(const E& rhs) const noexcept {
        return sub.alias(rhs);
    }
};

/*!
 * \brief Specialization for (sum-mean)_r_transformer
 */
template <typename T>
struct etl_traits<T, std::enable_if_t<cpp::or_c<
                         cpp::is_specialization_of<etl::sum_r_transformer, std::decay_t<T>>,
                         cpp::is_specialization_of<etl::mean_r_transformer, std::decay_t<T>>>::value>> {
    using expr_t     = T; ///< The expression type
    using sub_expr_t = std::decay_t<typename std::decay_t<T>::sub_type>; ///< The sub expression type

    static constexpr const bool is_etl                  = true;                                            ///< Indicates if the type is an ETL expression
    static constexpr const bool is_transformer          = true;                                            ///< Indicates if the type is a transformer
    static constexpr const bool is_view                 = false;                                           ///< Indicates if the type is a view
    static constexpr const bool is_magic_view           = false;                                           ///< Indicates if the type is a magic view
    static constexpr const bool is_fast                 = etl_traits<sub_expr_t>::is_fast;                 ///< Indicates if the expression is fast
    static constexpr const bool is_linear               = false;                                           ///< Indicates if the expression is linear
    static constexpr const bool is_value                = false;                                           ///< Indicates if the expression is of value type
    static constexpr const bool is_generator            = false;                                           ///< Indicates if the expression is a generated
    static constexpr const bool vectorizable            = false;                                           ///< Indicates if the expression is vectorizable
    static constexpr const bool needs_temporary_visitor = etl_traits<sub_expr_t>::needs_temporary_visitor; ///< Indicates if the expression needs a temporary visitor
    static constexpr const bool needs_evaluator_visitor = etl_traits<sub_expr_t>::needs_evaluator_visitor; ///< Indicaes if the expression needs an evaluator visitor
    static constexpr const order storage_order          = etl_traits<sub_expr_t>::storage_order;           ///< The expression storage order

    /*!
     * \brief Returns the size of the given expression
     * \param v The expression to get the size for
     * \returns the size of the given expression
     */
    static std::size_t size(const expr_t& v) {
        return etl::dim<0>(v.sub);
    }

    /*!
     * \brief Returns the dth dimension of the given expression
     * \param v The expression
     * \param d The dimension to get
     * \return The dth dimension of the given expression
     */
    static std::size_t dim(const expr_t& v, std::size_t d) {
        cpp_unused(d);
        return etl::dim<0>(v.sub);
    }

    /*!
     * \brief Returns the size of an expression of this fast type.
     * \returns the size of an expression of this fast type.
     */
    static constexpr std::size_t size() {
        return etl_traits<sub_expr_t>::template dim<0>();
    }

    /*!
     * \brief Returns the Dth dimension of an expression of this type
     * \tparam D The dimension to get
     * \return the Dth dimension of an expression of this type
     */
    template <std::size_t D>
    static constexpr std::size_t dim() {
        return etl_traits<sub_expr_t>::template dim<0>();
    }

    /*!
     * \brief Returns the number of expressions for this type
     * \return the number of dimensions of this type
     */
    static constexpr std::size_t dimensions() {
        return 1;
    }
};

/*!
 * \brief Specialization for (sum-mean)_r_transformer
 */
template <typename T>
struct etl_traits<T, std::enable_if_t<cpp::or_c<
                         cpp::is_specialization_of<etl::sum_l_transformer, std::decay_t<T>>,
                         cpp::is_specialization_of<etl::mean_l_transformer, std::decay_t<T>>>::value>> {
    using expr_t     = T; ///< The expression type
    using sub_expr_t = std::decay_t<typename std::decay_t<T>::sub_type>; ///< The sub expression type

    static constexpr const bool is_etl                  = true;  ///< Indicates if the type is an ETL expression
    static constexpr const bool is_transformer          = true;  ///< Indicates if the type is a transformer
    static constexpr const bool is_view                 = false; ///< Indicates if the type is a view
    static constexpr const bool is_magic_view           = false; ///< Indicates if the type is a magic view
    static constexpr const bool is_fast                 = etl_traits<sub_expr_t>::is_fast; ///< Indicates if the expression is fast
    static constexpr const bool is_linear               = false; ///< Indicates if the expression is linear
    static constexpr const bool is_value                = false; ///< Indicates if the expression is of value type
    static constexpr const bool is_generator            = false; ///< Indicates if the expression is a generated
    static constexpr const bool vectorizable            = false; ///< Indicates if the expression is vectorizable
    static constexpr const bool needs_temporary_visitor = etl_traits<sub_expr_t>::needs_temporary_visitor; ///< Indicates if the expression needs a temporary visitor
    static constexpr const bool needs_evaluator_visitor = etl_traits<sub_expr_t>::needs_evaluator_visitor; ///< Indicaes if the expression needs an evaluator visitor
    static constexpr const order storage_order          = etl_traits<sub_expr_t>::storage_order;           ///< The expression storage order

    /*!
     * \brief Returns the size of the given expression
     * \param v The expression to get the size for
     * \returns the size of the given expression
     */
    static std::size_t size(const expr_t& v) {
        return etl::size(v.sub) / etl::dim<0>(v.sub);
    }

    /*!
     * \brief Returns the dth dimension of the given expression
     * \param v The expression
     * \param d The dimension to get
     * \return The dth dimension of the given expression
     */
    static std::size_t dim(const expr_t& v, std::size_t d) {
        return etl::dim(v.sub, d + 1);
    }

    /*!
     * \brief Returns the size of an expression of this fast type.
     * \returns the size of an expression of this fast type.
     */
    static constexpr std::size_t size() {
        return etl_traits<sub_expr_t>::size() / etl_traits<sub_expr_t>::template dim<0>();
    }

    /*!
     * \brief Returns the Dth dimension of an expression of this type
     * \tparam D The dimension to get
     * \return the Dth dimension of an expression of this type
     */
    template <std::size_t D>
    static constexpr std::size_t dim() {
        return etl_traits<sub_expr_t>::template dim<D + 1>();
    }

    /*!
     * \brief Returns the number of expressions for this type
     * \return the number of dimensions of this type
     */
    static constexpr std::size_t dimensions() {
        return etl_traits<sub_expr_t>::dimensions() - 1;
    }
};

/*!
 * \brief Display the transformer on the given stream
 * \param os The output stream
 * \param transformer The transformer to print
 * \return the output stream
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const sum_r_transformer<T>& transformer) {
    return os << "sum_r(" << transformer.sub << ")";
}

/*!
 * \brief Display the transformer on the given stream
 * \param os The output stream
 * \param transformer The transformer to print
 * \return the output stream
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const mean_r_transformer<T>& transformer) {
    return os << "mean_r(" << transformer.sub << ")";
}

/*!
 * \brief Display the transformer on the given stream
 * \param os The output stream
 * \param transformer The transformer to print
 * \return the output stream
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const sum_l_transformer<T>& transformer) {
    return os << "sum_l(" << transformer.sub << ")";
}

/*!
 * \brief Display the transformer on the given stream
 * \param os The output stream
 * \param transformer The transformer to print
 * \return the output stream
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const mean_l_transformer<T>& transformer) {
    return os << "mean_l(" << transformer.sub << ")";
}

} //end of namespace etl