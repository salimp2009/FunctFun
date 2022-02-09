//
// Created by salim on 25/01/2022.
//

#ifndef FUNCTIONAL_ARITHMETICMEANUTILS_HPP
#define FUNCTIONAL_ARITHMETICMEANUTILS_HPP

#include "functProgPCH.hpp"
#include "range/v3/all.hpp"




namespace functfun
{
    // FIXME: Testing duration version but did not work
    template<typename T>
    concept DurationDouble  = std::chrono::treat_as_floating_point_v<typename T::rep>;

    template<typename T>
    concept DurationInt     = not DurationDouble<T>;

    template<typename T>
    concept Duration        = DurationDouble<T> || DurationInt<T>;


    constexpr auto mean = []<typename R, typename init=double>(const R&& range)
    {
               return std::transform_reduce(std::ranges::cbegin(range), std::ranges::cend(range),
                                     init{0.0},
                                     std::plus<>(),
                                     [&](const auto& elem) { return elem/static_cast<double>(std::size(range));});
    };

    // FIXME: Testing duration version but did not work
    constexpr auto meanDouble = []<typename R, typename init=std::chrono::duration<long double>>(const R&& range)
    {
        return std::transform_reduce(std::ranges::cbegin(range), std::ranges::cend(range),
                                     init{0.0},
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

    // FIXME: Testing duration version but did not work
//    template<std::ranges::viewable_range R>
//    requires std::ranges::forward_range<R> && Duration<std::ranges::range_value_t<R>>
//    constexpr auto slidingMean(R&& range, std::size_t sampleCount)
//    {
//        return range | ranges::views::sliding(sampleCount)
//               | ranges::views::transform(meanDouble)
//               | ranges::to<std::vector>();
//    }


} // end of namespace

#endif //FUNCTIONAL_ARITHMETICMEANUTILS_HPP