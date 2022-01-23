//
// Created by salim on 23/01/2022.
//

#ifndef FUNCTIONAL_MAKENETWORKREQUEST_HPP
#define FUNCTIONAL_MAKENETWORKREQUEST_HPP

#include "functProgPCH.hpp"

#endif //FUNCTIONAL_MAKENETWORKREQUEST_HPP

namespace functfun::details
{
        struct RequestField
        {
            constexpr static std::uint8_t OPT_FIELD     = 1<<0;
            constexpr static std::uint8_t REQ_FIELD1    = 1<<1;
            constexpr static std::uint8_t REQ_FIELD2    = 1<<2;
            constexpr static std::uint8_t ALL_FIELDS    = OPT_FIELD | REQ_FIELD1 | REQ_FIELD2;
        };

}

namespace functfun
{
    using namespace functfun::details;



}
