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

        V base=V();

        // this is used only when  !is_reference_v<InnerRng>;
        // when outer range is a simple range [a] not [[a]]
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
                        // FIXME : changed from inner to inner_ref to prevent shadowing member inner
                        auto&& inner_ref = update_inner(outerIt);
                        innerIt.template emplace<1>(std::ranges::begin(inner_ref));
                    }
                    else
                    { // variant innerIt holds a range inside the outerRange
                        // FIXME : changed from inner to inner_ref to prevent shadowing member inner
                        auto&& inner_ref = get_inner();
                        if(std::get<1>(innerIt) != std::ranges::end(inner_ref))
                        { break;}

                        if(++outerIt == std::ranges::end(parent->base))
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
                if(outerIt != std::ranges::end(parent->base))
                {
                    // FIXME : changed from inner to inner_ref to prevent shadowing member inner
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

            constexpr iterator& operator++() const {
                return std::visit([](auto& it) ->reference { ++it; }, innerIt);
                satisfy();
                return *this;
            }

            constexpr void operator++()  {
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
                if (outerIt == std::ranges::end(parent->base)) {
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
            requires refIs_glvalue && std::equality_comparable<Base> && std::equality_comparable<InnerBase> && std::equality_comparable<PatternBase> {
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


    }; //endof joinview


} // endof namespace functfun

#endif//FUNCTIONAL_JOINWITHVIEW_HPP
