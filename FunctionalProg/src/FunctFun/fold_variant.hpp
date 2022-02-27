//
// Created by salim on 27/02/2022.
//

#ifndef FUNCTIONAL_FOLD_VARIANT_HPP
#define FUNCTIONAL_FOLD_VARIANT_HPP

#include "functProgPCH.hpp"

namespace functfun
{

    template<typename T, typename F, typename...Fs>
    auto fold_at(T&& jsv, std::size_t jsvIndex, F&& f, Fs&&...fs)
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

    // FIXME: to be implemented...
    template<typename... Ts, typename... Fs>
    auto fold_variant(const std::variant<Ts...>& jsv, Fs&&... fs)
    {
        static_assert(sizeof...(Ts)== sizeof...(Fs));
        return fold_at(jsv, jsv.index(), std::forward<Fs>(fs)...);
    }






} // end of namespace functfun

#endif//FUNCTIONAL_FOLD_VARIANT_HPP
