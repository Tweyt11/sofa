cmake_minimum_required(VERSION 3.1)

project(SofaGeometry VERSION 1.0)

find_package(SofaFramework REQUIRED)

set(HEADER_FILES
    src/SofaGeometry/config.h
    src/SofaGeometry/Plane.h
    src/SofaGeometry/Ray.h
    src/SofaGeometry/Constants.h
    src/SofaGeometry/CGAL/Point2.h
)

set(SOURCE_FILES
    src/SofaGeometry/Plane.cpp
    src/SofaGeometry/Ray.cpp
    src/SofaGeometry/Constants.cpp
)

set(EXTRA_FILES
    SofaGeometryConfig.cmake.in
    )

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SOFAGEOMETRY")

find_package(CGAL REQUIRED)
if(!CGAL_FOUND)
    message(FATAL_ERROR "-- TEST")
    return()
else()
    message(STATUS "CGAL FOUND")
    target_include_directories(${PROJECT_NAME} PUBLIC ${CGAL_INCLUDE_DIRS})
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC SofaCore ${CGAL_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")

## Install rules for the library and headers; CMake package configurations files
sofa_create_package(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})

## When installing, copy the content of the example directory into module/SofaGeometry
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION modules/SofaGeometry COMPONENT resources)

## When installing, copy the target into module/SofaGeometry
install(TARGETS ${PROJECT_NAME} DESTINATION modules/SofaGeometry)

add_executable(test_CGAL

    src/SofaGeometry/CGAL/test_CGAL.cpp
    src/SofaGeometry/CGAL/Point2.h

)

    ######  LIBRARY LINKS    ###########################################

    target_link_libraries(test_CGAL ${CGAL_LIBRARY})


## Add the test subdirectory.
add_subdirectory(tests)

## Add the python binding subdirectory.
add_subdirectory(python)
