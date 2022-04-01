//
// Created by salim on 01/04/2022.
//

#ifndef FUNCTIONAL_JOINWITHVIEW_HPP
#define FUNCTIONAL_JOINWITHVIEW_HPP

#include "functProgPCH.hpp"
#include "rangeconcepts.hpp"
#include "viewAdaptors.hpp"
#include "rangeutils.hpp"

namespace functfun
{
    template<std::ranges::input_range V, std::ranges::forward_range Pattern>
    requires std::ranges::view<V>
            && std::ranges::input_range<std::ranges::range_reference_t<V>>
            && std::ranges::view<Pattern>
            && compatible_joinable_ranges<std::ranges::range_reference_t<V>, Pattern>
    class joinwith_view : public std::ranges::view_interface<joinwith_view<V, Pattern>>
    {
        using InnerRng = std::ranges::range_reference_t<V>;

        V base=V();
        [[no_unique_address]] std::ranges::__detail::__non_propagating_cache<std::remove_cv_t<InnerRng>> inner;


    };


} // endof namespace functfun

#endif//FUNCTIONAL_JOINWITHVIEW_HPP
