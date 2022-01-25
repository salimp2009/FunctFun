//
// Created by salim on 25/01/2022.
//

#ifndef FUNCTIONAL_ARITHMETICMEANUTILS_HPP
#define FUNCTIONAL_ARITHMETICMEANUTILS_HPP

#include "functProgPCH.hpp"

#endif //FUNCTIONAL_ARITHMETICMEANUTILS_HPP


template<typename... T>
constexpr auto mean = [](T&&... elems)->std::common_type_t<T...>
{
    // casted to double is promoting for unsigned and int elems to avoid overflow
    // FIXME: test this with float double and also concepts and/or overloads to
    auto sum = (static_cast<double>(elems) + ...);
    return sum/sizeof...(elems) ;
};

template<std::ranges::range R>
constexpr auto slidingMean(R&& range)
{
    return range | ranges::views::sliding(5)
                 | ranges::transform(mean<ranges::range_value_t<R>>)
                 | ranges::to<std::vector>;
}