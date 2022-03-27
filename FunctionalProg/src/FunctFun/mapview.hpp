//
// Created by salim on 21/03/2022.
//

#ifndef FUNCTIONAL_MAPVIEW_HPP
#define FUNCTIONAL_MAPVIEW_HPP

#include "functProgPCH.hpp"

#include "viewAdaptors.hpp"

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
    private:
        template<bool Const>
        using Base = details::maybeConst_t<Const, V>;

        template<bool Const>
        struct iter_cat {};

        template<bool Const>
        requires std::ranges::forward_range<Base<Const>>
        struct iter_cat<Const>
        {
            // this is Alternative to below implementation
//            using Base = map_view::Base<Const>;
//            using Result = std::invoke_result_t<F&, std::ranges::range_reference_t<Base>>;
//
//            using iterator_cat = decltype([]
//              {
//                 if constexpr(std::is_lvalue_reference_v<Result>)
//                 {
//                     using Cat = typename std::iterator_traits<std::ranges::iterator_t<Base>>::iterator_category;
//                     if constexpr (std::derived_from<Cat, std::contiguous_iterator_tag>)
//                     {
//                         return std::random_access_iterator_tag{};
//                     }
//                     else
//                     {
//                         return Cat{};
//                     }
//                 } else
//                 {
//                     return std::input_iterator_tag{};
//                 }
//              }
//            ());

        private:
            static auto S_iter_cat()
            {
                using Base = map_view::Base<Const>;
                using Result = std::invoke_result_t<F&, std::ranges::range_reference_t<Base>>;

                if constexpr(std::is_lvalue_reference_v<Result>)
                {
                    using Cat = typename std::iterator_traits<std::ranges::iterator_t<Base>>::iterator_category;
                    if constexpr (std::derived_from<Cat, std::contiguous_iterator_tag>)
                    {
                        return std::random_access_iterator_tag{};
                    }
                    else
                    {
                        return Cat{};
                    }
                } else
                {
                    return std::input_iterator_tag{};
                }
            }
        public:
            using iterator_category = decltype(S_iter_cat());
        };

        template<bool Const>
        struct Sentinel;

        template<bool Const>
        struct Iterator :iter_cat<Const>
        {
        private:
            using Parent    = details::maybeConst_t<Const, map_view>;
            using Base      = map_view::Base<Const>;

            //FIXME: Alternative implemtation; test if better; if worse delete once everythng works
//            static std::random_access_iterator_tag iterConcept(std::random_access_iterator_tag);
//            static std::bidirectional_iterator_tag iterConcept(std::bidirectional_iterator_tag);
//            static std::forward_iterator_tag iterConcept(std::forward_iterator_tag);
//            static std::input_iterator_tag iterConcept(std::input_iterator_tag);

            static auto S_iterConcept()
            {
                if constexpr (std::ranges::random_access_range<V>)
                {
                    return std::random_access_iterator_tag{};
                }
                else if constexpr (std::ranges::bidirectional_range<V>)
                {
                    return std::bidirectional_iterator_tag{};
                }
                else if constexpr (std::ranges::forward_range<V>)
                {
                    return std::forward_iterator_tag{};
                }
                else
                {
                    return std::input_iterator_tag{};
                }
            }

            using Base_iter = std::ranges::iterator_t<Base>;
            Base_iter mCurrent  = Base_iter();
            Parent* mParent     = nullptr;
        public:

//            using iterator_concept =decltype([]
//            {
//              using BaseIterCat = typename std::iterator_traits<std::ranges::iterator_t<Base>>::iterator_category;
//              return decltype(iterConcept(BaseIterCat{})){};
//            }());

            using iterator_concept  = decltype(S_iterConcept());
            using value_type        = std::remove_cvref<std::invoke_result_t<F&, std::ranges::range_reference_t<Base>>>;
            using difference_type   = std::ranges::range_difference_t<Base>;

            constexpr Iterator()=default;
            constexpr Iterator(Parent* parent, Base_iter current):mCurrent{std::move(current)}, mParent{parent} { }

            // FIXME : should this be explicit
            constexpr Iterator(Iterator<!Const> it) requires Const && std::convertible_to<std::ranges::iterator_t<V>, Base_iter>
                    :mCurrent{std::move(it.mCurrent)}, mParent{it.mParent} { }

            constexpr Base_iter&    base() const& noexcept { return mCurrent;}
            constexpr Base_iter     base() && { return std::move(mCurrent);}

            // mfun is an optional that is why it derefences mParent and dereferences mFun
            constexpr decltype(auto) operator*() const noexcept(noexcept(std::invoke(*mParent->mfun, *mCurrent)))

            { return std::invoke(*mParent->mfun, *mCurrent);}

            constexpr Iterator& operator++()
            {
                ++mCurrent;
                return *this;
            }

            constexpr void operator++(int) { ++mCurrent;}

            constexpr Iterator operator++(int) requires std::ranges::forward_range<Base>
            {
                auto temp = *this;
                ++*this;
                return temp;
            }

            constexpr Iterator& operator--() requires std::ranges::bidirectional_range<Base>
            {
                --mCurrent;
                return *this;
            }

            constexpr Iterator operator--(int) requires std::ranges::bidirectional_range<Base>
            {
                auto temp = *this;
                --*this;
                return temp;
            }

            constexpr Iterator& operator+=(difference_type n) requires std::ranges::random_access_range<Base>
            {
                mCurrent += n;
                return *this;
            }

            constexpr Iterator& operator-=(difference_type n) requires std::ranges::random_access_range<Base>
            {
                mCurrent -= n;
                return *this;
            }

            constexpr decltype(auto) operator[](difference_type n) const requires std::ranges::random_access_range<Base>
            {
                return std::invoke(*mParent->mfun, mCurrent[n]);
            }

            friend constexpr bool operator==(const Iterator& x, const Iterator& y)
            requires std::equality_comparable<Base_iter>
            {
                return x.mCurrent == y.mCurrent;
            }

            friend constexpr bool
            operator<(const Iterator& x, const Iterator& y)
            requires std::ranges::random_access_range<Base>
            { return x.mCurrent < y.mCurrentt; }

            friend constexpr bool
            operator>(const Iterator& x, const Iterator& y)
            requires std::ranges::random_access_range<Base>
            { return y < x; }

            friend constexpr bool
            operator<=(const Iterator& x, const Iterator& y)
            requires std::ranges::random_access_range<Base>
            { return !(y < x); }

            friend constexpr bool
            operator>=(const Iterator& x, const Iterator& y)
            requires std::ranges::random_access_range<Base>
            { return !(x < y); }

            // FIXME: check why need both <=> and other comparisions at the same time
#if __has_include(<compare>)
            friend constexpr auto operator<=>(const Iterator& x, const Iterator& y)
            requires std::ranges::random_access_range<Base>
                  && std::three_way_comparable<Base_iter>
            {
                return x.mCurrent<=>y.mCurrent;
            }
#endif

            friend constexpr Iterator operator+(Iterator i, difference_type n)
            requires std::ranges::random_access_range<Base>
            {
            return {i.mParent, i.mCurrent+n};
            }

            friend constexpr Iterator operator+(difference_type n, Iterator i)
            requires std::ranges::random_access_range<Base>
            {
                return {i.mParent, i.mCurrent+n};
            }

            friend constexpr Iterator operator-(Iterator i, difference_type n)
            requires std::ranges::random_access_range<Base>
            {
                return {i.mParent, i.mCurrent-n};
            }

            // FIXME: GCC did not have this; why
//            friend constexpr Iterator operator-(difference_type n, Iterator i)
//            requires std::ranges::random_access_range<Base>
//            {
//                return {i.mParent, i.mCurrent-n};
//            }

            // GCC Note:
            // "_GLIBCXX_RESOLVE_LIB_DEFECTS
            // 3483. transform_view::iterator's difference is overconstrained"
            friend constexpr  difference_type operator-(const Iterator& x, const Iterator& y  )
            requires std::sized_sentinel_for<std::ranges::iterator_t<Base>, std::ranges::iterator_t<Base>>
            {
                return x.mCurrent - y.mCurrent;
            }

            friend constexpr decltype(auto) iter_move(const Iterator& i) noexcept (noexcept(*i))
            {
                if constexpr (std::is_lvalue_reference_v<decltype(*i)>)
                {
                    return std::move(*i);
                }
                else
                {
                    return *i;
                }
            }

            friend Iterator<!Const>;
            template<bool> friend struct Sentinel;
        };

        template<bool Const>
        struct Sentinel
        {
        private:
            using Parent    = details::maybeConst_t<Const, map_view>;
            using Base      = map_view::Base<Const>;

            std::ranges::sentinel_t<Base> mEnd = std::ranges::sentinel_t<Base>();

            template<bool Const2>
            constexpr auto distance_from(const Iterator<Const2> i) const
            {
                return mEnd - i.mCurrent;
            }

            template<bool Const2>
            constexpr auto equal(const Iterator<Const2> i) const
            {
                return mEnd == i.mCurrent;
            }

        public:
            Sentinel()= default;

            constexpr explicit Sentinel(std::ranges::sentinel_t<Base> end) : mEnd{end} {}

            // FIXME: should this be explicit
            constexpr Sentinel(Sentinel<!Const> i) requires Const
                    && std::convertible_to<std::ranges::sentinel_t<V>, std::ranges::sentinel_t<Base>>
                : mEnd{std::move(i.mEnd)} { }

            constexpr std::ranges::sentinel_t<Base> base() const { return mEnd;}

            template<bool Const2>
            requires std::sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<details::maybeConst_t<Const2, V>>>
            friend constexpr bool operator==(const Iterator<Const2>& x, const Sentinel& y)
            { return y.equal(x);}

            template<bool Const2, typename Base2 = details::maybeConst_t<Const2, V>>
            requires std::sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<Base2>>
            friend constexpr auto  operator-(const Iterator<Const2>& x, const Sentinel& y) -> std::ranges::range_difference_t<Base2>
            { return y.distance_from(x);}

            template<bool Const2, typename Base2 = details::maybeConst_t<Const2, V>>
            requires std::sentinel_for<std::ranges::sentinel_t<Base>, std::ranges::iterator_t<Base2>>
            friend constexpr auto  operator-(const Sentinel& y, const Iterator<Const2>& x) -> std::ranges::range_difference_t<Base2>
            { return y.distance_from(x);}

            friend Sentinel<!Const>;
        };

        V mbase = V();
        [[no_unique_address]] std::optional<F> mfun;
        // FIXME: Original GCC Implementation; this is wrapper around optional
        //  except for semiregular type it stores directly
        //[[no_unique_address]] std::ranges::__detail::__box<_Fp> _M_fun;

    public:

        constexpr map_view() requires std::default_initializable<V> && std::default_initializable<F> =default;
        constexpr map_view(V range, F fun) : mbase{std::move(range)}, mfun{std::move(fun)} { }

        constexpr V base() const& requires std::copy_constructible<V> { return mbase; }
        constexpr V base() && { return std::move(mbase); }

        auto begin() -> Iterator<false> { return Iterator<false>{this, std::ranges::begin(mbase)};}
        auto end()   -> Sentinel<false> { return Sentinel<false>{std::ranges::end(mbase)};}
        auto end()   -> Iterator<false> requires std::ranges::common_range<V>
        { return Iterator<false>{this, std::ranges::end(mbase)};}

        auto begin() const -> Iterator<true> requires std::ranges::range<V const>
                && std::regular_invocable<const F&, std::ranges::range_reference_t<const V>>
        { return Iterator<true>{this, std::ranges::begin(mbase)};}

        auto end()   const -> Sentinel<true> requires std::ranges::range<V const>
                && std::regular_invocable<const F&, std::ranges::range_reference_t<const V>>
        { return Sentinel<true>{std::ranges::end(mbase)};}

        auto end()   const -> Iterator<true> requires std::ranges::common_range<V const>
                && std::regular_invocable<const F&, std::ranges::range_reference_t<const V>>
        { return Iterator<true>{this, std::ranges::end(mbase)};}

        // FIXME : check if these are needed because sstd::ranges::view_interface has already those functions
        //  ClangTidy shows it shadows those functions but this overload has a requires clause to optimize
        //  the parent has std:forward in requires clause and has to iterator from to end to get the size
       // constexpr auto size()       requires std::ranges::sized_range<V> { return std::ranges::size(mbase);}
       // constexpr auto size() const requires std::ranges::sized_range<V> { return std::ranges::size(mbase);}

    };

    template<typename Range, typename Fn>
    map_view(Range&&, Fn) -> map_view<std::views::all_t<Range>, Fn>;

    namespace views
    {
        namespace details
        {
            template<typename Range, typename F>
            concept canMapView = requires { map_view(std::declval<Range>(), std::declval<F>()); };
        } // namespace details

    struct MapView: adaptor::RangeAdaptor<MapView>
    {
        template<std::ranges::viewable_range Range, typename F>
        constexpr auto operator()(Range&& rng, F&& fn) const
        {
            return map_view(std::forward<Range>(rng), std::forward<F>(fn));
        }

        using RangeAdaptor<MapView>::operator();
        static constexpr int Sarity = 2;
        static constexpr bool S_hasSimpleExtraArgs = true;
    };

    inline constexpr MapView map;

    } // endof functfun::views

} // end of namespace functfun


#endif//FUNCTIONAL_MAPVIEW_HPP
