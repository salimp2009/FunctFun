//
// Created by salim on 01/04/2022.
//

#ifndef FUNCTIONAL_JOINWITHVIEW_HPP
#define FUNCTIONAL_JOINWITHVIEW_HPP

#include "functProgPCH.hpp"
#include "rangeconcepts.hpp"
#include "viewAdaptors.hpp"
#include "rangeutils.hpp"


namespace functfun
{
    template<std::ranges::input_range V, std::ranges::forward_range Pattern>
    requires std::ranges::view<V>
            && std::ranges::input_range<std::ranges::range_reference_t<V>>
            && std::ranges::view<Pattern>
            && compatible_joinable_ranges<std::ranges::range_reference_t<V>, Pattern>
    class joinwith_view : public std::ranges::view_interface<joinwith_view<V, Pattern>>
    {
        using InnerRng = std::ranges::range_reference_t<V>;

        V mbase =V();

        // this is used only when  !is_reference_v<InnerRng>;
        [[no_unique_address]] std::ranges::__detail::__non_propagating_cache<std::remove_cv_t<InnerRng>> inner;
        Pattern pattern = Pattern();

        template<bool Const>
        struct joinwith_iterator_cat{};

        template<bool Const>
            requires std::is_reference_v<std::ranges::range_reference_t<details::maybeConst_t<Const, V>>>
                    && std::ranges::forward_range<details::maybeConst_t<Const,V>>
                    && std::ranges::forward_range<details::maybeConst_t<Const, Pattern>>
        struct joinwith_iterator_cat<Const>
        {
            static std::bidirectional_iterator_tag cat(std::bidirectional_iterator_tag, std::bidirectional_iterator_tag, std::bidirectional_iterator_tag);
            static std::forward_iterator_tag cat(std::forward_iterator_tag, std::forward_iterator_tag, std::forward_iterator_tag);
            static std::input_iterator_tag cat(std::input_iterator_tag, std::input_iterator_tag, std::input_iterator_tag);

            using iterator_category =  decltype([]
                  {
                    using Base = details::maybeConst_t<Const, V>;

                    using OuterCat  = typename std::iterator_traits<std::ranges::iterator_t<Base>>::iterator_category;
                    using InnerCat  = typename std::iterator_traits<std::ranges::iterator_t<std::ranges::range_reference_t<Base>>>::iterator_category;
                    using PatternCat = typename std::iterator_traits<std::ranges::iterator_t<details::maybeConst_t<Const, Pattern>>>::iterator_category;

                    return decltype(cat(OuterCat{}, InnerCat{}, PatternCat{})){};
                  }());
        };

        template<bool Const>
        struct iterator : public joinwith_iterator_cat<Const>
        {
            using Parent        = details::maybeConst_t<Const, joinwith_view>;
            using Base          = details::maybeConst_t<Const, V>;
            using InnerBase     = std::ranges::range_reference_t<Base>;
            using PatternBase   = details::maybeConst_t<Const, Pattern>;

            using OuterIter     = std::ranges::iterator_t<Base>;
            using InnerIter     = std::ranges::iterator_t<InnerBase>;
            using PatternIter   = std::ranges::iterator_t<PatternBase>;
            static constexpr bool refIs_glvalue = std::is_reference_v<std::ranges::range_reference_t<Base>>;

            Parent* parent = nullptr;
            std::ranges::iterator_t<Base> outerIt = {};
            std::variant<PatternIter, InnerIter> innerIt = {};

            using iterator_concept = decltype([]
                  {
                    if constexpr (refIs_glvalue &&  bidi_common<Base> && bidi_common<InnerBase> && bidi_common<PatternBase>)
                    {
                        return std::bidirectional_iterator_tag{};
                    }
                    else if constexpr (refIs_glvalue && std::ranges::forward_range<Base> && std::ranges::forward_range<InnerBase>
                                                     && std::ranges::forward_range<PatternBase>)
                    {
                        std::forward_iterator_tag{};
                    }
                    else
                    {
                        return std::input_iterator_tag{};
                    }
                  }());

            using value_type = std::common_type_t<std::iter_value_t<InnerIter>, std::iter_value_t<PatternIter>>;
            using reference = std::common_reference_t<std::iter_reference_t<InnerIter>, std::iter_reference_t<PatternIter>>;
            using rvalue_reference = std::common_reference_t<std::iter_rvalue_reference_t<InnerIter>, std::iter_rvalue_reference_t<PatternIter>>;
            using difference_type = std::common_type_t<std::iter_difference_t<OuterIter>, std::iter_difference_t<InnerIter>, std::iter_difference_t<PatternIter>>;

            constexpr auto&& update_inner(std::ranges::iterator_t<Base> const& x)
            {
                if constexpr (refIs_glvalue)
                { return *x; }
                else
                { return parent->inner._M_emplace_deref(x);}
            }

            constexpr decltype(auto) get_inner()
            {
                if constexpr (refIs_glvalue)
                { return *outerIt; }
                else
                { return *parent->inner; }
            }

            constexpr void satisfy()
            {
                while(true)
                { // variant innerIt holds Pattern
                    if (innerIt.index()==0)
                    {
                        if(std::get<0>(innerIt) != std::ranges::end(parent->pattern))
                        { break;}
                        auto&& inner_ref = update_inner(outerIt);
                        innerIt.template emplace<1>(std::ranges::begin(inner_ref));
                    }
                    else
                    { // variant innerIt holds a range inside the outerRange
                        auto&& inner_ref = get_inner();
                        if(std::get<1>(innerIt) != std::ranges::end(inner_ref))
                        { break;}

                        if(++outerIt == std::ranges::end(parent->mbase))
                        {
                            if constexpr (refIs_glvalue)
                            { innerIt={}; }
                            break;
                        }
                        innerIt.template emplace<0>(std::ranges::begin(parent->pattern));
                    }
                }
            }

            iterator() requires std::ranges::forward_range<Base> = default;

            constexpr iterator(Parent& p, std::ranges::iterator_t<Base> outerit)
                            :parent{&p}, outerIt{std::move(outerit)}
            {
                if(outerIt != std::ranges::end(parent->mbase))
                {
                    auto&& inner_ref = update_inner(outerIt);
                    innerIt.template emplace<1>(std::ranges::begin(inner_ref));
                    satisfy();
                }
            }

            constexpr iterator(iterator<!Const> i) requires Const
                      && std::convertible_to<std::ranges::iterator_t<V>, OuterIter>
                      && std::convertible_to<std::ranges::iterator_t<InnerRng>, InnerIter>
                      && std::convertible_to<std::ranges::iterator_t<Pattern>, PatternIter>
                :outerIt{std::move(i.outerIt)}
            {
                if (i.innerIt.index()== 0) {
                    innerIt.template emplace<0>(std::get<0>(std::move(i.innerIt)));
                } else {
                    innerIt.template emplace<1>(std::get<1>(std::move(i.innerIt)));
                }
            }

            constexpr reference operator*() const {
                return std::visit([](auto& it) ->reference { return *it;}, innerIt);
            }

            constexpr iterator& operator++()  {
                std::visit([](auto& it) { ++it; }, innerIt);
                satisfy();
                return *this;
            }

            constexpr void operator++(int)  {
                ++*this;
            }

            constexpr iterator operator++(int) requires refIs_glvalue
                && std::forward_iterator<OuterIter> && std::forward_iterator<InnerIter> && std::forward_iterator<PatternIter>
            {
                auto temp =*this;
                ++*this;
                return temp;
            }

            constexpr iterator& operator--() requires refIs_glvalue
                && bidi_common<Base> && bidi_common<InnerBase> && bidi_common<Pattern>
            {
                if (outerIt == std::ranges::end(parent->mbase)) {
                    innerIt.template emplace<1>(std::ranges::end(*--outerIt));
                }

                while (true)
                {
                        // when Pattern is stored in variant
                        if (innerIt.index() == 0) {
                            auto &it = get<0>(innerIt);
                            if (it == std::ranges::begin(parent->pattern)) {
                                innerIt.template emplace<1>(std::ranges::end(*--outerIt));
                            } else {
                                break;
                            }
                        } else {
                            // when Range is stored in variant
                            auto &it = std::get<1>(innerIt);
                            if (it == ranges::begin(*outerIt)) {
                                innerIt.template emplace<0>(std::ranges::end(parent->pattern));
                            } else {
                                break;
                            }
                        }
                    }

                    std::visit([](auto &it) { --it; }, innerIt);
                    return *this;
            }

            constexpr iterator operator--(int) requires refIs_glvalue
                && bidi_common<Base> && bidi_common<InnerBase> && bidi_common<PatternBase>
            {
                auto temp = *this;
                ++*this;
                return temp;
            }

            friend constexpr bool operator==(const iterator& x, const iterator& y)
            requires refIs_glvalue && std::equality_comparable<OuterIter> && std::equality_comparable<InnerIter> {
                return x.outerIt == y.outerIt && x.innerIt == y.innerIt;
            }

            friend constexpr rvalue_reference iter_move(const iterator& x) {
                return std::visit<rvalue_reference>(std::ranges::iter_move, x.innerIt);
            }

            friend constexpr void iter_swap(const iterator& x, const iterator& y)
            requires std::indirectly_swappable<InnerIter, PatternIter> {
                return std::visit(std::ranges::iter_swap, x.innerIt, y.innerIt);
            }

        }; // endof iterator

        template<bool Const>
        struct sentinel
        {
            using Parent    = details::maybeConst_t<Const, joinwith_view>;
            using Base      = details::maybeConst_t<Const, V>;
            std::ranges::sentinel_t<Base> mEnd = std::ranges::sentinel_t<Base>();

            sentinel() = default;
            constexpr explicit sentinel(Parent& other_parent) : mEnd{std::ranges::end(other_parent.mbase)} {}
            constexpr sentinel(sentinel<!Const> s) requires Const
                && std::convertible_to<std::ranges::sentinel_t<V>, std::ranges::sentinel_t<Base>>
                : mEnd{std::move(s.mEnd)} { }

            template<bool Const2>
            requires std::sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<details::maybeConst_t<Const2, V>>>
            friend constexpr bool operator==(const iterator<Const2>& x, const sentinel& y) {
                return x.outerIt == y.mEnd;
            }
        }; // endof sentinel

    public:
        joinwith_view() requires std::default_initializable<V> && std::default_initializable<Pattern> = default;
        constexpr joinwith_view(V bas, Pattern pat) : mbase{std::move(bas)}, pattern{std::move(pat)} { }

        template<std::ranges::input_range R>
            requires std::constructible_from<V, std::ranges::views::all_t<R>>
                  && std::constructible_from<Pattern, std::ranges::single_view<std::ranges::range_value_t<InnerRng>>>
        constexpr joinwith_view(R&& r, std::ranges::range_value_t<InnerRng> p)
            : mbase{std::ranges::views::all(std::forward<R>(r))}, pattern{std::ranges::views::single(std::move(p))} { }

        constexpr V base() const& requires std::copy_constructible<V> { return mbase;}
        constexpr V base() && { return std::move(mbase);}

        constexpr auto begin() {
            constexpr bool useConst = details::simpleView<V> && std::is_reference_v<V>
                                    && details::simpleView<Pattern>;
                return iterator<useConst>{*this, std::ranges::begin(mbase)};
        }

        constexpr auto begin() const requires std::ranges::input_range<const V> &&
                std::ranges::forward_range<const Pattern> && std::is_reference_v<InnerRng>
        {
            return iterator<true>{*this, std::ranges::begin(mbase)};
        }

        constexpr auto end() {
            if constexpr (std::ranges::forward_range<V>
                       && std::is_reference_v<InnerRng> && std::ranges::forward_range<InnerRng>
                       && std::ranges::common_range<V> && std::ranges::common_range<InnerRng>) {
                return iterator<details::simpleView<V> && details::simpleView<Pattern>>{*this, std::ranges::end(mbase)};
            }  else {
                return sentinel<details::simpleView<V> && details::simpleView<Pattern>>{*this};
            }
        }

        constexpr auto end() const requires std::ranges::input_range<const V>
                                && std::ranges::forward_range<const Pattern>
                                && std::is_reference_v<std::ranges::range_reference_t<const V>> {
            if constexpr (std::ranges::forward_range<const V>
               && std::is_reference_v<std::ranges::range_reference_t<const V>> && std::ranges::forward_range<std::ranges::range_reference_t<const V>>
               && std::ranges::common_range<V> && std::ranges::common_range<std::ranges::range_reference_t<const V>>) {
                    return iterator<true>{*this, std::ranges::end(mbase)};
            } else {
                    return sentinel<true>{*this};
            }
        }
    }; //endof joinview

    template<typename R, typename P>
    joinwith_view(R&&, P&&) -> joinwith_view<std::ranges::views::all_t<R>, std::ranges::views::all_t<P>>;

    template<std::ranges::input_range R>
    joinwith_view(R&& , std::ranges::range_value_t<std::ranges::range_reference_t<R>>)
    -> joinwith_view<std::ranges::views::all_t<R>, std::ranges::single_view<std::ranges::range_value_t<std::ranges::range_reference_t<R>>>>;

    namespace views
    {
        namespace details
        {
            // FIXME : check if this is needed
            template<typename Range, typename Pattern>
            concept canJoinWith = requires { joinwith_view(std::declval<Range>(), std::declval<Pattern>()); };
        } // namespace details

        struct JoinWith : adaptor::RangeAdaptor<JoinWith>
        {
            template<std::ranges::viewable_range R, typename P>
            requires details::canJoinWith<R, P>
            auto operator()(R&& r, P&& p) const -> decltype(joinwith_view(std::forward<R>(r), std::forward<P>(p))) {
                return joinwith_view(std::forward<R>(r), std::forward<P>(p));
            }

            using adaptor::RangeAdaptor<JoinWith>::operator();
            static constexpr int Sarity =2;
        };

        inline constexpr JoinWith joinWith;
    }


} // endof namespace functfun

#endif//FUNCTIONAL_JOINWITHVIEW_HPP
