//
// Created by salim on 27/03/2022.
//

#ifndef FUNCTIONAL_RANGEUTILS_HPP
#define FUNCTIONAL_RANGEUTILS_HPP

#include "functProgPCH.hpp"
#include "functOptional.hpp"

namespace functfun
{
    namespace details
    {
        template<typename T>
        concept wrapable = std::copy_constructible<T> && std::is_object_v<T>;

        template<typename T>
        concept notCopyable = not std::copyable<T>;

        template<typename T>
        concept notMovable = not std::movable<T>;

        template<wrapable T>
        struct optionalWrap : std::optional<T>
        {
            using std::optional<T>::optional;

            constexpr optionalWrap() noexcept(std::is_nothrow_default_constructible_v<T>)
            requires std::default_initializable<T>
               : std::optional<T>{std::in_place} { }

            constexpr optionalWrap(const optionalWrap& ) = default;

            // FIXME: clang tidy warning to make noexcept but
            //  we need specify the conditional noexcept which maynot be defaulted then??
            constexpr optionalWrap(optionalWrap&& ) = default;

            using std::optional<T>::operator=;

            constexpr optionalWrap& operator=(const optionalWrap& other)
            noexcept(std::is_nothrow_copy_constructible_v<T>) requires notCopyable<T>
            {
                if(static_cast<bool>(other))
                { this->emplace(*other); }
                else
                { this->reset();}
                return *this;
            }

            constexpr optionalWrap& operator=(optionalWrap&& other)
            noexcept(std::is_nothrow_move_constructible_v<T>) requires notMovable<T>
            {
                if(static_cast<bool>(other))
                { this->emplace(std::move(*other)); }
                else
                { this->reset();}
                return *this;
            }
        };

        // For types that are semiregular no need for std::optional
        template<wrapable T>
        requires std::semiregular<T>
        struct optionalWrap<T>
        {
        private:
            [[no_unique_address]] T mValue = T();

        public:
            constexpr optionalWrap()=default;

            constexpr explicit optionalWrap(const T& val)
            noexcept(std::is_nothrow_copy_constructible_v<T>)
            :mValue{val} {}

            constexpr explicit  optionalWrap(T&& val)
            noexcept(std::is_nothrow_move_constructible_v<T>)
            :mValue{std::move(val)} {}

            template<typename... Args>
            requires std::constructible_from<T, Args...>
            constexpr explicit optionalWrap(std::in_place_t, Args&&... args)
            noexcept(std::is_nothrow_constructible_v<T, Args...>)
            :mValue{std::forward<Args>(args)...} { }

            constexpr bool has_value() const noexcept { return true;}

            constexpr T& operator*() noexcept { return mValue; }
            constexpr const T& operator*() const noexcept { return mValue; }

            constexpr T* operator->() noexcept { return std::addressof(mValue);}
            constexpr const T* operator->() const noexcept { return std::addressof(mValue);}

        };

        template<typename T>
        struct nonpropagating_cache
        {
            // when T is not an object; e.g a reference type,
            // nonpropagating cache will be used as an empty struct
            // to be able to declare member functions
            // used GCC-11.2 implementation for reference
        };


        template<typename T>
        requires std::is_object_v<T>
        struct nonpropagating_cache<T> : protected OptionalBase<T>
        {
            nonpropagating_cache() = default;

            constexpr nonpropagating_cache(const nonpropagating_cache&) noexcept { }

            constexpr nonpropagating_cache(const nonpropagating_cache&& other) noexcept
            { other.mreset(); }

            constexpr nonpropagating_cache& operator=(nonpropagating_cache& other) noexcept
            {
                if(std::addressof(other)!=this) { this -> mreset();}
                return *this;
            }

            constexpr nonpropagating_cache& operator=(nonpropagating_cache&& other) noexcept
            {
                this->mreset();
                other->mreset();
                return *this;
            }

            constexpr T& operator*() noexcept
            { return this->mget();}

            template<typename Iter>
            constexpr T& emplace_deref(const Iter& iter)
            {
                this->mreset();
                // similar to GCC-11.2 implementation ;not using OptionalBase
                // to initialize instead to avoid extra move
                // also optimized for constexpr from Bary Revzin's paper P2210R2
                // that paper implemented construct_at optional_base
                // FIXME: check if; this-> mpayload.mpayload.mvalue is correct
                //  otherwise original was; this->mpayload.mpayload
                std::construct_at(std::addressof(this->mpayload.mpayload.mvalue), *iter);
                this->mpayload.mengaged = true;
                return this ->mget();
            }

        };

    } //endof namespace details

} // endof namespace functfun

#endif//FUNCTIONAL_RANGEUTILS_HPP
