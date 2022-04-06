//
// Created by salim on 01/04/2022.
//

#ifndef FUNCTIONAL_RANGECONCEPTS_HPP
#define FUNCTIONAL_RANGECONCEPTS_HPP

#include "functProgPCH.hpp"

#define functFWD(x) static_cast<decltype(x)&&>(x)

namespace functfun::details
{
    template<typename T>
    using withRef = T&;

    template<typename T>
    concept canReference = requires { typename withRef<T>; };

    template<typename T>
    concept deReferancable = requires (T a)
    {
        { *a } -> canReference;
    };

    template<bool IsConst, typename T>
    using maybeConst_t = std::conditional_t<IsConst, const T, T>;

    struct Empty {};

    template<bool IsPresent, typename T>
    using maybePresent_t = std::conditional_t<IsPresent, T, Empty>;

} // end of namespace details

namespace functfun
{

    template<class R, class P>
    concept compatible_joinable_ranges =
            std::common_with<std::ranges::range_value_t<R>, std::ranges::range_value_t<P>> &&
            std::common_reference_with<std::ranges::range_reference_t<R>, std::ranges::range_reference_t<P>> &&
            std::common_reference_with<std::ranges::range_rvalue_reference_t<R>, std::ranges::range_rvalue_reference_t<P>>;

    template<typename R>
    concept bidi_common = std::ranges::bidirectional_range<R> && std::ranges::common_range<R>;






} // endof namespace functfun

#endif//FUNCTIONAL_RANGECONCEPTS_HPP
