cmake_minimum_required(VERSION 3.22)
project(HttpWeatherClient)

set(CMAKE_CXX_STANDARD 20)

set(BOOST_ROOT /usr/include/boost)
set(Boost_USE_STATIC_LIBS ON)

find_package(
        Boost 1.81 COMPONENTS
        system
        thread
        json
        program_options)

include_directories(${Boost_INCLUDE_DIR})
file(GLOB SOURCES "src/sources/*.cpp")
file(GLOB HEADERS "src/headers/*.hpp")
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
