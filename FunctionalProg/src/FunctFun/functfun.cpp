//
// Created by salim on 21/01/2022.
//

//#include "functProgPCH.hpp"
#include "functfun.hpp"
#include "MapFilesToLineCounts.hpp"
#include "arithmeticMeanUtils.hpp"

#include <range/v3/algorithm.hpp>
#include "fmt/format.h"

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
        constexpr std::array vecFiles = {"../functfun.hpp", "sampleText.txt"};

        const auto result = countlinesInFiles(vecFiles);

         // red squiggles are false error it works
        for(const auto& value : result )
        {
            std::printf(" %lli ", value  );
        }

        std::puts("\n--Test Passed--");
    }

    void slidingMean_Test()
    {
        std::puts("--slidingMean_Test--");
        const std::vector<double> vec1 = {1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};

        const auto result = slidingMean(vec1, 5);
        for(auto&& value : result)
        {
          fmt::print("{} \n ", fmt::join(value, " "));
            // this is being tested
          //fmt::print("{}\n", value);
        }


    }


    } //end of namespace

