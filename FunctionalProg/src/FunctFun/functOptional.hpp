//
// Created by salim on 08/04/2022.
//

#ifndef FUNCTIONAL_FUNCTOPTIONAL_HPP
#define FUNCTIONAL_FUNCTOPTIONAL_HPP

#include "functProgPCH.hpp"

namespace functfun
{

    template<typename T>
    struct OptionalPayload_base
    {
        using storedType = std::remove_const_t<T>;

        OptionalPayload_base()  = default;
        ~OptionalPayload_base() = default;

        template<typename... Args>
        constexpr OptionalPayload_base(std::in_place_t tag, Args&&... args) { }

        struct EmptyByte { };

        template<typename U>
        union Storage
        {
            constexpr Storage() noexcept : mempty() { }

            template<typename... Args>
            constexpr Storage(std::in_place_t, Args&&... args)
                : mvalue{std::forward<Args>(args)...} { }

            template<typename V, typename... Args>
            constexpr Storage(std::initializer_list<V> il, Args&&... args)
                : mvalue{il, std::forward<Args>(args)...} { }

            ~Storage() requires (not std::is_trivially_destructible_v<U>)
             { }

            EmptyByte mempty;
            U mvalue;
        };

        Storage<storedType> mpayload;
        bool mengaged = false;

        template<typename... Args>
        constexpr void mconstruct(Args&&... args)
        noexcept(std::is_nothrow_constructible_v<storedType, Args...>)
        {
            std::construct_at(std::addressof(this->mpayload), std::forward<Args>(args)...);
            this->mengaged = true;
        }
    };

    template<typename T,
            bool = std::is_trivially_destructible_v<T>,
            bool = std::is_trivially_copy_assignable_v<T> && std::is_copy_constructible_v<T>,
            bool = std::is_trivially_move_assignable_v<T> &&  std::is_trivially_move_constructible_v<T> >
    struct OptionalPayload;

    // payload for constexpr; trivial copy/move/destroy
    template<typename T>
    struct OptionalPayload<T, true, true, true> : OptionalPayload_base<T>
    {
        using OptionalPayload_base<T>::OptionalPayload_base;
        OptionalPayload() = default;
    };

    template<typename T, typename Derived>
    class OptionalBase_impl
    {
    protected:
        using storedType = std::remove_const_t<T>;

    //  mconstruct operation has engaged = false precondition
    //  mdestruct operatiion has engaged = true precondtion
        template<typename... Args>
        constexpr void mconstruct(Args&&... args)
            noexcept(std::is_nothrow_constructible_v<storedType, Args...>)
        {
            std::construct_at(std::addressof(static_cast<Derived*>(this)->mpayload.mpayload.mvalue), std::forward<Args>(args)...);
        }

        constexpr void mdestruct() noexcept
        { static_cast<Derived*>(this)->mpayload.mdestroy(); }

        // noprecondition safe ops
        constexpr void mreset() noexcept
        { static_cast<Derived*>(this)->mpayload.mreset(); }

        // FIXME: these getters & setters looks unneccesary
        constexpr bool m_isengaged() const noexcept
        { return static_cast<Derived*>(this)->mpayload.mengaged; }

        constexpr T& mget() & noexcept
        {
            assert(m_isengaged());
            return static_cast<Derived*>(this)->mpayload.mget();
        }

        constexpr const T& mget() const& noexcept
        {
            assert(m_isengaged());
            return static_cast<Derived*>(this)->mpayload.mget();
        }
    };


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
