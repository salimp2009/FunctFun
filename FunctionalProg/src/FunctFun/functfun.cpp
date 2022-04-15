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
#include "mapview.hpp"
#include "arithmeticVector.hpp"
#include "joinwithview.hpp"
#include "cartesianproduct.hpp"


#include <range/v3/algorithm.hpp>
#include <range/v3/all.hpp>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

// FIXME: this is a work around to makelazy_split work
//  due to a bug in STL version remove it when STL version is corrected
#define lazy_split_view split_view
#define lazy_split split

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
        using namespace std::literals;

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
                 return  elem.first + '=' + elem.second;
             });
        fmt::print("urlBase after join: {} \n", urlBase);

        std::vector<char> vecChar;
        join(std::cbegin(urlArgs), std::cend(urlArgs),
             std::ostream_iterator<char>(std::cout), "-&-"sv,
             [](const std::pair<const std::string&, std::string>& elem)
             {
                 return  elem.first + '=' + elem.second;
             });

        std::puts("\nRange version;");

        join(urlArgs, std::ostream_iterator<char>(std::cout), "&",
             [](const std::pair<const std::string&, std::string>& elem)
             {
                 //const auto& [key, val] = elem;
                 return  elem.first + '=' + elem.second;
             });

        std::puts("--");
    }

    void joinStrings_Test()
    {
        std::puts("--joinStrings_Test--");
        std::string myStr = {"salitos and Didos"};

        auto it = std::back_inserter(myStr);
        *it = 'C';
        std::string newStr = "& : html=stupid.com";
        fmt::print("{}", myStr);
        std::copy(newStr.begin(), newStr.end(), std::back_inserter(myStr));
        fmt::print("{}", myStr);

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

        fmt::print("{} \n", urlBase);

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

        std::string urlBase{"https:\\\\www.boost.org/?"};
        std::map<std::string, std::string> urlArgs{ {"help", "install"},
                                                   {"library", "multi_array"}};

        // this does not work with join; the inner has to be a single type
        // or a container of the same type; otherwise we need to use transform to convert
        // inner data to match a single type
        // auto rngMap = urlArgs | std::views::join;
        auto rngMap =urlArgs
                   | std::views::transform( [](const std::pair<const std::string, std::string>& elem ) { return elem.first + ':' + elem.second;})
                   | std::views::join | std::views::common;

        std::accumulate(std::begin(rngMap), std::end(rngMap), std::back_inserter(urlBase), [](auto outStr, auto&& elem) { return *outStr = std::move(elem);});

        fmt::print("{} \n", urlBase);

        std::vector<std::vector<int>> vecInt {{1,2} ,{3},{4,5}};
        auto rngInt = vecInt | std::views::join | std::views::common;
        for(const auto& val: rngInt)
        {
            fmt::print("{} ", val);
        }
        std::puts("");

    }

    void joinFuncObj_Test()
    {
        std::puts("--joinFuncObj_Test--");

        std::string urlBase{"https:\\\\www.boost.org/?"};
        std::map<std::string, std::string> urlArgs{ {"help", "install"},
                                                   {"library", "multi_array"}};

        join(std::cbegin(urlArgs), std::cend(urlArgs), std::back_insert_iterator(urlBase), '&',
                [](const std::pair<const std::string&, std::string>& elem)
                {
                    return  elem.first + '=' + elem.second;
                });

        fmt::print("{} \n", fmt::join(urlBase, ""));

        std::puts("range version of joinv2 function object;");
        join(urlArgs, std::ostream_iterator<char>(std::cout), '&',
               [](const std::pair<const std::string&, std::string>& elem)
               {
                   return  elem.first + '=' + elem.second;
               });

        decltype(auto) test = "--&--";
        //static_assert(std::is_same_v<char, std::remove_pointer_t<std::remove_cvref_t<decltype(test)>>>);

    }

    void splitview_Test()
    {
        std::puts("--splitview_Test--");
        constexpr static auto numbers = {0,1,0, 2,3,0, 3,4,5,0, 6,7,8};
        constexpr int delimiter{0};
        for(auto const& innerRng: numbers | std::views::lazy_split(delimiter))
        {
            fmt::print("{} \n ", fmt::join(innerRng, " "));
        }

        for(auto const& innerRng : numbers | std::views::split(delimiter))
        {
            fmt::print("{} \n", fmt::join(innerRng, " "));
        }

        using namespace std::literals;
        // this can be used to support as delimiter other than char
        constexpr std::ranges::single_view vwDel{delimiter};
        const auto& del = "-&-";
        std::ranges::single_view vwDel2{del};


        // FIXME : this is hack to understand how to std::ranges::single::view; DELETE once we dont need it
        auto size =0;
        for(auto temp = *vwDel2.data(); (*temp)!=0; ++temp )
        {
            size+=1;
            fmt::print("temp: {}, size: {} \n", *temp, size);
        }
        fmt::print("vwDel2: {},  size: {}\n", *vwDel2.data(), size);


        fmt::print("vwDel:{} \n", fmt::join(vwDel, ""));
        fmt::print("{}, [begin: {}] \n", fmt::join(vwDel2, ""), *vwDel2.begin());
        fmt::print("{} \n",*vwDel.data());
        const auto length = std::ranges::distance(vwDel2.begin(), vwDel2.end());
        fmt::print("{} \n", length);


        constexpr std::string_view sv{"-&-"};
        std::string tempStr ="testJoin";
        std::copy(sv.begin(), sv.end(), std::ostream_iterator<char>(std::cout, ""));
        std::copy(sv.begin(), sv.end(), std::back_inserter(tempStr));
        fmt::print("\n{} \n", tempStr);
    }

    void mapview_Test()
    {
        std::puts("--mapview_Test--");
        std::vector vec1{1,2,3,4,5,6,7,8,9,10};

        auto resultRng = vec1 | std::views::transform([](auto elem) { return elem*2;});

        fmt::print("std::ranges::views::transform result-> {}\n", fmt::join(resultRng, " "));
        fmt::print("original vec1 -> {}\n", fmt::join(vec1, " "));

        auto resultRng2  = vec1 | views::map([](auto&&elem) { return elem*2;}) | std::ranges::views::reverse;
        using R = decltype(resultRng2);
        static_assert(std::ranges::bidirectional_range<R>);
        static_assert(std::ranges::common_range<R>);

        fmt::print("views::map result-> {}\n",fmt::join(resultRng2, " "));
        fmt::print("{} ", vec1 | views::map([](auto&&elem) { return elem*2;}) | std::ranges::views::reverse);
        fmt::print("\noriginal vec1 -> {}\n", fmt::join(vec1, " "));
    }

    void expressionTemplates_Test()
    {
        std::puts("--expressionTemplates_Test--");
        mathVec<double> mvec1(10, 5.4);
        mathVec<double>mvec2(10, 10.3);
        mathVec<double>result(10);

        result =mvec1+mvec1+mvec2*mvec2;
        fmt::print("{}", result.data());
    }

    void joinwithview_Test()
    {
        std::puts("--joinwithview_Test--");
        std::vector<std::vector<int>> vec1={{1,2,3,4,5,6}};
        static_assert(std::is_reference_v<std::ranges::range_reference_t<decltype(vec1)>>);
        static_assert(std::is_same_v<std::ranges::range_reference_t<decltype(vec1)>, std::vector<int>&>);

        std::vector<std::string> vs = {"salim", "didem", "semos", "demir"};
        auto testPattern1 = ' ';
        auto testPatternRng = std::ranges::views::single(testPattern1);
        auto testOuterRng = std::ranges::views::all(vs);

        // red squiggles are false errors ; probably due to clang_tidy
        static_assert(std::is_reference_v<std::ranges::range_reference_t<decltype(vs)>>);
        static_assert(std::same_as<std::ranges::range_value_t<decltype(vs)>, std::string>);
        static_assert(std::common_with<std::ranges::range_value_t<std::ranges::range_reference_t<decltype(testOuterRng)>>,
                                       std::ranges::range_value_t<decltype(testPatternRng)>>);
        static_assert(compatible_joinable_ranges<std::ranges::range_reference_t<decltype(vs)>, decltype(testPatternRng)>);
        static_assert(std::is_same_v<std::ranges::range_value_t<std::ranges::range_reference_t<decltype(vs)>>, char>);
        static_assert(std::is_same_v<std::iter_value_t<std::iter_value_t<decltype(vs)>>, char>);
        static_assert(compatible_joinable_ranges<std::ranges::range_reference_t<decltype(testOuterRng)>, decltype(testPatternRng)>);

        std::vector vec2 = {'c', 'b', 'd', 'e'};
        static_assert(std::is_reference_v<std::ranges::range_reference_t<decltype(vec2)>>);

        std::vector vec3 = {1,2,32,4,5,6,7};
        static_assert(std::is_reference_v<std::ranges::range_reference_t<decltype(vec3)>>);
    }

    void joinwith_runtimeTest()
    {
        using namespace std::literals;
        std::puts("-- joinwith_runtimeTest--");
        std::vector<std::string> vs = {"salim", "didem", "semos", "demir"};

        fmt::print("{}\n", fmt::join(views::joinWith(vs, ' '), ""));
        fmt::print("{}\n", fmt::join(vs | views::joinWith("--"sv), ""));
        fmt::print("{}\n", fmt::join(vs | views::joinWith("--"sv) | std::views::reverse, ""));

        auto joinedRange = vs | views::joinWith(' ');
        using R = decltype(joinedRange);

        static_assert(std::ranges::bidirectional_range<R>);
        static_assert(std::ranges::common_range<R>);
    }

    void transpose_trial_Test()
    {
        std::puts("--transpose_trial_Test--");
        std::vector<std::vector<int>> multiArry{{1,2,3}, {4,5,6}, {7,8,9}};
        fmt::print("multArr: {} \n", multiArry);

        //const int rawArr[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
        std::vector<std::vector<int>> rawArr = {{1,2, 3}, {4,5,6}, {7,8,9}};
        fmt::print("rowArr: {} \n", rawArr);

        auto numCol = ranges::distance(ranges::front(rawArr));
        fmt::print("num of cols: {} \n", numCol);

        auto flatRange = rawArr | ranges::views::join;
        fmt::print("flatRawarr: {} \n", flatRange);

        // Original version
        auto r_col = [flatRange, numCol] (int colNo)
        {
            return flatRange | ranges::views::drop(colNo) | ranges::views::stride(numCol);
        };

        auto r_transp = ranges::views::iota(0, numCol) | ranges::views::for_each(
                                                                    [r_col] (int col){ return ranges::yield(r_col(col));});

        auto r_transp2 = ranges::views::iota(0, numCol) | ranges::views::transform(
                                                                 [r_col] (int col){ return std::invoke(r_col, std::forward<int>(col));});

        std::printf(" ");
        for(auto&& elem : r_transp)
        {
            fmt::print("rtranpose: {} \n", elem);
        }

        std::printf(" ");
        for(auto&& elem : r_transp2)
        {
            fmt::print("rtranpose2: {} \n", elem);
        }

        // my version ; NOTE: dont use to capture [&] in the inner lamba then it causes weird problems
        auto rangeCol = [](auto&& flatRange, auto numCol) {
            return [flatRange, numCol](auto colNo) { return flatRange | ranges::views::drop(colNo) | ranges::views::stride(numCol);};
        }(flatRange, numCol);

        auto transposArr = ranges::views::iota(0, numCol) | ranges::views::for_each(
                             [rangeCol] (auto col){ return ranges::yield(rangeCol(col));});

        std::printf(" ");
        for(auto&& elem : transposArr)
        {
            fmt::print("{} ", elem);
        }
        std::puts(" ");

        auto transArr = multiArry | ranges::views::stride(3);
        fmt::print("transArr from multiArr: {} \n", transArr);

        auto transArr2 = rawArr | ranges::views::join | ranges::views::stride(3) ;
        fmt::print("transArr2 frm rowArr: {} \n", transArr2);



    }

} //end of namespace

