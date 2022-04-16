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

    template<typename Range>
    concept simpleView = std::ranges::view<Range> && std::ranges::range<const Range>
            && std::same_as<std::ranges::iterator_t<Range>, std::ranges::iterator_t<const Range>>
            && std::same_as<std::ranges::sentinel_t<Range>, std::ranges::sentinel_t<const Range>>;

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

    template<class... Vs>
    concept cartesproduct_is_random = (std::ranges::random_access_range<Vs> && ...)
                                      && (std::ranges::sized_range<Vs> && ...);

    template<class... Vs>
    concept cartesproduct_is_bidicommon = (bidi_common<Vs> && ...);

    template<class... Vs>
    concept cartesproduct_is_common = (std::ranges::common_range<Vs> && ...)
                                     || (std::ranges::random_access_range<Vs> && ...);





} // endof namespace functfun

#endif//FUNCTIONAL_RANGECONCEPTS_HPP
