//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#ifndef ETL_PRINT_HPP
#define ETL_PRINT_HPP

#include<string>

#include "traits.hpp"

namespace etl {

template<typename T, enable_if_u<etl_traits<T>::is_value> = detail::dummy>
std::ostream& operator<<(std::ostream& stream, const T& v){
    return stream << to_string(v);
}

template<typename T, enable_if_u<(etl_traits<T>::dimensions() > 1)> = detail::dummy>
std::string to_string(const T& m){
    std::string v = "[";
    for(std::size_t i = 0; i < etl::dim<0>(m); ++i){
        v += to_string(sub(m, i));

        if(i < etl::dim<0>(m) - 1){
            v += "\n";
        }
    }
    v += "]";
    return v;
}

template<typename T, enable_if_u<etl_traits<T>::dimensions() == 1> = detail::dummy>
std::string to_string(const T& m){
    return to_octave(m);
}

template<bool Sub = false, typename T, enable_if_u<(etl_traits<T>::dimensions() > 1)> = detail::dummy>
std::string to_octave(const T& m){
    std::string v;
    if(!Sub){
        v = "[";
    }

    for(std::size_t i = 0; i < etl::dim<0>(m); ++i){
        v += to_octave<true>(sub(m, i));

        if(i < etl::dim<0>(m) - 1){
            v += ";";
        }
    }

    if(!Sub){
        v += "]";
    }

    return v;
}

template<bool Sub = false, typename T, enable_if_u<etl_traits<T>::dimensions() == 1> = detail::dummy>
std::string to_octave(const T& m){
    std::string v;
    if(!Sub){
        v = "[";
    }

    std::string comma = "";
    for(std::size_t j = 0; j  < etl::dim<0>(m); ++j){
        v += comma + std::to_string(m(j));
        comma = ",";
    }

    if(!Sub){
        v += "]";
    }

    return v;
}

} //end of namespace etl

#endif
