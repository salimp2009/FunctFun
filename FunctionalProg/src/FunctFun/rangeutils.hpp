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

            // FIXME: clang tidy warn to make noexcept but the we need
            //  to specify the conditional noexcept which maynot be defaulted then??
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

        // For type that are semiregular no need for optional
        template<wrapable T>
        requires std::semiregular<T>
        struct optionalWrap<T>
        {
        private:
            [[no_unique_address]] T mValue = T();

        public:
            constexpr optionalWrap()=default;

        };



    } //endof namespace details



} // endof namespace functfun

#endif//FUNCTIONAL_RANGEUTILS_HPP
