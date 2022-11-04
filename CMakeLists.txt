cmake_minimum_required(VERSION 3.1)
project(tracer VERSION 1.0)

find_package(GTest REQUIRED)
find_package(PNG REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(nlohmann_json REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Werror")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tracer src/main.cpp src/object.cpp src/scene.cpp src/math.cpp)
target_link_libraries(tracer PNG::PNG Boost::program_options nlohmann_json::nlohmann_json)
target_include_directories(tracer PRIVATE include)

add_executable(math_test test/math_test.cpp src/math.cpp src/object.cpp)
target_include_directories(math_test PRIVATE include)
target_link_libraries(math_test GTest::GTest)
