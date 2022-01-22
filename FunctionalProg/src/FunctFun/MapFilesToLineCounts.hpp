//
// Created by salim on 22/01/2022.
//

#ifndef FUNCTIONAL_MAPFILESTOLINECOUNTS_HPP
#define FUNCTIONAL_MAPFILESTOLINECOUNTS_HPP

#include "functProgPCH.hpp"
#include "range/v3/algorithm.hpp"
#include "range/v3/view.hpp"
#endif //FUNCTIONAL_MAPFILESTOLINECOUNTS_HPP


namespace functfun
{

    ///@defgroup Problem inputs a list of files and return the number of newline in each file
    auto countLines(const std::string_view filename)
    {
        std::ifstream in(filename.data());

        return std::ranges::count(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), '\n' );
    }


    template<std::ranges::range R, std::size_t Size>
    inline constexpr auto countlineInFiles(R&& files) ->std::array<std::ranges::range_value_t<R>, Size>
    {

       return files | std::views::transform(countLines);
       // ranges-v3 version
       //return files | ranges::views::transform(countLines);
    }




} // end of namespace