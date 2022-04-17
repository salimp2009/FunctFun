//
// Created by salim on 15/04/2022.
//

#ifndef FUNCTIONAL_CARTESIANPRODUCT_HPP
#define FUNCTIONAL_CARTESIANPRODUCT_HPP

#include "functProgPCH.hpp"
#include "rangeconcepts.hpp"

namespace functfun
{
    template<class... Ts>
    using tuple_or_pair = typename decltype([]{
        if constexpr (sizeof...(Ts) == 2) {
            return std::type_identity<std::pair<Ts...>>{};
        } else {
            return std::type_identity<std::tuple<Ts...>>{};
        }
    }())::type;

    template<class F, class Tuple>
    constexpr auto tuple_transform(F&& f, Tuple&& tuple) {
        return std::apply([&]<class...Ts>(Ts&&... elems){
            return tuple_or_pair<std::invoke_result_t<F&, Ts>...>(
                    std::invoke(f, std::forward<Ts>(elems))...);
        }, std::forward<Tuple>(tuple));
    }

    template<class F, class Tuple>
    constexpr void tuple_for_each(F&& f, Tuple&& tuple) {
        std::apply([&]<class... Ts>(Ts&&... elems){
            (std::invoke(f, std::forward<Ts>(elems)), ...);
        }, std::forward<Tuple>(tuple));
    }


    template<std::ranges::forward_range... Vs>
    requires (std::ranges::view<Vs> && ...)
    class cartesianproduct_view : public std::ranges::view_interface<Vs...>
    {
    private:
        std::tuple<Vs...> mbases;

        template<bool Const>
        struct iterator
        {
            maybeConst_t<Const, cartesianproduct_view>* mparent;
            tuple_or_pair<std::ranges::iterator_t<maybeConst_t<Const, Vs>>...> mcurrent{};

            template<std::size_t N = (sizeof...(Vs)-1)>
            void next() {
                auto& it = std::get<N>(mcurrent);
                ++it;
                if constexpr (N > 0) {
                    if(it==ranges::end(std::get<N>(mparent->mbases))) {
                        it = ranges::begin(std::get<N>(mparent->mbases));
                        next<N-1>();
                    }
                }
            }

            template<std::size_t N = (sizeof...(Vs)-1)>
            void prev();

        }; // endof iterator

        template<bool Const>
        struct sentinel;
    public:
        constexpr cartesianproduct_view()=default;
        constexpr cartesianproduct_view(Vs... bases): mbases{std::move(bases)...} { }

        // simple view is when constness does not matter
        constexpr iterator<false> begin() requires(!simpleView<Vs> || ...) {
            return iterator<false>(tuple_transform(std::ranges::begin, mbases));
        }

        constexpr iterator<true> begin() requires (std::ranges::range<Vs> && ...) {
            return iterator<true>(tuple_transform(std::ranges::begin, mbases));
        }

        constexpr iterator<false> end()
            requires (!simpleView<Vs> || ...) && product_common_or_random<Vs...> {
                iterator<false> it{tuple_transform(std::ranges::begin, mbases)};
                std::get<0>(it.current) = std::ranges::end(std::get<0>(mbases));
                return it;
        }

        constexpr iterator<true> end() const
            requires product_common_or_random<const Vs...> {
            iterator<true> it{tuple_transform(std::ranges::begin, mbases)};
            std::get<0>(it.mcurrent) = std::ranges::end(std::get<0>(mbases));
            return it;
        }



    }; // endof cartesianproduct_view



} // endof namespace functfun


#endif//FUNCTIONAL_CARTESIANPRODUCT_HPP
