cmake_minimum_required(VERSION 3.12)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

project(agar)

include_directories(environment PRIVATE 
    include
    )

file(GLOB_RECURSE srcs
     "src/client/**.cpp"
     "src/core/**.cpp"
     "src/math/**.cpp"
     "src/server/**.cpp"
     "src/state/**.cpp"
     "include/**.h"
)

file(GLOB_RECURSE test_srcs
     "src/**.cc"
)


enable_testing()

add_executable(agario ${srcs} src/main.cpp )
add_executable(agario_tests ${srcs} ${test_srcs} )
set_target_properties(agario PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)

set_target_properties(agario_tests PROPERTIES 
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON)

target_link_libraries(agario boost_program_options sfml-graphics sfml-window sfml-system gtest_main)
target_link_libraries(agario_tests sfml-graphics sfml-window sfml-system gtest_main)

include(GoogleTest)
gtest_discover_tests(agario_tests)
