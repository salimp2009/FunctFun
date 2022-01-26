//
// Created by salim on 25/01/2022.
//

#ifndef FUNCTIONAL_ARITHMETICMEANUTILS_HPP
#define FUNCTIONAL_ARITHMETICMEANUTILS_HPP

#include "functProgPCH.hpp"
#include "range/v3/all.hpp"

#endif //FUNCTIONAL_ARITHMETICMEANUTILS_HPP


// FIXME: add concept to constraint and define the pre / post conditions
constexpr auto mean = []<typename R>(const R&& range)
{
    // FIXME: test this with float double and also concepts and/or overloads to
    if constexpr(std::floating_point<std::ranges::range_value_t<R>>)
    {
        // FIXME::check to optimize & test
       constexpr auto average = [](const auto& rng) { return std::midpoint(*rng.begin(), *(rng.begin()+1));};
       // this is being tested; not the quite result but close
//       auto result =std::accumulate(range.begin(), range.end(), 0.0, [](auto elem1, auto elem2) { return std::midpoint(elem1, elem2);});
//        fmt::print("{} \n", result);
       return range | ranges::views::sliding(2) | ranges::views::transform(average) ;
    }
    else
    {
        // FIXME:: check to optimize & test
        constexpr auto average = [](const auto& rng)
        {
            // casted to double because midpoint rounds integers; 5+2/2 =4 ;it should be a double e.g; 5+2/2 =3.5
            return std::midpoint(static_cast<double>(*rng.begin()), static_cast<double>(*std::next(rng)) );
        };

        return range | ranges::views::sliding(2) | ranges::views::transform(average) ;
    }
};

template<std::ranges::range R>
constexpr auto slidingMean(R&& range, std::size_t sampleCount)
{
    return range | ranges::views::sliding(sampleCount)
                 | ranges::views::transform(mean)
                 | ranges::to<std::vector>();
}