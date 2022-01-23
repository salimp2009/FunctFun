//
// Created by salim on 21/01/2022.
//

#include "functProgPCH.hpp"

#include "functfun.hpp"
#include "MapFilesToLineCounts.hpp"

#include <range/v3/algorithm.hpp>
#include "fmt/core.h"

namespace functfun
{
    void functfun_BasicTest()
    {
        std::puts("-- functfun_BasicTest--");
        fmt::print("{} \n", "test fmt");
        std::string s1={"salim"};
        auto result = ranges::starts_with(s1,std::string{"sa"});
        auto result2 = s1.starts_with("sa");
        std::printf("%s \n", result ? "true": "false");
        std::printf("%s \n", result2 ? "true": "false");
    }

    void countlines_Test()
    {
        std::puts("--countlines_Test--");

        // FIXME : add a findFile input: directory name and/or filename
        const std::array vecFiles = {"../functfun.hpp", "sampleText.txt"};

        const auto result = countlinesInFiles(vecFiles);

         // red squiggles are false error it works
        for(const auto& value : result )
        {
            std::printf(" %lli ", value  );
        }
        //
        std::puts("\n--Test Passed--");

    }













} //end of namespace

