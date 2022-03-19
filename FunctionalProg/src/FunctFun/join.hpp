//
// Created by salim on 05/03/2022.
//

#ifndef FUNCTIONAL_JOIN_HPP
#define FUNCTIONAL_JOIN_HPP

#include "functProgPCH.hpp"


namespace functfun
{
    struct join_func
    {
        template<std::input_iterator It, std::sentinel_for<It> S, typename OutputIt, typename T, class Proj= std::identity,
                 std::indirectly_unary_invocable<std::projected<It, Proj>> Pred>
        requires std::indirectly_writable<OutputIt, typename std::indirect_result_t<Pred&, std::projected<It, Proj>>::value_type> &&
                 std::indirectly_writable<OutputIt, T>
        constexpr auto operator()(It first, S last, OutputIt dest, T&& delimiter, Pred pred, Proj proj= {}) const ->std::ranges::unary_transform_result<It, OutputIt>
        {
            if(first == last) return {std::move(first), std::move(dest)};

            for(; first !=last; ++first)
            {
                auto&& newStr = std::invoke(std::forward<Pred>(pred), std::invoke(proj, *first));
                *dest = delimiter;
                std::accumulate(std::begin(newStr), std::end(newStr), dest,
                                [](auto&& itStr, auto&& elem) { return *itStr = std::forward<decltype(elem)>(elem);} );
            }

            return {std::move(first), std::move(dest)};
        }

        // FIXME: turn some of those requires clauses to concepts; maybe create a inner_type_t that those checks
        template<std::input_iterator It, std::sentinel_for<It> S, typename OutputIt, typename T, class Proj= std::identity,
                 std::indirectly_unary_invocable<std::projected<It, Proj>> Pred>
        requires std::indirectly_writable<OutputIt, typename std::indirect_result_t<Pred&, std::projected<It, Proj>>::value_type> &&
                 (std::indirectly_writable<OutputIt, std::iter_value_t<T>> || std::indirectly_writable<OutputIt, std::remove_cvref_t<decltype(*std::begin(std::declval<T>()))>>)
        constexpr auto operator()(It first, S last, OutputIt dest, T&& delimiter, Pred pred, Proj proj= {}) const ->std::ranges::unary_transform_result<It, OutputIt>
        {
            if(first == last) return {std::move(first), std::move(dest)};

            for(; first !=last; ++first)
            {
                auto&& newStr = std::invoke(std::forward<Pred>(pred), std::invoke(proj, *first));
                std::accumulate(std::begin(delimiter), std::prev(std::end(delimiter)), dest, [](auto&& outDest, const auto& elem) { return *outDest= elem;});
                std::accumulate(std::begin(newStr), std::end(newStr), dest,
                                [](auto&& itStr, auto&& elem) { return *itStr = std::forward<decltype(elem)>(elem);});
            }

            return {std::move(first), std::move(dest)};
        }


        template<std::ranges::input_range Rng, typename OutputIt, typename T, class Proj= std::identity,
                 std::indirectly_unary_invocable<std::projected<std::ranges::iterator_t<Rng>, Proj>> Pred>
        requires std::indirectly_writable<OutputIt, typename std::indirect_result_t<Pred&, std::projected<std::ranges::iterator_t<Rng>, Proj>>::value_type> &&
                (std::indirectly_writable<OutputIt, T> || std::indirectly_writable<OutputIt, std::iter_value_t<T>> || std::indirectly_writable<OutputIt, std::remove_cvref_t<decltype(*std::begin(std::declval<T>()))>>)
        constexpr auto operator()(Rng&& rng, OutputIt dest, T&& delimiter, Pred pred, Proj proj={}) const -> std::ranges::unary_transform_result<std::ranges::borrowed_iterator_t<Rng>, OutputIt>
        {
            return (*this)(std::ranges::begin(rng), std::ranges::end(rng), dest,
                           std::move(delimiter), std::move(pred), std::move(proj));
        }

    };

    inline constexpr join_func join{};

} // end of namespace functfun

#endif//FUNCTIONAL_JOIN_HPP
