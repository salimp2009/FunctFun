//
// Created by salim on 31/03/2022.
//

#ifndef FUNCTIONAL_ARITHMETICVECTOR_HPP
#define FUNCTIONAL_ARITHMETICVECTOR_HPP

#include "functProgPCH.hpp"

namespace functfun
{

    // A naive example for a expression template from Rainer Grimm's example
    template<typename T, typename Cont= std::vector<T>>
    class mathVec
    {
        Cont cont;

    public:
        // mathVec with initial size
        constexpr explicit mathVec(const std::size_t n) : cont(n){}

        // mathVec with initial size and value
        mathVec(const std::size_t n, const double initialValue) : cont(n, initialValue){}

        // Constructor for underlying container
        constexpr explicit mathVec(const Cont& other) : cont(other){}

        // assignment operator for mathVec of different type
        template<typename T2, typename R2>
       constexpr mathVec & operator=(const mathVec<T2, R2>& other){
            assert(size() == other.size());
            for (std::size_t i = 0; i < cont.size(); ++i) cont[i] = other[i];
            return *this;
        }

        // size of underlying container
        [[nodiscard]] constexpr std::size_t size() const{
            return std::size(cont);
        }

        // index operators
        constexpr T operator[](const std::size_t i) const{
            return cont[i];
        }

        [[nodiscard]] constexpr T& operator[](const std::size_t i){
            return cont[i];
        }

        // returns the underlying data
        [[nodiscard]] const Cont& data() const{
            return cont;
        }

        [[nodiscard]] Cont& data(){
            return cont;
        }
    };

    template<typename T, typename Op1, typename Op2>
    class mathVecAdd
    {
        const Op1& op1;
        const Op2& op2;

    public:
        constexpr mathVecAdd(const Op1& a, const Op2& b) :op1{a}, op2{b} { }

        constexpr T operator[](const std::size_t i) const
        {
            return op1[i] + op2[i];
        }

        [[nodiscard]] constexpr std::size_t size() const
        {
            return std::size(op1);
        }

    };

    template<typename T, typename Op1, typename Op2>
    class mathVecMult
    {
        const Op1& op1;
        const Op2& op2;

    public:
        constexpr mathVecMult(const Op1& a, const Op2& b) :op1{a}, op2{b} { }

        constexpr T operator[](const std::size_t i) const
        {
            return op1[i] * op2[i];
        }

        [[nodiscard]] constexpr std::size_t size() const
        {
            return std::size(op1);
        }
    };


    template<typename T, typename Rng1, typename Rng2>
    constexpr mathVec<T, mathVecAdd<T, Rng1, Rng2>>
    operator+(const mathVec<T, Rng1>& a, const mathVec<T, Rng2>& b) noexcept
    {
        return mathVec<T, mathVecAdd<T,Rng1,Rng2>>(mathVecAdd<T,Rng1, Rng2>(a.data(), b.data()));
    }

    template<typename T, typename Rng1, typename Rng2>
    constexpr mathVec<T, mathVecMult<T, Rng1, Rng2>>
    operator*(const mathVec<T, Rng1>& a, const mathVec<T, Rng2>& b) noexcept
    {
        return mathVec<T, mathVecMult<T,Rng1,Rng2>>(mathVecMult<T,Rng1, Rng2>(a.data(), b.data()));
    }

} // endof namespace functfun

#endif//FUNCTIONAL_ARITHMETICVECTOR_HPP
