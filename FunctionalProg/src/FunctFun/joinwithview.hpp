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
        };

    };


} // endof namespace functfun

#endif//FUNCTIONAL_JOINWITHVIEW_HPP
