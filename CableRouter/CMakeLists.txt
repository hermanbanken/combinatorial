cmake_minimum_required(VERSION 2.8.4)
project(CableRouter)

SET(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/4.9.1/bin/g++-4.9" )
SET(CMAKE_CPP_COMPILER "/usr/local/Cellar/gcc/4.9.1/bin/gcc-4.9" )

set(CMAES_CXX_FLAGS "-g -Wall -fopenmp -lgflags -lstdc++ -lcmaes")
set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} ${CMAES_CXX_FLAGS}")

set(SOURCE_FILES main.cpp reader.cpp simplifier.cpp structures.cpp ga/ga.cpp graph/graph.cpp graph/sampler.cpp graph/dijkstrasolver.cpp graph/astarsolver.cpp graph/thetastarsolver.cpp)
add_executable(CableRouter ${SOURCE_FILES})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/vendor/libcmaes/src /usr/local/Cellar/eigen/3.2.2/include/eigen3")
target_include_directories (CableRouter PUBLIC /usr/local/Cellar/eigen/3.2.2/include/eigen3)
target_include_directories (CableRouter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/libcmaes)
target_include_directories (CableRouter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/libcmaes/src)

SET (CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT} $ENV{LDFLAGS} -L/usr/local/lib
     CACHE STRING "Flags used by the linker during the creation of dll's.")
