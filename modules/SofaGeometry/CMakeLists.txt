cmake_minimum_required(VERSION 3.1)

project(SofaGeometry VERSION 1.0)

find_package(SofaFramework REQUIRED)

set(HEADER_FILES
    src/SofaGeometry/config.h
    src/SofaGeometry/Plane.h
    src/SofaGeometry/Ray.h
    src/SofaGeometry/Constants.h
    src/SofaGeometry/CGAL/Kernel.h
    src/SofaGeometry/CGAL/Point2.h
    src/SofaGeometry/CGAL/Polygon2.h
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

set(CGAL_DIR
    /usr/local/include/CGAL
    )

# CGAL and its components
find_package( CGAL COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()

endif()

# include helper file
include( ${CGAL_USE_FILE} )


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()

endif()

# include for local directory

# include for local package





#find_package(CGAL_REQUIRED)
#if(!CGAL_FOUND)
#    message(FATAL_ERROR "-- TEST")
#    return()
#else()
#    message(STATUS "CGAL FOUND")
#    target_include_directories(${PROJECT_NAME} PUBLIC ${CGAL_INCLUDE_DIRS})
#endif()

target_link_libraries(${PROJECT_NAME} PUBLIC SofaCore ${CGAL_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")


## Install rules for the library and headers; CMake package configurations files
sofa_create_package(${PROJECT_NAME} ${PROJECT_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})

## When installing, copy the content of the example directory into module/SofaGeometry
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples DESTINATION modules/SofaGeometry COMPONENT resources)

## When installing, copy the target into module/SofaGeometry
install(TARGETS ${PROJECT_NAME} DESTINATION modules/SofaGeometry)


    ######  LIBRARY LINKS    ###########################################


## Add the test subdirectory.
add_subdirectory(tests)

## Add the python binding subdirectory.
add_subdirectory(python)
