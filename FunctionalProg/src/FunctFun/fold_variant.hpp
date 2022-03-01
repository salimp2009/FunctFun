//
// Created by salim on 27/02/2022.
//

#ifndef FUNCTIONAL_FOLD_VARIANT_HPP
#define FUNCTIONAL_FOLD_VARIANT_HPP

#include "functProgPCH.hpp"

namespace functfun
{
    // Alternative; my version but less efficient
    template<typename T, typename F, typename...Fs>
    auto fold_at_Alt(T&& jsv, std::size_t jsvIndex, F&& f, Fs&&...fs)
    {
        if( jsvIndex==0) return std::invoke(std::forward<F>(f), std::get<0>(jsv));

        return [&]<std::size_t...Idx> (std::index_sequence<Idx...>)
        {
            std::string result{};
            ([&]()
                {
                     if (Idx+1==jsvIndex)
                     {
                        result = std::invoke(fs, std::get<Idx+1>(jsv));
                     }

                }(),...);
            return result;

        }(std::make_index_sequence<sizeof...(fs)>{});
    }

    // FIXME: to be tested :)
     template<std::size_t N, std::size_t TotalFs>
    struct apply_at
    {
        template<typename Result, typename T, typename F, typename... Fs>
        constexpr static auto apply(T&& jsv, std::size_t n, F&& f, Fs&&... fs)
        {
            if (n == N)
            {
                return std::invoke(std::forward<F>(f), std::get<N>(std::forward<T>(jsv)));
            }
            else
            {
                return apply_at<N+1, TotalFs>:: template apply<Result, T, Fs...>(
                         std::forward<T>(jsv),
                         n,
                         std::forward<Fs>(fs)...);
             }

        }
    };

    template<std::size_t TotalFs>
    struct apply_at<TotalFs, TotalFs>
    {
        template<typename Result, typename T, typename... Fs>
        constexpr static auto apply(T, std::size_t, Fs...)
        {
            assert("variant index out of range" && false);
            return Result{};
        }
    };


    template<typename T, typename F, typename...Fs>
    auto fold_at(T&& jsv, std::size_t jsvIndex, F&& f, Fs&&...fs)
    {
        using Result = decltype(std::invoke(f, std::get<0>(jsv)));
        return apply_at<0, sizeof...(Fs)+1>::template apply<Result, T, F, Fs...>(
                std::forward<T>(jsv),
                jsvIndex,
                std::forward<F>(f),
                std::forward<Fs>(fs)...);
    }


    template<typename... Ts, typename... Fs>
    auto fold_variant(const std::variant<Ts...>& jsv, Fs&&... fs)
    {
        static_assert(sizeof...(Ts)== sizeof...(Fs));
        return fold_at(jsv, jsv.index(), std::forward<Fs>(fs)...);
    }




} // end of namespace functfun

#endif//FUNCTIONAL_FOLD_VARIANT_HPP
