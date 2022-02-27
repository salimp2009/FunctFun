//
// Created by salim on 23/02/2022.
//

#ifndef FUNCTIONAL_JSONDATATYPES_HPP
#define FUNCTIONAL_JSONDATATYPES_HPP

#include "functProgPCH.hpp"


namespace functfun
{
    struct JsonWrapper;
    using JsonArray = std::vector<JsonWrapper>;
    using JsonObject =std::map<std::string, JsonWrapper>;
    using JsonValue = std::variant<bool,
                                   double,
                                   std::string,
                                   std::nullptr_t,
                                   JsonArray,
                                   JsonObject>;

    struct JsonWrapper
    {
        JsonValue value;
        constexpr explicit(false) operator JsonValue& () noexcept { return value; }
        constexpr explicit(false) operator  const JsonValue& () const &  noexcept { return value; }
    };


}

#endif//FUNCTIONAL_JSONDATATYPES_HPP
