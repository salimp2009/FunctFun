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
        inline constexpr auto countLines = [](std::ifstream file)
        {
            return std::ranges::count(std::istreambuf_iterator<char>(file >>std::noskipws), std::istreambuf_iterator<char>(), '\n' );
        };

        inline constexpr auto openfile = [](std::string_view filename )
        {
            std::ifstream in(filename.data());
            in.unsetf(std::ios_base::skipws);
            return in;
        };
    }

    // FIXME : create a version to copy the result into an array and return compile time
    //  or it can be done by using is_const_evaluated
    //  ->if true then use std::transform and copy into an array and return it
    template<std::ranges::input_range R>
    inline constexpr auto countlinesInFiles(R&& files)
    {
        return files | std::views::transform(details::openfile)
                     | std::views::transform(details::countLines);
        //ranges-v3 version
        //return files | ranges::views::transform(details::openfile) | ranges::views::transform(details::countLines);
    }


} // end of namespace