//
// Created by salim on 01/04/2022.
//

#ifndef FUNCTIONAL_RANGECONCEPTS_HPP
#define FUNCTIONAL_RANGECONCEPTS_HPP

#include "functProgPCH.hpp"

namespace functfun
{

    template<class R, class P>
    concept compatible_joinable_ranges =
            std::common_with<std::ranges::range_value_t<R>, std::ranges::range_value_t<P>> &&
            std::common_reference_with<std::ranges::range_reference_t<R>, std::ranges::range_reference_t<P>> &&
            std::common_reference_with<std::ranges::range_rvalue_reference_t<R>, std::ranges::range_rvalue_reference_t<P>>;

    template<typename R>
    concept bidi_common = std::ranges::bidirectional_range<R>;






} // endof namespace functfun

#endif//FUNCTIONAL_RANGECONCEPTS_HPP
