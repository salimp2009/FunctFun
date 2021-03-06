//
// Created by salim on 24/03/2022.
//

#ifndef FUNCTIONAL_VIEWADAPTORS_HPP
#define FUNCTIONAL_VIEWADAPTORS_HPP

#include "functProgPCH.hpp"


namespace functfun
{
    namespace views::adaptor
    {
        template<typename  Adaptor, typename...Args>
        concept adaptorInvocable = requires { std::declval<Adaptor>()(std::declval<Args>()...);};

        // True if the range adaptor non-closure Adaptor can be partially applied
        // with Args.
        template<typename Adaptor, typename... Args>
        concept adaptorPartial_appl_via = (Adaptor::Sarity > 1) &&
                                          (sizeof...(Args) == Adaptor::Sarity -1) &&
                                          (std::constructible_from<std::decay_t<Args>, Args> && ...);


        template<typename Adaptor, typename...Args>
        struct Partial;

        // GCC-11.2 Notes;
        // True if the behavior of the range adaptor non-closure _Adaptor is
        // independent of the value category of its extra arguments _Args.
        template<typename Lhs, typename Rhs>
        struct Pipe;


        // Notes from GCC-11.2 Implementation:
        // The mbase class of every range adaptor closure.
        // The derived class should define the optional static data member
        // S_hasSimpleCallop to true if the behavior of this adaptor is
        // independent of the constness/value category of the adaptor object.
        struct RangeApdatorClosure
        {
            // range | adaptor == adaptor(range)
            template<typename Self, typename Range>
            requires std::derived_from<std::remove_cvref_t<Self>, RangeApdatorClosure>
                    && adaptorInvocable<Self, Range>
            friend constexpr auto operator| (Range&& rng, Self&& self)
            {
                return std::forward<Self>(self)(std::forward<Range>(rng));
            }

            template<typename Lhs, typename Rhs>
            requires std::derived_from<Lhs, RangeApdatorClosure>
                    && std::derived_from<Rhs, RangeApdatorClosure>
            friend constexpr auto operator|(Lhs lhs, Rhs rhs)
            {
                return Pipe<Lhs, Rhs>(std::move(lhs), std::move(rhs));
            }
        };

        // The mbase class of every range adaptor non-closure.

        // The static data member Derived::Sarity must contain the total number of
        // arguments that the adaptor takes, and the class Derived must introduce
        // RangeAdaptor::operator() into the class scope via a using-declaration.

        // The optional static data member Derived::ShasSimpleExtraArgs should
        // be defined to true if the behavior of this adaptor is independent of the
        // constness/value category of the extra arguments.  This data member could
        // also be defined as a variable template parameterized by the types of the
        // extra arguments.
        template<typename Derived>
        struct RangeAdaptor
        {
            template<typename... Args>
            requires adaptorPartial_appl_via<Derived, Args...>
            constexpr auto operator()(Args&&...args) const
            {
                return Partial<Derived, std::decay_t<Args>...>{std::forward<Args>(args)...};
            }
        };

        // GCC-11.2 Notes;
        // "True if the range adaptor closure Adaptor has a simple operator(), i.e.
        // one that's not overloaded according to constness or value category of the
        // Adaptor object"
        template<typename Adaptor>
        concept closureHasSimpleCallop = Adaptor:: S_hasSimpleCallop;

        template<typename Adaptor, typename...Args>
        concept  adaptorHasSimpleExtraArgs = Adaptor::S_hasSimpleExtraArgs
                || Adaptor::template S_hasSimpleExtraArgs<Args...>;

        template<typename Adaptor, typename...Args>
        struct Partial: RangeApdatorClosure
        {
            std::tuple<Args...> mArg;
            constexpr Partial(Args... args): mArg{std::move(args)...} {}

            template<typename Range>
            requires adaptorInvocable<Adaptor, Range, const Args&...>
            constexpr auto operator()(Range&& rng) const&
            {
                auto forwardFunct = [&rng](const auto&... args)
                { return Adaptor{}(std::forward<Range>(rng),args...);};

                return std::apply(forwardFunct, mArg);
            }

            template<typename Range>
            requires adaptorInvocable<Adaptor, Range, Args...>
            constexpr auto operator()(Range&& rng) &&
            {
                auto forwardFunct = [&rng](auto&&... args)
                { return Adaptor{}(std::forward<Range>(rng),std::move(args)...);};

                return std::apply(forwardFunct, mArg);
            }

            template<typename Range>
            constexpr auto operator()(Range&& rng) const && = delete;
        };

        // Partial specialization; Adaptor gets one single arg
        template<typename Adaptor, typename Arg>
        struct Partial<Adaptor, Arg>: RangeApdatorClosure
        {
            Arg mArg;

            constexpr Partial(Arg arg):mArg{std::move(arg)} {}

            template<typename Range>
            requires adaptorInvocable<Adaptor, Range, const Arg&>
            constexpr auto operator()(Range&& rng) const&
            {
                return Adaptor{}(std::forward<Range>(rng), mArg);
            }

            template<typename Range>
            requires adaptorInvocable<Adaptor, Range, Arg>
            constexpr auto operator()(Range&& rng) &&
            {
                return Adaptor{}(std::forward<Range>(rng), std::move(mArg));
            }

            template<typename Range>
            constexpr auto operator()(Range&& rng) const && = delete;
        };


        // Partial specialization of the primary template for the case where the extra
        // arguments of the adaptor can always be safely and efficiently forwarded by
        // const reference.  This lets us get away with a single operator() overload,
        // which makes overload resolution failure diagnostics more concise.
        template<typename Adaptor, typename... Args>
        requires adaptorHasSimpleExtraArgs<Adaptor, Args...>
                && (std::is_trivially_copyable_v<Args> && ...)
        struct Partial<Adaptor, Args...>: RangeApdatorClosure
        {
            std::tuple<Args...> mArgs;

            constexpr Partial(Args&&...args) :mArgs{std::move(args)...} {}

            // Invoke Adaptor with arguments rng, const mArgs&... regardless
            // of the value category of this _Partial object.
            template<typename Range>
            constexpr auto operator()(Range&& rng) const
            {
                auto forwardFunc = [&rng](const auto&... args)
                { return Adaptor{}(std::forward<Range>(rng), args...);};
                return std::apply(forwardFunc, mArgs);
            }

            static constexpr bool S_hasSimpleCallop = true;
        };

        // similar to above specialization but with one arg
        template<typename Adaptor, typename Arg>
        requires adaptorHasSimpleExtraArgs<Adaptor, Arg> && std::is_trivially_copyable_v<Arg>
        struct Partial<Adaptor, Arg> : RangeApdatorClosure
        {
            Arg mArg;

            constexpr Partial(Arg arg):mArg{std::move(arg)} { }

            template<typename Range>
            requires adaptorInvocable<Adaptor, Range, const Arg&>
            constexpr auto operator()(Range&& rng) const
            {
                return Adaptor{}(std::forward<Range>(rng), mArg);
            }

            static constexpr bool S_hasSimpleCallop = true;
        };


        template<typename Lhs, typename Rhs, typename Range>
        concept pipeInvocable = requires { std::declval<Rhs>()(std::declval<Lhs>()(std::declval<Range>()));};

        // A range adaptor closure that represents composition of the range
        // adaptor closures Lhs and Rhs; e.g : range | lhs | rhs == rhs(lhs(range))
        template<typename Lhs, typename Rhs>
        struct Pipe : RangeApdatorClosure
        {
            [[no_unique_address]] Lhs mLhs;
            [[no_unique_address]] Rhs mRhs;

            constexpr Pipe(Lhs lhs, Rhs rhs):mLhs{std::move(lhs)}, mRhs{std::move(rhs)} { }

            template<typename Range>
            requires pipeInvocable<const Lhs&, const Rhs&, Range>
            constexpr auto operator()(Range&& rng) const &
            { return mRhs(mLhs(std::forward<Range>(rng))); }

            template<typename Range>
            requires pipeInvocable<Lhs, Rhs, Range>
            constexpr auto operator()(Range&& rng) &&
            { return std::move(mRhs)(std::move(mLhs)(std::forward<Range>(rng))); }

            template<typename Range>
            constexpr auto operator()(Range&& rng) const&& = delete;
        };


        // A partial specialization where both adaptor operands have a simple operator().
        template<typename Lhs, typename Rhs>
        requires closureHasSimpleCallop<Lhs> && closureHasSimpleCallop<Rhs>
        struct Pipe<Lhs, Rhs>:RangeApdatorClosure
        {
            [[no_unique_address]] Lhs mLhs;
            [[no_unique_address]] Rhs mRhs;

            constexpr Pipe(Lhs lhs, Rhs rhs):mLhs{std::move(lhs)}, mRhs{std::move(rhs)} { }

            template<typename Range>
            requires pipeInvocable<const Lhs&, const Rhs&, Range>
            constexpr auto operator()(Range&& rng) const
            { return mRhs(mLhs(std::forward<Range>(rng))); }

            static constexpr bool S_hasSimpleCallop = true;
        };


    } // end of namespace views::adaptor























} // endof namespace functfun



#endif//FUNCTIONAL_VIEWADAPTORS_HPP
