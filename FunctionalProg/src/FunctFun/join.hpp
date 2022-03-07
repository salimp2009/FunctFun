//
// Created by salim on 05/03/2022.
//

#ifndef FUNCTIONAL_JOIN_HPP
#define FUNCTIONAL_JOIN_HPP

#include "functProgPCH.hpp"


namespace functfun
{
    // FIXME: originally this is was supposed to join strings
    //  but  it does not with std::back_insert(myString)
    //  tried to use std::accumulate ; it works with std::back_inserter with string
    //  tested with 2 strings in test code it worked but when i tried here it throws a size exception
    // not sure; i get a string by applying predicate over the first element
    // then pass the string to accumulate to add it to other string by using the Output iterator
    // exact same code worked in joinStrings_Test() test code somehow messing it here :)

    template<std::input_iterator It, typename OutputIt, typename T,  std::invocable<std::iter_reference_t<It>> Pred>
    auto join(It first, It last, OutputIt&& dest, T&& delimiter, Pred pred) ->OutputIt
    {
        if(first == last) return std::forward<OutputIt>(dest);

        for(; first !=last; ++first, ++dest )
        {
//                std::string newStr = std::invoke(std::forward<Pred>(pred),  *first);
//                std::accumulate(std::begin(newStr), std::end(newStr), dest,
//                                [](auto&& itStr, const auto& elem) { return itStr = elem;} );
//                std::ranges::copy(newStr.begin(), newStr.end(), dest);
               *dest =  delimiter+ std::invoke(std::forward<Pred>(pred),  *first) ;
        }

       return dest;
    }






} // end of namespace functfun

#endif//FUNCTIONAL_JOIN_HPP
