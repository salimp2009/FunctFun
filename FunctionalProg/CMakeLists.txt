cmake_minimum_required(VERSION 3.21)
project(FunctionalProg)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(IS_GNU On)
    add_definitions(-DIS_GCC=true)
else()
    set(IS_GNU Off)
    add_definitions(-DIS_GCC=false)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set(IS_CLANG On)
    add_definitions(-DIS_CLANG=true)
else()
    set(IS_CLANG Off)
    add_definitions(-DIS_CLANG=false)
endif()

if(MSVC)
    add_definitions(-DIS_MSVC=true)
    #add_definitions(/WX)
    add_definitions(/W3)
    add_definitions(/wd4189)
    add_definitions(/wd4100)
    add_definitions(/wd4996) # 'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
    add_definitions(/wd4267)
    add_definitions(/wd4505)
    #add_definitions(/await) # enable coroutine support

    # Use sane and nice C++ for MSVC.
    # This makes alternative tokens (not, and, ...) as actual keywords and
    # enables more conformant C++ in general
    add_definitions(/permissive-)
    #add_definitions(/std:c++latest)

else()
    add_definitions(-DIS_MSVC=false)
    #add_definitions(-Werror)
    add_definitions(-Wall)
    add_definitions(-Wextra)
    add_definitions(-Wold-style-cast)
    add_definitions(-Wno-unused-variable)
    add_definitions(-Wno-unused-parameter)
    add_definitions(-Wno-unused-value)
    add_definitions(-Wno-cpp)
    add_definitions(-Wconversion)
    add_definitions(-Wpessimizing-move)

    if(IS_GNU)
        add_definitions(-Wmisleading-indentation)
        add_definitions(-Wduplicated-cond)
        add_definitions(-Wduplicated-branches)
        add_definitions(-Wlogical-op)  # warn about logical operations being used where bitwise were probably wanted
        add_definitions(-Wuseless-cast) # warn if you perform a cast to the same type
    endif()

    if(IS_CLANG)
        add_definitions(-Wno-unused-private-field)
        add_definitions(-Wno-unneeded-internal-declaration)

    else()
        add_definitions(-fcoroutines) # required for GCC-10
        #add_definitions(-fpermissive) # to relax typename requirement

    endif()
endif()


#added to use precompiled headers
add_library(functProgPCH INTERFACE)
target_precompile_headers(functProgPCH INTERFACE
        [["functProgPCH.hpp"]]
        )

target_include_directories(functProgPCH  INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
        )

add_library(funcFunLib
        src/functProgPCH.hpp
        src/FunctFun/functfun.hpp
        src/FunctFun/functfun.cpp
        src/FunctFun/MapFilesToLineCounts.hpp
        src/FunctFun/MakeNetWorkRequest.hpp
        src/FunctFun/arithmeticMeanUtils.hpp
        src/FunctFun/makeUniformDistribution.hpp
        src/FunctFun/stringTrim.hpp
        src/FunctFun/stringTrim.cpp
        src/FunctFun/functUtils.hpp
        src/FunctFun/jsonDataTypes.hpp
        src/FunctFun/renderJsonValues.hpp
        src/FunctFun/fold_variant.hpp
        src/FunctFun/JsonDataTest.cpp
        src/FunctFun/join.hpp
        src/FunctFun/mapview.hpp
        src/FunctFun/viewAdaptors.hpp
        src/FunctFun/rangeutils.hpp
        src/FunctFun/arithmeticVector.hpp
        src/FunctFun/joinwithview.hpp
        src/FunctFun/rangeconcepts.hpp
        src/FunctFun/functOptional.hpp
        src/FunctFun/cartesianproduct.hpp)


#USE this; once conan new is used
#find_package(fmt REQUIRED)
#find_package(range-v3 REQUIRED)
#find_package(Boost REQUIRED)
#find_package(ut REQUIRED)


target_include_directories(funcFunLib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_link_libraries(funcFunLib PRIVATE functProgPCH)
target_link_libraries(funcFunLib PRIVATE
                                #DELETE this; once conan new is used
                                CONAN_PKG::fmt
                                CONAN_PKG::range-v3
                                #USE this; once conan new is used
                                #fmt::fmt
                                #range-v3::meta
                                #range-v3::concepts
                                #Boost::system
                                #boost::ut
                                )



add_executable(FunctFun src/functProgTest.cpp src/FunctFun/mapview.hpp)

target_link_libraries(FunctFun PRIVATE
                            project_options
                            funcFunLib)
                            #DELETE THIS !!; after convertion conan to new version
                            #PUBLIC
                            #CONAN_PKG::fmt
                            #CONAN_PKG::range-v3 )
