//
// Created by salim on 05/03/2022.
//

#ifndef FUNCTIONAL_JOIN_HPP
#define FUNCTIONAL_JOIN_HPP

#include "functProgPCH.hpp"


namespace functfun
{
    // FIXME: add a range version and optimize if possible
    //  and also change name join_toString
    //  try to add some more constraint if makes sense
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
                                [](auto&& itStr, const auto& elem) { return *itStr = std::move(elem);} );
        }

       return dest;
    }


    // FIXME: this is not a good soln; no constexpr because of std::string
    // FIXME: might delete this option; it is too expensive; requires effort
    //  to get compile time std::string
    // FIXME : change this accept any range and return a range without using std::string
    //  but if we use delimiter then it needs to be converted to a string if other type are int..
    template<std::input_iterator It,  typename T,  class Proj= std::identity,
             std::indirectly_unary_invocable<std::projected<It, Proj>> Pred>
    requires std::convertible_to<std::string, std::indirect_result_t<Pred, std::projected<It,Proj>>>
    auto join(It first, It last,  T&& delimiter, Pred pred, Proj proj={})->std::string
    {
        if(first == last) return {};

        auto newStr = std::invoke(std::forward<Pred>(pred), std::invoke(std::forward<Proj>(proj), *first));
        std::string result{ std::move(newStr)};

        for(++first; first !=last; ++first)
        {
            newStr = delimiter + std::invoke(std::forward<Pred>(pred), std::invoke(std::forward<Proj>(proj), *first));
            result.append(std::move(newStr));
        }

        return result;
    }






} // end of namespace functfun

#endif//FUNCTIONAL_JOIN_HPP
