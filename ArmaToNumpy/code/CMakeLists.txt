cmake_minimum_required(VERSION 2.6)
project(structure)

set(CMAKE_MODULE_PATH
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_MODULE_PATH}"
	)

message("cmake_module_path: ${CMAKE_MODULE_PATH}")

include(FindBLAS)
include(FindLAPACK)
include(PythonSetup)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall -DARMA_DONT_USE_WRAPPER")

message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})

include_directories(${PYTHON_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/pybind11/include)
include_directories(${PROJECT_SOURCE_DIR}/armadillo/include)

file(GLOB SOURCES "src/*.cpp")

add_library(cpp_bfmatch SHARED
	${SOURCES}
	)

target_link_libraries(cpp_bfmatch ${LAPACK_LIBRARIES})

set_target_properties(cpp_bfmatch PROPERTIES PREFIX "")
set_target_properties(cpp_bfmatch PROPERTIES SUFFIX ".so")

add_custom_command(TARGET cpp_bfmatch
						POST_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:cpp_bfmatch> ${PROJECT_SOURCE_DIR}/..)
