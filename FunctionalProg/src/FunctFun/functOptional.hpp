//
// Created by salim on 08/04/2022.
//

#ifndef FUNCTIONAL_FUNCTOPTIONAL_HPP
#define FUNCTIONAL_FUNCTOPTIONAL_HPP

#include "functProgPCH.hpp"

namespace functfun
{
    template<typename T,
            bool = std::is_trivially_copy_constructible_v<T>,
            bool = std::is_trivially_move_constructible_v<T>>
    struct OptionalBase : std::_Optional_base_impl<T, OptionalBase<T>>
    {
        // disengaged optional constructor
        constexpr OptionalBase() = default;

        // engaged optional constructors
        template<typename... Args>
        requires std::is_constructible_v<T, Args...>
        constexpr explicit OptionalBase(std::in_place_t, Args&&... args)
               : mpayload{std::in_place, std::forward<Args>(args)...} { }

        template<typename U, typename... Args>
        requires std::is_constructible_v<T, std::initializer_list<U>&, Args...>
        constexpr explicit OptionalBase(std::in_place_t, std::initializer_list<U> il, Args&&...args)
                :mpayload{std::in_place, il, std::forward<Args>(args)...} { }

        // Copy & move constrs
        constexpr OptionalBase(const OptionalBase& other)
                :mpayload{other._M_payload._M_engaged, other._M_payload} { }

        constexpr OptionalBase(OptionalBase&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
            :mpayload{other._M_payload._M_engaged, std::move(other._M_payload)} { }

        // FIXME :check if constexpr needed here (probably not)
        OptionalBase& operator=(const OptionalBase&) = default;
        OptionalBase& operator=(OptionalBase&&) = default;

        std::_Optional_payload<T> mpayload;
    };



} // endof namespace functfun

#endif//FUNCTIONAL_FUNCTOPTIONAL_HPP
