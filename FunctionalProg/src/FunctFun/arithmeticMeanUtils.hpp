//
// Created by salim on 25/01/2022.
//

#ifndef FUNCTIONAL_ARITHMETICMEANUTILS_HPP
#define FUNCTIONAL_ARITHMETICMEANUTILS_HPP

#include "functProgPCH.hpp"
#include "range/v3/all.hpp"




namespace functfun
{

    constexpr auto mean = []<typename R, typename TInit=double>(const R&& range)
    {
               return std::transform_reduce(std::ranges::cbegin(range), std::ranges::cend(range),
                                     TInit{0.0},
                                     std::plus<>(),
                                     [&](const auto& elem) { return elem/static_cast<double>(std::size(range));});
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

#endif //FUNCTIONAL_ARITHMETICMEANUTILS_HPP