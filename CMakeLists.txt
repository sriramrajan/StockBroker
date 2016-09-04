cmake_minimum_required(VERSION 3.0)
file ( GLOB SRCS src/*.cpp src/*.h)
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost REQUIRED COMPONENTS date_time filesystem system thread)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

if(COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    MESSAGE("Boost information:")
    MESSAGE("  Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    add_executable(broker ${SRCS})
    target_link_libraries(broker ${Boost_LIBRARIES})
endif()
