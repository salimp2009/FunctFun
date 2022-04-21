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


            // FIXME: THIS IS ALTERNATIVE; tuple_or_pair; this seems to be working;
            //  if final implementation has a problem with reference and value_type use this;
            // ALTERNATIVE to tuple_or_pair
            /*
            //            template<typename...Ts>
            //            static auto tuple_or_pair_Alt = []{
            //                if constexpr (sizeof...(Ts)>2) {
            //                    return std::type_identity<std::tuple<Ts...>>::type;
            //                } else {
            //                    return std::type_identity<std::pair<Ts...>>::type;
            //                }
            //            };
            //
            //            template<typename... Ts>
            //            using tuple_or_pair_type = decltype(tuple_or_pair_Alt<Ts...>());
             */

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

            using reference = tuple_or_pair<std::ranges::range_reference_t<maybeConst_t<Const, Vs>>...>;
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
                requires ((std::ranges::bidirectional_range<maybeConst_t<Const, Vs>> && ...)) {
                prev();
                return *this;
            }

            constexpr iterator operator--(int)
                requires ((std::ranges::bidirectional_range<maybeConst_t<Const, Vs>> && ...)) {
                auto tmp = *this;
                --*this;
                return tmp;
            }

            constexpr iterator& operator+=(difference_type n)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                advance(n);
                return *this;
            }

            constexpr iterator& operator-=(difference_type n)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                advance(-n);
                return *this;
            }

            // FIXME: originally reference is used but there is a problem with tuple_or_pair;
            //  created another alternate and it works
            constexpr decltype(auto) operator[](difference_type n) const
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                return *((*this) + n);
            }

            friend constexpr bool operator==(const iterator& x, const iterator& y)
                requires ((std::equality_comparable<std::ranges::iterator_t<maybeConst_t<Const, Vs>>> && ...)) {
                 return x.mcurrent == y.mcurrent;
            }

            friend constexpr auto operator<(const iterator& x, const iterator& y)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                return x.mcurrent < y.mcurrent;
            }

            friend constexpr auto operator>(const iterator& x, const iterator& y)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                return y < x;
            }

            friend constexpr auto operator<=(const iterator& x, const iterator& y)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                return !(y < x);
            }

            friend constexpr auto operator>=(const iterator& x, const iterator& y)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                return !(x < y);
            }

            friend constexpr auto operator<=>(const iterator& x, const iterator& y)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...))
                         && ((std::three_way_comparable<maybeConst_t<Const, Vs>> && ...)) {
                return x.mcurrent <=> y.mcurrent;
            }

            friend constexpr iterator operator+(const iterator x, difference_type n)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                 return iterator{ x } += n ;
            }

            friend constexpr iterator operator+(difference_type n, const iterator x)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                return  x + n ;
            }

            friend constexpr iterator operator-(const iterator x, difference_type n)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                return iterator{x} -= n;
            }

            // FIXME: change the constraint in requires clause and not sure
            //  why the error show call to function with no params; probably has to be called like
            //  y.scaled_difference(x) because you cannot access a friend's functions directly unless it is static
            friend constexpr difference_type operator-(const iterator& x, const iterator& y)
                requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                return scaled_distance(x, y);
            }

            // FIXME : replace this with the existing implementations ;
            //  this is not efficient as them; the other takes only iterator less copy
            template<size_t N = (sizeof...(Vs)-1) >
            constexpr auto scaled_distance(const iterator& x, const iterator& y) {
                difference_type sum{0};
                sum += (std::get<N>(x.mcurrent) - std::get<N>(y.mcurrent) ) * std::get<N>(x.mparent->mbases);
                if constexpr (N > 0) {
                    scaled_distance<N-1>(x,y);
                }
                return sum;
            }

            template<std::size_t N = (sizeof...(Vs)-1) >
            void advance(difference_type n) requires ((std::ranges::random_access_range<maybeConst_t<Const, Vs>> && ...)) {
                auto& it    = std::get<N>(mcurrent);
                auto& base  = std::get<N>(mparent->mbases);
                auto begin  = std::ranges::begin(base);
                auto end    = std::ranges::end(base);
                auto size   = end -begin;

                auto distance_from_begin = it - begin;
                // the position we can be when we advance n times should not exceed the end
                auto offset = (distance_from_begin+n) % size;
                // how many times we need to advance it; will negative if we are decrementing
                auto times_cycled = (distance_from_begin + n) / size - (offset < 0 ? 1:0);
                it = begin + static_cast<difference_type>(offset <0 ? offset+size : offset );

                if constexpr (N > 0) {
                    if( times_cycled !=0){
                        advance<N-1>(times_cycled);
                    }
                } else {
                    if(times_cycled > 0) {
                        std::ranges::advance(it, end);
                    }
                }
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
