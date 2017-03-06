# based on Hoomd-Blue's PythonSetup.cmake

macro(run_python code result)
execute_process(
	COMMAND
	${PYTHON_EXECUTABLE} -c ${code}
	OUTPUT_VARIABLE ${result}
	RESULT_VARIABLE PY_ERR
	OUTPUT_STRIP_TRAILING_WHITESPACE
	)

if(PY_ERR)
	message(STATUS "Error while querying python for info")
endif(PY_ERR)
endmacro(run_python)

find_package(PythonInterp REQUIRED)

# get python version
run_python("import sys\; print('%d' % (sys.version_info[0]))" PYTHON_VERSION_MAJOR)
run_python("import sys\; print('%d' % (sys.version_info[1]))" PYTHON_VERSION_MINOR)
set(PYTHON_VERSION "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}")
string(REPLACE "." "" _python_version_no_dots ${PYTHON_VERSION})

# determine python include directories
run_python("from distutils import sysconfig\; print(sysconfig.get_python_inc())" _python_include_hint)
run_python("from distutils import sysconfig\; print(sysconfig.PREFIX)" _python_prefix_hint)
run_python("from distutils import sysconfig\; print(sysconfig.get_config_var('LIBPL'))" _python_static_hint)
run_python("from distutils import sysconfig\; print(sysconfig.get_config_var('LIBRARY'))" _python_static_lib_name)
run_python("from distutils import sysconfig\; print(sysconfig.get_config_var('LDLIBRARY'))" _python_dynamic_lib_name)
run_python("from distutils import sysconfig\; print(sysconfig.get_python_lib(plat_specific=True))" PYTHON_SYSTEM_SITE)

get_filename_component(_python_lib_first ${_python_dynamic_lib_name} NAME)
find_path(PYTHON_INCLUDE_DIR Python.h
			HINTS ${_python_include_hint}
			NO_DEFAULT_PATH)

set(_old_suffixes ${CMAKE_FIND_LIBRARY_SUFFIXES})
set(CMAKE_FIND_LIBRARY_SUFFIXES ";${CMAKE_FIND_LIBRARY_SUFFIXES}")
find_library(PYTHON_LIBRARY
				NAMES ${_python_lib_first} python${_python_version_no_dots} python${PYTHON_VERSION}
				HINTS ${_python_prefix_hint} ${_python_static_hint} ${_python_lib_hint}
				PATH_SUFFIXES lib64 lib libs
				NO_DEFAULT_PATH)
set(${CMAKE_FIND_LIBRARY_SUFFIXES} _old_suffixes)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PythonLibs DEFAULT_MSG PYTHON_LIBRARY PYTHON_INCLUDE_DIR PYTHON_SYSTEM_SITE)

message("PYTHON_LIBRARY: ${PYTHON_LIBRARY}")
