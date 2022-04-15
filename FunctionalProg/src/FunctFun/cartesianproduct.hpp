//
// Created by salim on 15/04/2022.
//

#ifndef FUNCTIONAL_CARTESIANPRODUCT_HPP
#define FUNCTIONAL_CARTESIANPRODUCT_HPP

#include "functProgPCH.hpp"

namespace functfun
{
    template<std::ranges::forward_range... Vs>
    requires (std::ranges::view<Vs> && ...)
    class cartesianproduct_view : public std::ranges::view_interface<Vs...>
    {
        std::tuple<Vs...> mbases;

        template<bool Const>
        struct iterator;

        template<bool Const>
        struct sentinel;


    };





} // endof namespace functfun


#endif//FUNCTIONAL_CARTESIANPRODUCT_HPP
