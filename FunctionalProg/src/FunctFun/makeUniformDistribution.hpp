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
    template<typename A, typename B=A, std::integral C = std::common_type_t<A,B>,
             typename D = std::uniform_int_distribution<C>>
    inline constexpr auto make_uniform_distribution(A&& min, B&& max = std::numeric_limits<B>::max())->D
    {
        return D(min, max);
    }

    template<typename A, typename B=A, std::floating_point C = std::common_type_t<A,B>,
            typename D = std::uniform_real_distribution<C>>
    inline constexpr auto make_uniform_distribution(A&& min, B&& max = B{1.0})->D
    {
        return D(min, max);
    }

    template<typename T>
    concept ChronoDurationDouble  = std::chrono::treat_as_floating_point_v<typename T::rep>;

    template<typename T>
    concept ChronoDurationInt     = not ChronoDurationDouble<T>;

    template<ChronoDurationInt A, ChronoDurationInt B=A, ChronoDurationInt C = std::common_type_t<A,B>,
            typename D = std::uniform_int_distribution<typename C::rep>>
    inline constexpr auto make_uniform_distribution(A&& min, B&& max = B::max())->D
    {
        return D(min.count(), max.count());
    }

    template<ChronoDurationDouble A, ChronoDurationDouble B=A, ChronoDurationDouble C = std::common_type_t<A,B>,
            typename D = std::uniform_real_distribution<typename C::rep>>
    inline constexpr auto make_uniform_distribution(A&& min, B&& max = B{1.0})->D
    {
        return D(min.count(), max.count());
    }





} // end of namespace