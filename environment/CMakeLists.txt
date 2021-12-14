cmake_minimum_required(VERSION 3.16)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF)

project(agar)

include_directories(environment PRIVATE 
    include
    )

file(GLOB_RECURSE srcs
     "src/**.cpp"
     "include/**.h"
)

message(STATUS "${srcs}")

add_executable(agario ${srcs} )
target_link_libraries(agario boost_program_options)
target_link_libraries(agario sfml-graphics sfml-window sfml-system)
