//
// Created by salim on 21/03/2022.
//

#ifndef FUNCTIONAL_TRANSFORMVIEW_HPP
#define FUNCTIONAL_TRANSFORMVIEW_HPP

#include "functProgPCH.hpp"

namespace functfun::details
{
    template<typename T>
    using withRef = T&;

    template<typename T>
    concept canReference = requires { typename withRef<T>; };

    template<typename T>
    concept deReferancable = requires (T a)
    {
        { *a } -> canReference;
    };

    template<bool IsConst, typename T>
    using maybeConst_t = std::conditional_t<IsConst, const T, T>;

    struct Empty {};

    template<bool IsPresent, typename T>
    using maybePresent_t = std::conditional_t<IsPresent, T, Empty>;



} // end of namespace details



namespace functfun
{
    template<std::ranges::input_range V, std::copy_constructible F>
    requires std::ranges::view<V> &&
             std::is_object_v<F> && std::regular_invocable<F&, std::ranges::range_reference_t<V>> &&
             details::canReference<std::invoke_result_t<F&, std::ranges::range_reference_t<V>>>
    class map_view : public std::ranges::view_interface<map_view<V,F>>
    {
        template<bool IsConst>
        using mBase = details::maybeConst_t<IsConst, V>;

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
        constexpr map_view() requires std::default_initializable<V> && std::default_initializable<F> =default;
        constexpr map_view(V range, F fun) : mbase{std::move(range)}, mfun{std::move(fun)} { }

        constexpr V base() const& requires std::copy_constructible<V> { return mbase; }
        constexpr V base() && { return std::move(mbase); }

        auto begin() -> Iterator<false> { return Iterator<false>{this, std::ranges::begin(mbase)};}
        auto end()   -> Sentinel<false> { return Sentinel<false>{std::ranges::end(mbase)};}
        auto end()   -> Iterator<false> requires std::ranges::common_range<V> { return Iterator<false>{this, std::ranges::end(mbase)};}

        auto begin() const -> Iterator<true> requires std::ranges::range<V const> && std::regular_invocable<const F&, std::ranges::range_reference_t<const V>>
        { return Iterator<true>{this, std::ranges::begin(mbase)};}

        auto end()   const -> Sentinel<true> requires std::ranges::range<V const> && std::regular_invocable<const F&, std::ranges::range_reference_t<const V>>
        { return Sentinel<true>{std::ranges::end(mbase)};}

        auto end()   const -> Iterator<true> requires std::ranges::common_range<V const> && std::regular_invocable<const F&, std::ranges::range_reference_t<const V>>
        { return Iterator<true>{this, std::ranges::end(mbase)};}

        constexpr auto size()       requires std::ranges::sized_range<V> { return std::ranges::size(mbase);}
        constexpr auto size() const requires std::ranges::sized_range<V> { return std::ranges::size(mbase);}

    };

    template<typename Range, typename Fn>
    map_view(Range&&, Fn) -> map_view<std::views::all_t<Range>, Fn>;

    namespace views::adaptor
    {
        template<typename  Adaptor, typename...Args>
        concept adaptorInvokable = requires { std::declval<Adaptor>()(std::declval<Args>()...);};

        // True if the range adaptor non-closure Adaptor can be partially applied
        // with Args.
        template<typename Adaptor, typename... Args>
        concept adaptorPartail_appl_via = (Adaptor::Sarity > 1) &&
                                          (sizeof...(Args) == Adaptor::Sarity -1) &&
                                          (std::constructible_from<std::decay_t<Args>, Args> && ...);

        struct RangeApdatorClosure
        {

        };


        template<typename Adaptor, typename...Args>
        struct Partial: RangeApdatorClosure
        {
            std::tuple<Args...> mArgs;
            constexpr Partial(Args... args):mArgs{std::move(args)...} {}

        };

        // FIXME: incomplete
        template<typename Derived>
        struct RangeAdaptor
        {
            template<typename... Args>
            requires adaptorPartail_appl_via<Derived, Args...>
            constexpr auto operator()(Args&&...args) const
            {
                return Partial<Derived, std::decay_t<Args>...>{std::forward<Args>(args)...};
            }
        };



    } // end of namespace views::adaptor

    namespace views
    {



    } // endof functfun::views





} // end of namespace functfun


#endif//FUNCTIONAL_TRANSFORMVIEW_HPP
