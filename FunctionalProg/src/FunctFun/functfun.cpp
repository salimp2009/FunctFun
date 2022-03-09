//
// Created by salim on 21/01/2022.
//

//#include "functProgPCH.hpp"
#include "functfun.hpp"
#include "MapFilesToLineCounts.hpp"
#include "arithmeticMeanUtils.hpp"
#include "makeUniformDistribution.hpp"
#include "stringTrim.hpp"
#include "functUtils.hpp"
#include "join.hpp"
//#include "jsonDataTypes.hpp"
//#include "renderJsonValues.hpp"
//#include "fold_variant.hpp"

#include <range/v3/algorithm.hpp>
#include <fmt/format.h>
#include <fmt/core.h>



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
        constexpr std::array vecFiles{"../../FunctionalProg/src/FunctFun/functfun.hpp", "../../FunctionalProg/src/FunctFun/sampleFiles/sampleText.txt"};

        // red squiggles are false error it works
        const auto result = countlinesInFiles(vecFiles);

        for(const auto& value : result )
        {
            fmt::print("num of lines: {} \n", value);
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

        std::vector v = { 1, 2, 3 };

    }


    void slidingMean_ChronoDurTest()
    {
        std::puts("--slidingMean_Chrono Duration Test--");

        using namespace std::chrono_literals;
        const std::vector vec1 = {1.0s,3.0s,4.0s,5.0s,6.0s,7.0s,8.0s,9.0s,10.0s,15.0s};

        std::printf("\n Chrono Duration [double]; ");
        const auto result = slidingMean(vec1, 5);
        for(const auto& elem : result)
        {
            fmt::print("[{:.4LF}sec]", elem.count());
        }

        std::printf("\n Chrono Duration [int]; ");

        const std::vector vec2 ={1s,2s,3s,4s,5s,6s,8s,9s,10s,12s,25s};
        const auto result2 = slidingMean(vec2, 5);
        for(const auto& elem : result2)
        {
            fmt::print("[{}sec]", elem.count());
        }

        std::puts("\n--Test Passed--\n");
    }

    void stringTrim_Test()
    {
        std::puts("--stringTrim_Test--");

        std::string str1 ={"  SALIM PAMUKCU   "};
        std::printf("Before trim: %s ------ \n", str1.c_str());
        string_trim(str1);
        std::printf("After trim: %s ------ \n", str1.c_str());
    }

    void filterTransform_Test()
    {
        std::puts("--filterTransform_Test--");
        std::vector<PersonType> people ={{"Salim", "Male"}, {"Didem", "Female"},{"Semos", "Female"},{"Demir", "Male"}};
        std::vector<PersonType> females;

        std::ranges::copy_if(people,std::back_inserter(females), [](const auto& elem) {return elem=="Female";}, &PersonType::gender);

        std::vector<std::string> femalesNames(std::size(females));
        std::ranges::transform(females, femalesNames.begin(), &PersonType::name);
        // os.get() ; get is a member function of std::reference_wrapper<> or member function std::ref
        std::accumulate(femalesNames.cbegin(), femalesNames.cend(), std::ref(std::cout),
                        [](auto&& os,const auto& elem) ->decltype(auto) {return os.get()<<elem<<" ";});

        std::puts("");
        std::copy(femalesNames.cbegin(), femalesNames.cend(), std::ostream_iterator<std::string>(std::cout, " "));
        fmt::print("\n{}", fmt::join(femalesNames, " "));
        std::puts("");

    }

    void viewsfilterTransform_Test()
    {
        std::puts("--viewsfilterTransform_Test--");

        std::vector<PersonType> people ={{"Salim", "Male"}, {"Didem", "Female"},{"Semos", "Female"},{"Demir", "Male"}};
        auto femaleSelect = [](const PersonType& elem) { return elem.gender=="Female";};
        std::vector<std::string> femalesNames;

        std::ranges::copy(people | std::views::filter(femaleSelect) | std::views::transform(&PersonType::name), std::back_inserter(femalesNames));
        std::puts("\nCopy version with piping; ");
        fmt::print("{}", fmt::join(femalesNames, " "));
        std::puts(" ");

        auto rng = people | std::views::filter(femaleSelect) | std::views::transform(&PersonType::name) | std::views::common;
        std::accumulate(rng.begin(), rng.end(), std::back_inserter(femalesNames), [](auto&& vecIter, const auto& elem){   return *vecIter = elem;});
        std::puts("\nAccumulate with range version; ");
        fmt::print("{}", fmt::join(femalesNames, " "));
        std::puts(" ");


        std::puts("\nAccumulate with ostream_iterator version; ");
        std::accumulate(femalesNames.begin(), femalesNames.end(), std::ostream_iterator<std::string>(std::cout, " "), [](auto&& outIter, const auto& elem){   return *outIter = elem;});
        std::puts(" ");

        auto rng2 = people | std::views::filter(femaleSelect) | std::views::common;
        std::transform(rng2.begin(), rng2.end(), std::back_inserter(femalesNames), [](const auto& elem) {return elem.name;});
        std::puts("\nTransform with range2 version(only filtered); ");
        fmt::print("{}", fmt::join(femalesNames, " "));
        std::puts(" ");

        std::puts("\nRange only; ");
        fmt::print("{}", fmt::join(rng, " "));
        std::puts(" ");

        std::puts("\nCopy version with range back inserted; ");
        std::copy(rng.begin(), rng.end(), std::back_inserter(femalesNames));
        fmt::print("{}", fmt::join(femalesNames, " "));
        std::puts(" ");

    }

    void join_Test()
    {
        std::puts("-- join_Test ");

        std::string urlBase{"https:\\\\www.boost.org/?"};
        std::map<std::string, std::string> urlArgs{ {"help", "install"},
                                                    {"library", "multi_array"}};

        fmt::print("urlBase: {}\n", urlBase);
        for(const auto& elem : urlArgs)
        {
            fmt::print("{}, {}\n", elem.first, elem.second);
        }

        join(std::cbegin(urlArgs), std::cend(urlArgs),
             std::back_insert_iterator(urlBase), '&',
             [](const std::pair<const std::string&, std::string>& elem)
             {
                 //const auto& [key, val] = elem;
                 return  elem.first + '=' + elem.second;
             });
        fmt::print("urlBase after join: {} \n", urlBase);

    }

    void joinStrings_Test()
    {
        std::puts("--joinStrings_Test--");
        std::string myStr = {"salitos and Didos"};

        auto it = std::back_inserter(myStr);
        *it = 'C';
        std::string newStr = "& : html=stupid.com";
        fmt::print("{} \n", myStr);
        std::copy(newStr.begin(), newStr.end(), std::back_inserter(myStr));
        fmt::print("{} \n", myStr);

        std::accumulate(newStr.begin(), newStr.end(), std::back_inserter(myStr),
                        [](auto&& itStr, const auto& elem) { return std::move(itStr = elem);} );
        fmt::print("{} \n", myStr);

    }

    void joinMapToString_Test()
    {
        std::puts("--joinMapToString_Test--");

        std::string urlBase ={"https:\\\\www.boost.org/?"};
        std::map<std::string, std::string> urlArgs{ {"help", "install"},
                                                   {"library", "multi_array"}};
        auto rngStr = urlArgs | std::views::transform([] (const std::pair<const std::string, std::string>& elemMap) ->std::string { return '&' + elemMap.first + "=" + elemMap.second;})
                              | std::views::common;

        std::accumulate(std::begin(rngStr), std::end(rngStr), std::ostream_iterator<std::string>(std::cout, " , "), [](auto&& osStr, const auto& elem) { return osStr = elem;});

        // this does not work because everytime we call a rng from views it will convert map element into single string
        // when we pass to accumulate to back_insert to a string it will expect a char but we are sending string
        //std::accumulate(rngStr.begin(),rngStr.end(), std::back_inserter(urlBase), [](auto&& init, char elem){ return init = std::move(elem); });

        for(auto elemStr : rngStr)
        {
            std::accumulate(elemStr.begin(), elemStr.end(), std::back_inserter(urlBase), [](auto&& init, char elem){ return init = elem; });
        }

        fmt::print("\nurlBase: {} \n", urlBase);

    }


    void rangesJoin_Examples()
    {
        std::puts("--rangesJoin_Examples--");

        std::vector<std::string> vecStr{"hello", "didem", "salitos", "semos"};

        auto rngStr = vecStr | std::views::join | std::views::common;

        std::string baseStr = {"www.boost.org/?"};
        auto outIt = std::back_inserter(baseStr);

        for(const char c : rngStr | std::views::take(10))
        {
            fmt::print("{} ", c);
            *outIt = c;
        }
        std::puts(" ");
        fmt::print("{}\n", baseStr);

        std::accumulate(rngStr.begin(), rngStr.end(), outIt, [](auto&& outStr, auto&& elem) { return outStr = std::move(elem);});
        fmt::print("{}\n", baseStr);
        std::puts(" ");
    }

    } //end of namespace

