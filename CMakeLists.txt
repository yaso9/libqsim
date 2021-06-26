cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(qsim VERSION 1.0.0 DESCRIPTION "A library for simulating quantum circuits")

find_package(ArrayFire REQUIRED)
include_directories(${ArrayFire_INCLUDE_DIRS})

add_library(qsim SHARED src/Gate.cpp src/Circuit.cpp src/utils.cpp)
set_target_properties(qsim PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(qsim PROPERTIES SOVERSION ${PROJECT_VERSION})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests src/tests/all.cpp)
target_link_libraries(tests gtest_main qsim ${ArrayFire_LIBRARIES})

include(GoogleTest)
gtest_discover_tests(tests)