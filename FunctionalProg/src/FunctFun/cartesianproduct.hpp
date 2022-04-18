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
            void prev() {
                auto& it = std::get<N>(mcurrent);
                if(it == std::ranges::begin(std::get<N>(mparent->mbase))) {
                    it = std::ranges::end(std::get<N>(mparent->mbases));
                    if constexpr (N > 0) {
                        prev<N-1>();
                    }
                }
                --it;
            }

            using iterator_category = std::input_iterator_tag;

            //  FIXME : check if this work; my implementation
            using iterator_concept = decltype([](){
                if constexpr ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                    return std::random_access_iterator_tag{};
                }
                else if constexpr ((std::ranges::bidirectional_range<maybeConst_t<Const, Vs>> && ...)) {
                    return std::bidirectional_iterator_tag{};
                }
                else {
                    return std::forward_iterator_tag{};
                }
            }());

            using value_type = tuple_or_pair<std::ranges::range_value_t<maybeConst_t<Const, Vs>>...>;
            using difference_type = std::common_type_t<std::ranges::range_difference_t<maybeConst_t<Const, Vs>>...>;

            iterator()=default;

            constexpr explicit iterator(tuple_or_pair<std::ranges::iterator_t<maybeConst_t<Const, Vs>>...> current)
                :mcurrent{std::move(current)} { }

            constexpr iterator(iterator<!Const> i) requires Const
                 && (std::convertible_to<std::ranges::iterator_t<Vs>, std::ranges::iterator_t<maybeConst_t<Const, Vs>>> && ...)
                :mcurrent{std::move(i.mcurrent)} { }

            constexpr auto operator*() const {
                return tuple_transform([](auto& it) ->decltype(auto){
                    return *it; }, mcurrent);
            }

            constexpr iterator& operator++() {
                next();
                return *this;
            }

            constexpr iterator operator++(int) {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--()
                requires (std::ranges::bidirectional_range<maybeConst_t<Const, Vs>> && ...) {
                prev();
                return *this;
            }

            constexpr iterator operator--(int)
                requires (std::ranges::bidirectional_range<maybeConst_t<Const, Vs>> && ...) {
                auto tmp = *this;
                --*this;
                return tmp;
            }




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

        constexpr std::default_sentinel_t end() const
            requires (!product_common_or_random<const Vs...>) {
                return {};
        }

        constexpr auto size()
            requires (std::ranges::sized_range<Vs> && ...) {
            //Multiply all the sizes together, returning the common type of all of them
            return std::apply([](auto&&... ranges){
                using size_type = std::common_type<std::ranges::range_size_t<decltype(ranges)>...>;
                return (static_cast<size_type>(std::ranges::size(ranges)) * ...);
            }, mbases );
        }

        constexpr auto size() const
            requires (std::ranges::sized_range<const Vs> && ...) {
            //Multiply all the sizes together, returning the common type of all of them
            return std::apply([](auto&&... ranges){
                using size_type = std::common_type<std::ranges::range_size_t<decltype(ranges)>...>;
                return (static_cast<size_type>(std::ranges::size(ranges)) * ...);
            }, mbases );
        }

    }; // endof cartesianproduct_view

    template<class... Vs>
    cartesianproduct_view(Vs&&...) -> cartesianproduct_view<std::views::all_t<Vs>...>;



} // endof namespace functfun


#endif//FUNCTIONAL_CARTESIANPRODUCT_HPP
