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

    /// @def countlines; takes is a list of files and return the number of newline in each file
    /// @def std::noskipws disables to skipping the whitespace which is enabled by default so that we continue to read the end of line
    namespace details
    {
        inline constexpr auto countLines = [](std::ifstream file)
        {
            return std::ranges::count(std::istreambuf_iterator<char>(file >>std::noskipws), std::istreambuf_iterator<char>(), '\n' );
        };

        inline constexpr auto openFile = [](std::string_view filename )
        {
            std::ifstream in(filename.data());
            in.unsetf(std::ios_base::skipws);
            return in;
        };
    }

    template<std::ranges::input_range R>
    requires std::ranges::viewable_range<R>
    inline constexpr auto countlinesInFiles(R&& files)
    {
        return files | std::views::transform(details::openFile)
                     | std::views::transform(details::countLines);
    }


} // end of namespace