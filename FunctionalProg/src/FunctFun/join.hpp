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
    //  since it turns to a underlying type "char" and every
    //  character in string needs to be pushed back one-by-one
    //  or may try ranges::join for this or C version of strcat
    //  FIXME: Best way might to get the second range as input and use insert;
    //   it will probably work for everything and might use Projection as well

    template<std::input_iterator It, typename OutputIt, typename T,  std::invocable<std::iter_reference_t<It>> Pred>
    constexpr auto join(It first, It last, OutputIt&& dest, T&& delimiter, Pred pred) ->OutputIt
    {
        if(first == last) return {std::move(dest)};

        for(; first !=last; ++first, ++dest)
        {
               *dest =  delimiter+ std::invoke(std::forward<Pred>(pred),  *first) ;
        }

       return dest;
    }






} // end of namespace functfun

#endif//FUNCTIONAL_JOIN_HPP
