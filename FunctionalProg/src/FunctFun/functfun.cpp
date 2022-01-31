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

        // red squiggles are false error it works
        const auto result = countlinesInFiles(vecFiles);

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

        // testing ideas how to deal chrono duration
        // 1st issues; give in duration type and return int or float random number
        // 2nd issue; create make_duration distribution that accepts int and float and returns a random duration
        using namespace  std::chrono_literals;
        std::chrono::duration dr1(10s);
        fmt::print("dr1:{} \n", dr1.count());
        long long int mydur = dr1.count();
        fmt::print("mydur [expected: =dr1]: {} \n", mydur);

        std::chrono::duration dr2(10.5s);
        std::chrono::duration dr0(1.5s);
        long double mydur2 = dr2.count();
        fmt::print("mydur2 [expected: =dr2]:{:.4f} \n", mydur2);


        auto mydurdist = make_uniform_distribution(dr0.count(), dr2.count());
        fmt::print("chrono duration distribution: {} \n", mydurdist(gen));

        // Test for accepting chrono duration for float
        auto result4 = make_uniform_distribution(1.5s,10.6s);
        fmt::print("duration float distribution: {:.4f} \n", result4(gen));

        // Test for accepting chrono duration for int
        auto result5 = make_uniform_distribution(1s,10s);
        fmt::print("duration int distribution: {} \n", result5(gen));



    }



    } //end of namespace

