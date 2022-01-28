//
// Created by salim on 21/01/2022.
//

//#include "functProgPCH.hpp"
#include "functfun.hpp"
#include "MapFilesToLineCounts.hpp"
#include "arithmeticMeanUtils.hpp"
#include "makeUniformDistribution.hpp"

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

        const std::vector<double> vec1 = {1.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,15};
        const auto result = slidingMean(vec1, 5);
        fmt::print("result vec1: {:.4f} \n", fmt::join(result, " "));

        const std::vector vec2 ={1,2,3,4,5,6,8,9,10,12,25};
        const auto result2 = slidingMean(vec2, 5);
        fmt::print("result vec2: {:.4f} \n", fmt::join(result2, " "));

        std::puts("--Test Passed--\n");

    }

    void makeUniformDistribution_Test()
    {
        std::puts("--makeUniformDistribution_Test--");
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        auto result = make_uniform_distribution(1,100);
        fmt::print("make uniform dist for ints: {} \n", result(gen));

        auto result2 = make_uniform_distribution(1.0,10.0);
        fmt::print("make uniform dist for double/floats: {:.6f} \n", result2(gen));

        auto result3 = make_uniform_distribution(1,10.0);
        fmt::print("make uniform dist for an int,double input [expected:double]: {:.6f} \n", result3(gen));
        std::puts("--Test Passed--\n");
    }



    } //end of namespace

