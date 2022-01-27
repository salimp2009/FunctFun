//
// Created by salim on 25/01/2022.
//

#ifndef FUNCTIONAL_ARITHMETICMEANUTILS_HPP
#define FUNCTIONAL_ARITHMETICMEANUTILS_HPP

#include "functProgPCH.hpp"
#include "range/v3/all.hpp"

#endif //FUNCTIONAL_ARITHMETICMEANUTILS_HPP


namespace functfun
{
    // FIXME: add some doxygen :)
    constexpr auto mean = []<typename R>(const R&& range)
    {
        constexpr double initSum=0.0;
        return std::transform_reduce(std::ranges::begin(range), std::ranges::end(range),
                                     initSum,
                                     std::plus<>(),
                                     [&](auto elem) { return elem/static_cast<double>(std::size(range));});
    };

    template<std::ranges::viewable_range R>
    requires std::ranges::forward_range<R>
    constexpr auto slidingMean(R&& range, std::size_t sampleCount)
    {
        return range | ranges::views::sliding(sampleCount)
                     | ranges::views::transform(mean)
                     | ranges::to<std::vector>();
    }


} // end of namespace

