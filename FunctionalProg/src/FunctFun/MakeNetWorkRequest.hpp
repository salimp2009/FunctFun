//
// Created by salim on 23/01/2022.
//

#ifndef FUNCTIONAL_MAKENETWORKREQUEST_HPP
#define FUNCTIONAL_MAKENETWORKREQUEST_HPP

#include "functProgPCH.hpp"

#endif //FUNCTIONAL_MAKENETWORKREQUEST_HPP

// This is experimental ; not usable as it is ; to experiment to set requires fields set
// before a network send_request() is sent; example from Ben Deane's talk CppCon 2018
// and original implementation by  @timtro (twitter handle) presented at gist.github.com/timtro
namespace functfun::details
{
            constexpr static std::uint8_t OPT_FIELD     = 1<<0;
            constexpr static std::uint8_t REQ_FIELD1    = 1<<1;
            constexpr static std::uint8_t REQ_FIELD2    = 1<<2;
    [[maybe_unused]] constexpr static std::uint8_t ALL_FIELDS    = OPT_FIELD | REQ_FIELD1 | REQ_FIELD2;

            struct field_t {};
}

namespace functfun
{
    using namespace functfun::details;

    template<std::uint8_t N>
    struct request_t;

    template<>
    struct [[maybe_unused]] request_t<0>
    {
       [[maybe_unused]] field_t f1{};
       [[maybe_unused]] field_t f2{};
       [[maybe_unused]] field_t f3{};
       [[maybe_unused]] field_t f4{};
    };

    template<std::uint8_t N>
    struct [[maybe_unused]] request_t: request_t<N-1>
    {
        [[maybe_unused]] request_t<N & ~REQ_FIELD1>& set_req_field1(field_t f)
        {
            this->f1 =f;
            return *this;
        }

        [[maybe_unused]] request_t<N & ~REQ_FIELD2>& set_req_field2(field_t f)
        {
            this->f2 =f;
            return *this;
        }

        [[maybe_unused]] request_t& set_opt_field(field_t f)
        {
            this->f3 =f;
            return *this;
        }

//        [[maybe_unused]] field_t f1{};
//        [[maybe_unused]] field_t f2{};
//        [[maybe_unused]] field_t f3{};
//        [[maybe_unused]] field_t f4{};
    };

    [[maybe_unused]] request_t<ALL_FIELDS> make_request()
    {
        request_t<4> rqt1{};
        rqt1.set_opt_field(field_t{});
        std::puts("making requests");
        return request_t<ALL_FIELDS>{};
    }

    template<std::uint8_t N>
    [[maybe_unused]] void send_request(const request_t<N>& req) =delete;

    [[maybe_unused]] void send_request(const request_t<OPT_FIELD>& req)
    {
        std::puts("making request from OPT_FIELD");
    }

}
