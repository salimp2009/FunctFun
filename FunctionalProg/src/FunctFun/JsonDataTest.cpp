//
// Created by salim on 02/03/2022.
//

#include "functProgPCH.hpp"
#include "functfun.hpp"
#include "jsonDataTypes.hpp"
#include "renderJsonValues.hpp"

#include <fmt/format.h>


namespace functfun
{

    void jsonData_Test()
    {
        std::puts("--jsonData_Test--");
        using namespace  std::string_literals;
        JsonValue jvalue1 = "Hello Json"s;

        fmt::print("{} \n", std::get<std::string>(jvalue1));
        fmt::print("{} \n", jvalue1.index());

        auto result = render_JsonValue(jvalue1);
        std::puts("render JsonValue: ");
        fmt::print("{} \n", result);


        JsonArray jarr1 = {{"my jarray"}, {100.00}, {true}};
        auto result2 = render_array(jarr1);

        std::puts("render JsonArray: ");
        fmt::print("{} \n", result2);

        JsonObject jobj1 = {{"value1"s, {100.0}}, {{"value2"}, {"Json String"}}, {{"value3"}, {false}}};
        auto result3 = render_object(jobj1);

        std::puts("render JsonObject: ");
        fmt::print("{} \n", result3);

    }




} // end of namespace functfun
