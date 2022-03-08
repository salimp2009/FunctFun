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

    template<std::input_iterator It, typename OutputIt, typename T,  std::invocable<std::iter_reference_t<It>> Pred>
    auto join(It first, It last, OutputIt dest, T&& delimiter, Pred pred) ->OutputIt
    {
        if(first == last) return std::forward<OutputIt>(dest);

        for(; first !=last; ++first)
        {
                auto newStr = std::invoke(std::forward<Pred>(pred), *first);
                *dest = delimiter;
                std::accumulate(std::begin(newStr), std::end(newStr), dest,
                                [](auto&& itStr, const auto& elem) { return *itStr = std::move(elem);} );
        }

       return dest;
    }


    template<std::input_iterator It,  typename T,  std::invocable<std::iter_reference_t<It>> Pred>
    auto join(It first, It last,  T&& delimiter, Pred pred) ->std::string
    {
        if(first == last) return {};
        std::string result{ std::move(std::invoke(std::forward<Pred>(pred), *first))};

        for(++first; first !=last; ++first)
        {
            auto newStr = delimiter + std::invoke(std::forward<Pred>(pred), *first);
            result.insert(result.end(), std::move(newStr));
        }

        return result;
    }






} // end of namespace functfun

#endif//FUNCTIONAL_JOIN_HPP
