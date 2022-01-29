//
// Created by salim on 28/01/2022.
//

#ifndef FUNCTIONAL_MAKEUNIFORMDISTRIBUTION_HPP
#define FUNCTIONAL_MAKEUNIFORMDISTRIBUTION_HPP
#include "functProgPCH.hpp"

#endif //FUNCTIONAL_MAKEUNIFORMDISTRIBUTION_HPP


namespace functfun
{
    // FIXME: add one overload for chrono duration
    template<typename A, typename B, std::integral C = std::common_type_t<A,B>,
             typename D = std::uniform_int_distribution<C>>
    constexpr auto make_uniform_distribution(A&& min, B&& max = std::numeric_limits<B>::max())->D
    {
        return D(min, max);
    }

    template<typename A, typename B, std::floating_point C = std::common_type_t<A,B>,
            typename D = std::uniform_real_distribution<C>>
    constexpr auto make_uniform_distribution(A&& min, B&& max = B{1.0})->D
    {
        return D(min, max);
    }

    // FIXME : not working yet
    template<typename T>
    class uniform_duration_distribution:std::uniform_int_distribution<int> {};

    template<>
    class uniform_duration_distribution<double>:std::uniform_real_distribution<double> {};

    template<typename A, typename B, typename C =std::common_type_t<A, B>,
             typename D = uniform_duration_distribution<C>>
    [[maybe_unused]] constexpr auto make_duration_distribution(A&& min, B&& max = B::max() )->D
    {
        return D(min, max);
    }




} // end of namespace