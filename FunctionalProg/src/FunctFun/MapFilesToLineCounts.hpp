//
// Created by salim on 22/01/2022.
//

#ifndef FUNCTIONAL_MAPFILESTOLINECOUNTS_HPP
#define FUNCTIONAL_MAPFILESTOLINECOUNTS_HPP

#include "functProgPCH.hpp"
#include <range/v3/algorithm.hpp>
#include <range/v3/view.hpp>
#include <fmt/format.h>

#endif //FUNCTIONAL_MAPFILESTOLINECOUNTS_HPP


namespace functfun
{

    /// countlines; takes is a list of files and return the number of newline in each file
    namespace details
    {
        constexpr auto countLines = [](std::ifstream file)
        {
            return std::ranges::count(std::istreambuf_iterator<char>(file >>std::noskipws), std::istreambuf_iterator<char>(), '\n' );
        };

        constexpr auto openfile = [](std::string_view filename )
        {
            std::ifstream in(filename.data());
            in.unsetf(std::ios_base::skipws);
            fmt::print("{} \n", filename);
            return in;
        };
    }

    // FIXME: try to optimize this !!
    template<std::ranges::range R>
    inline auto countlinesInFiles(R&& files) ->std::vector<int>
    {
        auto result = files | std::views::transform(details::openfile)
                            | std::views::transform(details::countLines);

        std::vector<int> resultVec{};
        std::ranges::copy(result, std::back_inserter(resultVec));

       // ranges-v3 version
        [[maybe_unused]] auto result2 = files | ranges::views::transform(details::openfile) | ranges::views::transform(details::countLines);

        return resultVec;
    }




} // end of namespace