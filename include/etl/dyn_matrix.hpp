//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef ETL_DYN_MATRIX_HPP
#define ETL_DYN_MATRIX_HPP

#include<vector>

#include "assert.hpp"
#include "tmp.hpp"
#include "fast_op.hpp"
#include "fast_expr.hpp"

namespace etl {

template<typename T>
struct dyn_matrix {
public:
    using       value_type = T;
    using     storage_impl = std::vector<value_type>;
    using         iterator = typename storage_impl::iterator;
    using   const_iterator = typename storage_impl::const_iterator;

private:
    storage_impl _data;
    const std::size_t _rows;
    const std::size_t _columns;

public:
    ///{{{ Construction

    explicit dyn_matrix(const dyn_matrix& rhs) : _data(rhs._data), _rows(rhs._rows), _columns(rhs._columns) {
        //Nothing to init
    }

    dyn_matrix(std::size_t rows, std::size_t columns) : _data(rows * columns), _rows(rows), _columns(columns) {
        //Nothing to init
    }

    dyn_matrix(std::size_t rows, std::size_t columns, const value_type& value) : _data(rows * columns), _rows(rows), _columns(columns) {
        std::fill(_data.begin(), _data.end(), value);
    }

    dyn_matrix(std::size_t rows, std::size_t columns, std::initializer_list<value_type> l) : _data(rows * columns), _rows(rows), _columns(columns) {
        etl_assert(l.size() == size(), "Cannot copy from an initializer of different size");

        std::copy(l.begin(), l.end(), begin());
    }

    template<typename LE, typename Op, typename RE>
    explicit dyn_matrix(const binary_expr<value_type, LE, Op, RE>& e) :
            _data(etl::rows(e) * etl::columns(e)),
            _rows(etl::rows(e)),
            _columns(etl::columns(e)) {
        for(std::size_t i = 0; i < rows(); ++i){
            for(std::size_t j = 0; j < columns(); ++j){
                _data[i * columns() + j] = e(i,j);
            }
        }
    }

    template<typename E, typename Op>
    explicit dyn_matrix(const unary_expr<value_type, E, Op>& e) :
            _data(etl::rows(e) * etl::columns(e)),
            _rows(etl::rows(e)),
            _columns(etl::columns(e)){
        for(std::size_t i = 0; i < rows(); ++i){
            for(std::size_t j = 0; j < columns(); ++j){
                _data[i * columns() + j] = e(i,j);
            }
        }
    }

    //Default move
    dyn_matrix(dyn_matrix&& rhs) = default;

    //}}}

    //{{{ Assignment

    //Copy assignment operator

    dyn_matrix& operator=(const dyn_matrix& rhs){
        ensure_same_size(*this, rhs);

        for(std::size_t i = 0; i < size(); ++i){
            _data[i] = rhs[i];
        }

        return *this;
    }

    //Allow copy from other containers

    template<typename Container, enable_if_u<std::is_same<typename Container::value_type, value_type>::value> = detail::dummy>
    dyn_matrix& operator=(const Container& vec){
        etl_assert(vec.size() == _rows * _columns, "Cannot copy from a vector of different size");

        for(std::size_t i = 0; i < _rows * _columns; ++i){
            _data[i] = vec[i];
        }

        return *this;
    }

    //Construct from expression

    template<typename LE, typename Op, typename RE>
    dyn_matrix& operator=(binary_expr<value_type, LE, Op, RE>&& e){
        ensure_same_size(*this, e);

        for(std::size_t i = 0; i < rows(); ++i){
            for(std::size_t j = 0; j < columns(); ++j){
                _data[i * columns() + j] = e(i,j);
            }
        }

        return *this;
    }

    template<typename E, typename Op>
    dyn_matrix& operator=(unary_expr<value_type, E, Op>&& e){
        ensure_same_size(*this, e);

        for(std::size_t i = 0; i < rows(); ++i){
            for(std::size_t j = 0; j < columns(); ++j){
                _data[i * columns() + j] = e(i,j);
            }
        }

        return *this;
    }

    //Set the same value to each element of the matrix
    dyn_matrix& operator=(const value_type& value){
        std::fill(_data.begin(), _data.end(), value);

        return *this;
    }

    //Default move
    dyn_matrix& operator=(dyn_matrix&& rhs) = default;

    //}}}

    //{{{ Accessors

    size_t rows() const {
        return _rows;
    }

    size_t columns() const {
        return _columns;
    }

    constexpr size_t size() const {
        return _rows * _columns;
    }

    value_type& operator()(size_t i, size_t j){
        etl_assert(i < _rows, "Out of bounds");
        etl_assert(j < _columns, "Out of bounds");

        return _data[i * _columns + j];
    }

    const value_type& operator()(size_t i, size_t j) const {
        etl_assert(i < _rows, "Out of bounds");
        etl_assert(j < _columns, "Out of bounds");

        return _data[i * _columns + j];
    }

    const value_type& operator[](size_t i) const {
        etl_assert(i < size(), "Out of bounds");

        return _data[i];
    }

    value_type& operator[](size_t i){
        etl_assert(i < size(), "Out of bounds");

        return _data[i];
    }

    const_iterator begin() const {
        return _data.begin();
    }

    const_iterator end() const {
        return _data.end();
    }

    iterator begin(){
        return _data.begin();
    }

    iterator end(){
        return _data.end();
    }

    //}}}
};

} //end of namespace etl

#endif
