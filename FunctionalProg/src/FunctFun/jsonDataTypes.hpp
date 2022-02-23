//
// Created by salim on 23/02/2022.
//

#ifndef FUNCTIONAL_JSONDATATYPES_HPP
#define FUNCTIONAL_JSONDATATYPES_HPP

namespace functfun
{
    struct JsonWrapper;
    using JsonArray = std::vector<JsonWrapper>;
    using JsonObject =std::map<std::string, JsonWrapper>;
    using JsonValue = std::variant<bool,
                                   std::string,
                                   std::nullptr_t,
                                   JsonArray,
                                   JsonObject>;

    struct JsonWrapper
    {
        JsonValue value;
        constexpr operator JsonValue&() {return value;}
        constexpr operator const JsonValue&() const& {return value;}

    };




}

#endif//FUNCTIONAL_JSONDATATYPES_HPP
