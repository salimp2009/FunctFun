//
// Created by salim on 05/03/2022.
//

#ifndef FUNCTIONAL_JOIN_HPP
#define FUNCTIONAL_JOIN_HPP

#include "functProgPCH.hpp"


namespace functfun
{
    // FIXME: change name join_toString
    //  try to add some more constraint if makes sense
    // FIXME: turn into struct join_impl with 2 operator()(...) similar to std::ranges
    template<std::input_iterator It, typename OutputIt, typename T, class Proj= std::identity,
             std::indirectly_unary_invocable<std::projected<It, Proj>> Pred>
    constexpr auto join(It first, It last, OutputIt dest, T&& delimiter, Pred pred, Proj proj= {}) ->OutputIt
    {
        if(first == last) return std::forward<OutputIt>(dest);

        for(; first !=last; ++first)
        {
                auto newStr = std::invoke(std::forward<Pred>(pred), std::invoke(proj, *first));
                *dest = delimiter;
                std::accumulate(std::begin(newStr), std::end(newStr), dest,
                                [](auto&& itStr, auto&& elem) { return *itStr = std::forward<decltype(elem)>(elem);} );
        }

       return dest;
    }

    template<std::ranges::input_range Rng, typename OutputIt, typename T, class Proj= std::identity,
             std::indirectly_unary_invocable<std::projected<std::ranges::range_reference_t<Rng>, Proj>> Pred>
    constexpr auto join(Rng&& rng, OutputIt dest, T&& delimiter, Pred pred, Proj proj={}) ->OutputIt
    {
        auto first = std::ranges::begin(rng);
        auto last = std::ranges::end(rng);

        if(first == last) return std::forward<OutputIt>(dest);

        for(; first !=last; ++first)
        {
            auto newStr = std::invoke(std::forward<Pred>(pred), std::invoke(proj, *first));
            *dest = delimiter;
            std::accumulate(std::ranges::begin(newStr), std::ranges::end(newStr), dest,
                            [](auto&& itStr, auto&& elem) { return *itStr = std::forward<decltype(elem)>(elem);} );

        }

        return dest;
    }








} // end of namespace functfun

#endif//FUNCTIONAL_JOIN_HPP
