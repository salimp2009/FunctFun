//
// Created by salim on 20/02/2022.
//

#include "functProgPCH.hpp"
#include "stringTrim.hpp"

namespace functfun
{

   std::string string_trim_left(std::string s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](const auto& elem) { return elem != ' ';}));
        std::erase_if(s, [](const auto& elem) { return elem != ' ';});
        return s;
    }
    [[nodiscard]] std::string string_trim_right(std::string s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](const auto& elem) { return elem != ' ';}).base(), s.end());
        return s;
    }

    std::string string_trim(std::string s)
    {
        return string_trim_left(string_trim_right(std::move(s)));

    }

}