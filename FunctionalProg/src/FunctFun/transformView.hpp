//
// Created by salim on 21/03/2022.
//

#ifndef FUNCTIONAL_TRANSFORMVIEW_HPP
#define FUNCTIONAL_TRANSFORMVIEW_HPP

#include "functProgPCH.hpp"

namespace functfun
{
    template<std::ranges::input_range V, std::copy_constructible F>
    requires std::ranges::view<V> &&
             std::is_object_v<F> && std::regular_invocable<F&, std::ranges::range_reference_t<V>>
    class map_view : public std::ranges::view_interface<map_view<V,F>>
    {
        template<bool IsConst>
        struct Iterator
        {

        };
        template<bool IsConst>
        struct Sentinel
        {

        };
        V mbase = V();
        [[no_unique_address]] F mfun;

    public:
        constexpr map_view()=default;
        constexpr map_view(V range, F fun) : mbase{std::move(range)}, mfun{std::move(fun)} { }

        constexpr V base() const& { return mbase; }
        constexpr V base() && { return std::move(mbase); }

        auto begin() -> Iterator<false> { return Iterator<false>{this, std::ranges::begin(mbase)};}
        auto end()   -> Sentinel<false> { return std::ranges::end(mbase);}
        auto end()   -> Iterator<false> requires std::ranges::common_range<V> { return std::ranges::end(mbase);}

        auto begin() const -> Iterator<true> requires std::ranges::range<V const> { return Iterator<true>{this, std::ranges::begin(mbase)};}
        auto end()   const -> Sentinel<true> requires std::ranges::range<V const> { return std::ranges::end(mbase);}
        auto end()   const -> Iterator<true> requires std::ranges::common_range<V const> { return std::ranges::end(mbase);}

    };






} // end of namespace functfun


#endif//FUNCTIONAL_TRANSFORMVIEW_HPP
