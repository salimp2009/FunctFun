//
// Created by salim on 25/01/2022.
//

#ifndef FUNCTIONAL_ARITHMETICMEANUTILS_HPP
#define FUNCTIONAL_ARITHMETICMEANUTILS_HPP

#include "functProgPCH.hpp"
#include "range/v3/all.hpp"

namespace functfun::details
{
    // FIXME: try to give an option to user to define the value of the initial value
    template<typename T=double>
    constexpr auto mean = []<typename R>(const R&& range)
    {
        return std::transform_reduce(std::ranges::cbegin(range), std::ranges::cend(range),
                                     T{0.0},
                                     std::plus<>(),
                                     [&](const auto& elem) { return elem/static_cast<double>(std::size(range));});
    };

    template<typename T>
    concept ArithmeticType = std::is_arithmetic_v<std::remove_cvref_t<T>>;

    template<typename T>
    concept Duration     =  std::is_same_v<std::chrono::duration<long double>,std::remove_cvref_t<T>> ||
                            std::is_same_v<std::chrono::duration<long int>,std::remove_cvref_t<T>>  ||
                            std::is_same_v<std::chrono::duration<int>,std::remove_cvref_t<T>> ||
                            std::is_same_v<std::chrono::duration<double>,std::remove_cvref_t<T>> ||
                            std::is_same_v<std::chrono::duration<long long int>,std::remove_cvref_t<T>>;
}

namespace functfun
{
    using namespace functfun::details;

    // FIXME: try to give an option to user to define the value of the initial value
    template<std::ranges::viewable_range R>
    requires std::ranges::forward_range<R> && ArithmeticType<std::ranges::range_value_t<R>>
    constexpr auto slidingMean(R&& range, std::size_t sampleCount)
    {
        return range | ranges::views::sliding(sampleCount)
                     | ranges::views::transform(mean<double>)
                     | ranges::to<std::vector>();
    }

    template<std::ranges::viewable_range R>
    requires std::ranges::forward_range<R> && Duration<std::ranges::range_value_t<R>>
    constexpr auto slidingMean(R&& range, std::size_t sampleCount)
    {
        return range | ranges::views::sliding(sampleCount)
                     | ranges::views::transform(mean<std::chrono::duration<long double>>)
                     | ranges::to<std::vector>();
    }



} // end of namespace

#endif //FUNCTIONAL_ARITHMETICMEANUTILS_HPP