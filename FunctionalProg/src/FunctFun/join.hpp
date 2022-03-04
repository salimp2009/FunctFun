//
// Created by salim on 05/03/2022.
//

#ifndef FUNCTIONAL_JOIN_HPP
#define FUNCTIONAL_JOIN_HPP

#include "functProgPCH.hpp"

namespace functfun
{
    template<std::input_iterator It, typename OutputIt, typename T,  std::regular_invocable<It, OutputIt, T> Pred>
    OutputIt join(It first, It last, OutputIt dest, T delimiter, Pred op)
    {


        return {};
    }






} // end of namespace functfun

#endif//FUNCTIONAL_JOIN_HPP
