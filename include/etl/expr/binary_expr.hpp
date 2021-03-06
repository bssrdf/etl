//=======================================================================
// Copyright (c) 2014-2017 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

namespace etl {

/*!
 * \brief A binary expression
 *
 * A binary expression has a left hand side expression and a right hand side expression and for each element applies a binary opeartor to both expressions.
 */
template <typename T, typename LeftExpr, typename BinaryOp, typename RightExpr>
struct binary_expr final :
        dim_testable<binary_expr<T, LeftExpr, BinaryOp, RightExpr>>,
        value_testable<binary_expr<T, LeftExpr, BinaryOp, RightExpr>>,
        iterable<binary_expr<T, LeftExpr, BinaryOp, RightExpr>>
{
private:
    static_assert(cpp::or_c<
                      cpp::and_c<std::is_same<LeftExpr, scalar<T>>, std::is_same<RightExpr, scalar<T>>>,
                      cpp::and_c<is_etl_expr<LeftExpr>, std::is_same<RightExpr, scalar<T>>>,
                      cpp::and_c<is_etl_expr<RightExpr>, std::is_same<LeftExpr, scalar<T>>>,
                      cpp::and_c<is_etl_expr<LeftExpr>, is_etl_expr<RightExpr>>>::value,
                  "One argument must be an ETL expression and the other one convertible to T");

    using this_type = binary_expr<T, LeftExpr, BinaryOp, RightExpr>; ///< This type

    LeftExpr lhs;  ///< The Left hand side expression
    RightExpr rhs; ///< The right hand side expression

    friend struct etl_traits<binary_expr>;
    friend struct optimizer<binary_expr>;
    friend struct optimizable<binary_expr>;
    friend struct transformer<binary_expr>;

public:
    using value_type        = T;                              ///< The Value type
    using memory_type       = void;                           ///< The memory type
    using const_memory_type = void;                           ///< The const memory type
    using iterator          = etl::iterator<const this_type>; ///< The iterator type
    using const_iterator    = etl::iterator<const this_type>; ///< The const iterator type

    /*!
     * \brief The vectorization type for V
     */
    template <typename V = default_vec>
    using vec_type       = typename V::template vec_type<T>;

    //Cannot be constructed with no args
    binary_expr() = delete;

    /*!
     * \brief Construct a new binary expression
     * \param l The left hand side of the expression
     * \param r The right hand side of the expression
     */
    binary_expr(LeftExpr l, RightExpr r)
            : lhs(std::forward<LeftExpr>(l)), rhs(std::forward<RightExpr>(r)) {
        //Nothing else to init
    }

    /*!
     * \brief Copy construct a new binary expression
     * \param e The expression from which to copy
     */
    binary_expr(const binary_expr& e) = default;

    /*!
     * \brief Move construct a new binary expression
     * \param e The expression from which to move
     */
    binary_expr(binary_expr&& e) noexcept = default;

    //Expressions are invariant
    binary_expr& operator=(const binary_expr& e) = delete;
    binary_expr& operator=(binary_expr&& e) = delete;

    /*!
     * \brief Test if this expression aliases with the given expression
     * \param other The other expression to test
     * \return true if the two expressions aliases, false otherwise
     */
    template <typename E>
    bool alias(const E& other) const noexcept {
        return lhs.alias(other) || rhs.alias(other);
    }

    //Apply the expression

    /*!
     * \brief Returns the element at the given index
     * \param i The index
     * \return a reference to the element at the given index.
     */
    value_type operator[](size_t i) const {
        return BinaryOp::apply(lhs[i], rhs[i]);
    }

    /*!
     * \brief Returns the value at the given index
     * This function never alters the state of the container.
     * \param i The index
     * \return the value at the given index.
     */
    value_type read_flat(size_t i) const {
        return BinaryOp::apply(lhs.read_flat(i), rhs.read_flat(i));
    }

    /*!
     * \brief Perform several operations at once.
     * \param i The index at which to perform the operation
     * \tparam V The vectorization mode to use
     * \return a vector containing several results of the expression
     */
    template <typename V = default_vec>
    ETL_STRONG_INLINE(vec_type<V>) load(size_t i) const {
        return BinaryOp::template load<V>(lhs.template load<V>(i), rhs.template load<V>(i));
    }

    /*!
     * \brief Perform several operations at once.
     * \param i The index at which to perform the operation
     * \tparam V The vectorization mode to use
     * \return a vector containing several results of the expression
     */
    template <typename V = default_vec>
    ETL_STRONG_INLINE(vec_type<V>) loadu(size_t i) const {
        return BinaryOp::template load<V>(lhs.template loadu<V>(i), rhs.template loadu<V>(i));
    }

    /*!
     * \brief Returns the value at the given position (args...)
     * \param args The position indices
     * \return The value at the given position (args...)
     */
    template <typename... S, cpp_enable_if(sizeof...(S) == safe_dimensions<this_type>::value)>
    value_type operator()(S... args) const {
        static_assert(cpp::all_convertible_to<size_t, S...>::value, "Invalid size types");

        return BinaryOp::apply(lhs(args...), rhs(args...));
    }

    /*!
     * \brief Creates a sub view of the expression, effectively removing the first dimension and fixing it to the given index.
     * \param i The index to use
     * \return a sub view of the expression at position i.
     */
    template <bool B = (safe_dimensions<this_type>::value > 1), cpp_enable_if(B)>
    auto operator()(size_t i) {
        return sub(*this, i);
    }

    /*!
     * \brief Creates a sub view of the expression, effectively removing the first dimension and fixing it to the given index.
     * \param i The index to use
     * \return a sub view of the expression at position i.
     */
    template <bool B = (safe_dimensions<this_type>::value > 1), cpp_enable_if(B)>
    auto operator()(size_t i) const {
        return sub(*this, i);
    }

    /*!
     * \brief Creates a slice view of the matrix, effectively reducing the first dimension.
     * \param first The first index to use
     * \param last The last index to use
     * \return a slice view of the matrix at position i.
     */
    auto slice(size_t first, size_t last) noexcept {
        return etl::slice(*this, first, last);
    }

    /*!
     * \brief Creates a slice view of the matrix, effectively reducing the first dimension.
     * \param first The first index to use
     * \param last The last index to use
     * \return a slice view of the matrix at position i.
     */
    auto slice(size_t first, size_t last) const noexcept {
        return etl::slice(*this, first, last);
    }

    // Assignment functions

    /*!
     * \brief Assign to the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_to(L&& lhs)  const {
        std_assign_evaluate(*this, lhs);
    }

    /*!
     * \brief Add to the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_add_to(L&& lhs)  const {
        std_add_evaluate(*this, lhs);
    }

    /*!
     * \brief Sub from the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_sub_to(L&& lhs)  const {
        std_sub_evaluate(*this, lhs);
    }

    /*!
     * \brief Multiply the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_mul_to(L&& lhs)  const {
        std_mul_evaluate(*this, lhs);
    }

    /*!
     * \brief Divide the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_div_to(L&& lhs)  const {
        std_div_evaluate(*this, lhs);
    }

    /*!
     * \brief Modulo the given left-hand-side expression
     * \param lhs The expression to which assign
     */
    template<typename L>
    void assign_mod_to(L&& lhs)  const {
        std_mod_evaluate(*this, lhs);
    }

    // Internals

    /*!
     * \brief Apply the given visitor to this expression and its descendants.
     * \param visitor The visitor to apply
     */
    void visit(const detail::temporary_allocator_visitor& visitor) const {
        lhs.visit(visitor);
        rhs.visit(visitor);
    }

    /*!
     * \brief Apply the given visitor to this expression and its descendants.
     * \param visitor The visitor to apply
     */
    void visit(const detail::back_propagate_visitor& visitor) const {
        lhs.visit(visitor);
        rhs.visit(visitor);
    }

    /*!
     * \brief Apply the given visitor to this expression and its descendants.
     * \param visitor The visitor to apply
     */
    void visit(detail::evaluator_visitor& visitor) const {
        bool old_need_value = visitor.need_value;
        visitor.need_value = true;
        lhs.visit(visitor);
        visitor.need_value = true;
        rhs.visit(visitor);
        visitor.need_value = old_need_value;
    }

    /*!
     * \brief Prints the type of the binary expression to the stream
     * \param os The output stream
     * \param expr The expression to print
     * \return the output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const binary_expr& expr) {
        if (BinaryOp::desc_func) {
            return os << BinaryOp::desc() << "(" << expr.lhs << ", " << expr.rhs << ")";
        } else {
            return os << "(" << expr.lhs << ' ' << BinaryOp::desc() << ' ' << expr.rhs << ")";
        }
    }
};

/*!
 * \brief Specialization for binary_expr.
 */
template <typename T, typename LeftExpr, typename BinaryOp, typename RightExpr>
struct etl_traits<etl::binary_expr<T, LeftExpr, BinaryOp, RightExpr>> {
    using expr_t       = etl::binary_expr<T, LeftExpr, BinaryOp, RightExpr>; ///< The type of the expression
    using left_expr_t  = std::decay_t<LeftExpr>;                             ///< The type of the left expression
    using right_expr_t = std::decay_t<RightExpr>;                            ///< The type of the right expression
    using value_type   = T;                                                  ///< The value type

    static constexpr bool left_directed = cpp::not_u<etl_traits<left_expr_t>::is_generator>::value; ///< True if directed by the left expression, false otherwise

    using sub_expr_t = std::conditional_t<left_directed, left_expr_t, right_expr_t>; ///< The type of sub expression

    static constexpr bool is_etl                  = true;                                                                                                                     ///< Indicates if the type is an ETL expression
    static constexpr bool is_transformer          = false;                                                                                                                    ///< Indicates if the type is a transformer
    static constexpr bool is_view                 = false;                                                                                                                    ///< Indicates if the type is a view
    static constexpr bool is_magic_view           = false;                                                                                                                    ///< Indicates if the type is a magic view
    static constexpr bool is_fast                 = etl_traits<sub_expr_t>::is_fast;                                                                                          ///< Indicates if the expression is fast
    static constexpr bool is_linear               = etl_traits<left_expr_t>::is_linear && etl_traits<right_expr_t>::is_linear && BinaryOp::linear;                            ///< Indicates if the expression is linear
    static constexpr bool is_thread_safe           = etl_traits<left_expr_t>::is_thread_safe && etl_traits<right_expr_t>::is_thread_safe && BinaryOp::thread_safe;             ///< Indicates if the expression is linear
    static constexpr bool is_value                = false;                                                                                                                    ///< Indicates if the expression is of value type
    static constexpr bool is_direct                = false;                                                                                                                    ///< Indicates if the expression has direct memory access
    static constexpr bool is_generator            = etl_traits<left_expr_t>::is_generator && etl_traits<right_expr_t>::is_generator;                                          ///< Indicates if the expression is a generator expression
    static constexpr bool needs_evaluator = etl_traits<left_expr_t>::needs_evaluator || etl_traits<right_expr_t>::needs_evaluator;                    ///< Indicaes if the expression needs an evaluator visitor
    static constexpr bool is_padded               = is_linear && etl_traits<left_expr_t>::is_padded && etl_traits<right_expr_t>::is_padded;                          ///< Indicates if the expression is padded
    static constexpr bool is_aligned               = is_linear && etl_traits<left_expr_t>::is_aligned && etl_traits<right_expr_t>::is_aligned;                          ///< Indicates if the expression is padded
    static constexpr order storage_order          = etl_traits<left_expr_t>::is_generator ? etl_traits<right_expr_t>::storage_order : etl_traits<left_expr_t>::storage_order; ///< The expression storage order

    /*!
     * \brief Indicates if the expression is vectorizable using the
     * given vector mode
     * \tparam V The vector mode
     */
    template <vector_mode_t V>
    using vectorizable = cpp::bool_constant<
        etl_traits<left_expr_t>::template vectorizable<V>::value && etl_traits<right_expr_t>::template vectorizable<V>::value && BinaryOp::template vectorizable<V>::value>;

    /*!
     * \brief Get reference to the main sub expression
     * \param v The binary expr
     * \return a refernece to the main sub expression
     */
    template <bool B = left_directed, cpp_enable_if(B)>
    static constexpr auto& get(const expr_t& v) {
        return v.lhs;
    }

    /*!
     * \brief Get reference to the main sub expression
     * \param v The binary expr
     * \return a refernece to the main sub expression
     */
    template <bool B = left_directed, cpp_disable_if(B)>
    static constexpr auto& get(const expr_t& v) {
        return v.rhs;
    }

    /*!
     * \brief Returns the size of the given expression
     * \param v The expression to get the size for
     * \returns the size of the given expression
     */
    static size_t size(const expr_t& v) {
        return etl_traits<sub_expr_t>::size(get(v));
    }

    /*!
     * \brief Returns the dth dimension of the given expression
     * \param v The expression
     * \param d The dimension to get
     * \return The dth dimension of the given expression
     */
    static size_t dim(const expr_t& v, size_t d) {
        return etl_traits<sub_expr_t>::dim(get(v), d);
    }

    /*!
     * \brief Returns the size of an expression of this fast type.
     * \returns the size of an expression of this fast type.
     */
    static constexpr size_t size() {
        return etl_traits<sub_expr_t>::size();
    }

    /*!
     * \brief Returns the Dth dimension of an expression of this type
     * \tparam D The dimension to get
     * \return the Dth dimension of an expression of this type
     */
    template <size_t D>
    static constexpr size_t dim() {
        return etl_traits<sub_expr_t>::template dim<D>();
    }

    /*!
     * \brief Returns the number of expressions for this type
     * \return the number of dimensions of this type
     */
    static constexpr size_t dimensions() {
        return etl_traits<sub_expr_t>::dimensions();
    }
};

} //end of namespace etl
