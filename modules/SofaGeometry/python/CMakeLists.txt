cmake_minimum_required(VERSION 3.1)
project(PythonSofaGeometry)

####################################################################################################
### Module dependencies
####################################################################################################
find_package(PythonLibs 2.7 QUIET)
if(!PythonLibs_FOUND)
    message(FATAL_ERROR "-- The python package 'SofaGeometry' requires the PythonLibs 2.7 cmake package.")
    return()
endif()

find_package(SofaGeometry QUIET)
if(!SofaGeometry_FOUND)
    message(FATAL_ERROR "-- The python package 'SofaGeometry' requires the SofaGeometry c++module to be activated.")
    return()
endif()

find_package(Cython QUIET)
if(!Cython_FOUND)
    message(FATAL_ERROR "-- The python package 'SofaGeometry' requires Cython to be installed to generate the bindings.")
    return()
endif()

####################################################################################################
### Module files
####################################################################################################
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/__init__.py
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/Plane.pyx
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/Plane.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/Ray.pyx
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/Ray.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/vector.pyx
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/vector.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/cpp_Plane.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/cpp_Ray.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/cpp_vector.pxd

)

set(EXAMPLES_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/example1.py
    )

# sofa_add_python_module( TARGET MyModule SOURCES ${SOURCE_FILES} DEPENDS Deps1 Deps2 CYTHONIZE True OUTPUT ${CMAKE_CURRENT_BIN_DIR})
sofa_add_python_module(TARGET PythonSofaGeometry
                       SOURCES ${SOURCE_FILES} ${EXAMPLES_FILES}
                       DEPENDS SofaGeometry
                       OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry"
                       CYTHONIZE ${Cython_FOUND}
                       )

                       
set(SOURCE_FILES_CGAL
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/__init__.py
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/cpp_point2D.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/cpp_kernel.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/cpp_polygon2D.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/cpp_vector2D.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/point2D.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/point2D.pyx
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/polygon2D.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/polygon2D.pyx
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/vector2D.pxd
    ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL/vector2D.pyx

)



set(EXAMPLES_FILES_CGAL
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/example1.py
    )

# sofa_add_python_module( TARGET MyModule SOURCES ${SOURCE_FILES} DEPENDS Deps1 Deps2 CYTHONIZE True OUTPUT ${CMAKE_CURRENT_BIN_DIR})
sofa_add_python_module(TARGET PythonSofaGeometry_CGAL
                       SOURCES ${SOURCE_FILES_CGAL} ${EXAMPLES_FILES_CGAL}
                       DEPENDS SofaGeometry
                       OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry/CGAL"
                       CYTHONIZE ${Cython_FOUND}
                       )                       
                       
                       
## Python configuration file (build tree), the lib in the source dir (easier while developping .py files)
file(WRITE "${CMAKE_BINARY_DIR}/etc/sofa/python.d/SofaGeometry" "${CMAKE_CURRENT_SOURCE_DIR}")

## Python configuration file (install tree)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/installed-SofaGeometry-config"
     "modules/SofaGeometry/python/")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/installed-SofaGeometry-config"
         DESTINATION "etc/sofa/python.d"
         RENAME "SofaGeometry")

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SofaGeometry DESTINATION modules/SofaGeometry/python)

#add_subdirectory(tests)
