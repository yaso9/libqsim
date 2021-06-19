cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

project(quantum_simulator)

find_package(ArrayFire REQUIRED)
include_directories(${ArrayFire_INCLUDE_DIRS})

add_executable(quantum_simulator src/main.cpp src/Gate.cpp src/Circuit.cpp src/utils.cpp)

target_link_libraries(quantum_simulator ${ArrayFire_LIBRARIES})

install(TARGETS quantum_simulator RUNTIME DESTINATION bin)
