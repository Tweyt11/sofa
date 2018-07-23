cmake_minimum_required(VERSION 3.1)

project(SofaEditor VERSION 1.0)

find_package(SofaFramework REQUIRED)

set(HEADER_FILES
    src/SofaEditor/config.h
    src/SofaEditor/BaseSofaEditor.h
)

set(SOURCE_FILES
    initSofaEditor.cpp
    src/SofaEditor/BaseSofaEditor.cpp
)

set(EXTRA_FILES
    SofaEditorConfig.cmake.in
    )

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SOFAEDITOR")

target_link_libraries(${PROJECT_NAME} PUBLIC SofaCore SofaSimulationGraph)
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")

## Install rules for the library and headers; CMake package configurations files
sofa_create_package(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME} SofaEditor)

find_package(PythonLibs 2.7 REQUIRED)
if(PythonLibs_FOUND)    
    sofa_set_python_directory(${PROJECT_NAME} "python")
    add_subdirectory(python)
else()
    message("Python 2.7 is not available, disabling compilation of the binding PySofaEditor")
endif()
add_subdirectory(SofaEditor_test)

## When installing, copy the content of the example directory into module/SofaEditor
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION modules/SofaEditor COMPONENT resources)

## When installing, copy the target into module/SofaEditor
install(TARGETS ${PROJECT_NAME} DESTINATION modules/SofaEditor)
