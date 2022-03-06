//
// Created by salim on 05/03/2022.
//

#ifndef FUNCTIONAL_JOIN_HPP
#define FUNCTIONAL_JOIN_HPP

#include "functProgPCH.hpp"


namespace functfun
{
    template<typename It, typename OutputIt, typename T,  typename Pred>
    constexpr auto join(It first, It last, OutputIt&& dest, T&& delimiter, Pred pred) ->OutputIt
    {
        //if(first == last) return {std::move(dest)};

//        dest= "c";
//        dest= "d+++";
//        dest = std::invoke(pred,  *first);
//        dest= "new+++";
//        dest = std::invoke(pred,  *(std::next(first)));

        for(; first !=last; std::next(first))
        {

               dest = std::string{delimiter};
               dest = std::invoke(pred,  *first);
        }


       return dest;
    }






} // end of namespace functfun

#endif//FUNCTIONAL_JOIN_HPP
