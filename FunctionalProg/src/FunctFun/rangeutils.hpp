//
// Created by salim on 27/03/2022.
//

#ifndef FUNCTIONAL_RANGEUTILS_HPP
#define FUNCTIONAL_RANGEUTILS_HPP

namespace functfun
{
    namespace details
    {
        template<typename T>
        concept boxable = std::copy_constructible<T> && std::is_object_v<T>;

        template<boxable T>
        struct optionalWrap : std::optional<T>
        {
            using std::optional<T>::optional;

            constexpr optionalWrap() noexcept(std::is_nothrow_default_constructible_v<T>)
                    requires std::default_initializable<T>
               : std::optional<T>{std::in_place} { }
            constexpr optionalWrap(const optionalWrap& ) =default;
            constexpr optionalWrap(optionalWrap&& ) =default;

        };

    } //endof namespace details



} // endof namespace functfun

#endif//FUNCTIONAL_RANGEUTILS_HPP
