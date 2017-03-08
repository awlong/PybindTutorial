cmake_minimum_required(VERSION 2.6)
project(structure)

set(LIBNAME "example")

set(CMAKE_MODULE_PATH
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_MODULE_PATH}"
	)

message("cmake_module_path: ${CMAKE_MODULE_PATH}")

include(FindBLAS)
include(FindLAPACK)
include(PythonSetup)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -Wall")

message(STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS})

include_directories(${PYTHON_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../../pybind11/include)

file(GLOB SOURCES "src/*.cpp")

add_library(${LIBNAME} SHARED
	${SOURCES}
	)

target_link_libraries(${LIBNAME} ${LAPACK_LIBRARIES})

set_target_properties(${LIBNAME} PROPERTIES PREFIX "")
set_target_properties(${LIBNAME} PROPERTIES SUFFIX ".so")

add_custom_command(TARGET ${LIBNAME}
						POST_BUILD
						COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${LIBNAME}> ${PROJECT_SOURCE_DIR}/..)
