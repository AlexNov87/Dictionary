cmake_minimum_required(VERSION 3.30)
project(dictionary CXX)
set(CMAKE_CXX_STANDARD 23)
include(boost.cmake)
include(pqxx.cmake)
include(libs.cmake)

add_executable(srv server.cpp)
target_link_libraries(srv ${LIBS} ${Boost_LIBRARIES})

add_executable(cli client.cpp)
target_link_libraries(cli ${LIBS} clientlib ${Boost_LIBRARIES})
##
#add_executable(del deleter.cpp)
