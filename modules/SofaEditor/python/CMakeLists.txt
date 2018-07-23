cmake_minimum_required(VERSION 3.1)
project(PySofaEditor)

### Plugin dependencies
find_package(PythonLibs 2.7 QUIET)
if(!PythonLibs_FOUND)
    message(FATAL_ERROR "-- PySofaEditor requires the PythonLibs 2.7 cmake package.")
endif()

find_package(SofaEditor QUIET)
if(!SofaEditor_FOUND)
    message(FATAL_ERROR "-- PySofaEditor requires the SofaEditor module to be activated.")
endif()

find_package(SofaFramework REQUIRED)

message("DIR TO COMPILE: ${SofaEditor_INCLUDE_DIRS}")

set(SRC_FILES
    main.cpp
    )

set(CYTHON_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaEditor.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/CySofaEditor.pyx
)

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

add_custom_target(PyxSofaEditor
                  COMMAND "cython" ${CMAKE_CURRENT_SOURCE_DIR}/CySofaEditor.pyx --cplus
                  DEPENDS ${CYTHON_FILES}
                  SOURCES ${CYTHON_FILES})

message("TEST" ${INCLUDE_DIRECTORIES})

set(CYTHONIZED_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/PySofaEditor.cpp
)

## Create a new target that is a SHARED library from the given files
add_library(CySofaEditor SHARED ${SRC_FILES})
add_dependencies(CySofaEditor PyxSofaEditor)

## Remove the "lib" prefix otherwise python
set_target_properties(CySofaEditor PROPERTIES PREFIX "")

## The python binding generated o
set_target_properties(CySofaEditor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/pybinding")

## Add include path to this target
target_include_directories(CySofaEditor PRIVATE ${PYTHON_INCLUDE_DIR})

## Add link library to this target
target_link_libraries(CySofaEditor PRIVATE ${PYTHON_LIBRARIES} SofaEditor)

# The implementation of Python deliberately breaks strict-aliasing rules, so we
# compile with -fno-strict-aliasing to prevent the compiler from relying on
# those rules to optimize the code.
if(${CMAKE_COMPILER_IS_GNUCC})
    set(SOFAPYTHON_COMPILER_FLAGS "${SOFAPYTHON_COMPILER_FLAGS} -fno-strict-aliasing")
endif()

add_custom_target(PySofaEditor DEPENDS CySofaEditor SOURCES ${PYTHON_FILES})


## Python configuration file (build tree)
file(WRITE "${CMAKE_BINARY_DIR}/etc/sofa/python.d/CySofaEditor" "${CMAKE_BINARY_DIR}/lib/pybinding")

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PARENT_DIR_NAME ${PARENT_DIR} NAME)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SofaEditor DESTINATION modules/SofaEditor/python)
install(TARGETS CySofaEditor DESTINATION modules/SofaEditor/python/SofaEditor)
install(TARGETS CySofaEditor DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/lib/pybinding")
