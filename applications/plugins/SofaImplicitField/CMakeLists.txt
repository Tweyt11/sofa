cmake_minimum_required(VERSION 3.1)
project(SofaImplicitField)

set(SOFAIMPLICITFIELD_VERSION 1.0)
set(SOFA_HAVE_SOFAIMPLICITFIELD 1)

find_package(SofaDistanceGrid REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HEADER_FILES
    src/SofaImplicitField/config.h
    src/SofaImplicitField/initSofaImplicitField.h

    # This is backward compatibility
    src/SofaImplicitField/deprecated/SphereSurface.h
    src/SofaImplicitField/deprecated/ImplicitSurfaceContainer.h    # This is a backward compatibility file toward ScalarField
    src/SofaImplicitField/deprecated/InterpolatedImplicitSurface.h # This is a backward compatibility file toward DiscreteridField

    src/SofaImplicitField/components/geometry/DiscreteGridField.h
    src/SofaImplicitField/components/geometry/SphericalField.h
    src/SofaImplicitField/components/geometry/ScalarField.h
    src/SofaImplicitField/components/geometry/SurfaceMeshGenerationFromScalarField.h
    src/SofaImplicitField/components/mapping/ImplicitSurfaceMapping.h
    src/SofaImplicitField/components/mapping/ImplicitSurfaceMapping.inl
    src/SofaImplicitField/components/visual/PointCloudImplicitFieldVisualization.h
)

set(SOURCE_FILES
    src/SofaImplicitField/initSofaImplicitField.cpp

    ## This is a backward compatibility..
    src/SofaImplicitField/deprecated/SphereSurface.cpp
    src/SofaImplicitField/deprecated/InterpolatedImplicitSurface.cpp

    src/SofaImplicitField/components/geometry/ScalarField.cpp
    src/SofaImplicitField/components/geometry/DiscreteGridField.cpp
    src/SofaImplicitField/components/geometry/SphericalField.cpp
    src/SofaImplicitField/components/geometry/SurfaceMeshGenerationFromScalarField.cpp
    src/SofaImplicitField/components/mapping/ImplicitSurfaceMapping.cpp
    src/SofaImplicitField/components/visual/PointCloudImplicitFieldVisualization.cpp
)


set(EXTRA_FILES
    README.md
    examples/PointCloudImplicitFieldVisualization.scn
    )

add_subdirectory(SofaImplicitField_test)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} ${EXTRA_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER_FILES}")
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_HAVE_SOFAIMPLICITFIELD")
target_link_libraries(${PROJECT_NAME} PUBLIC SofaDistanceGrid)

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<INSTALL_INTERFACE:include>")

## Install rules for the library and headers; CMake package configurations files
sofa_create_package(SofaImplicitField ${SOFAIMPLICITFIELD_VERSION} ${PROJECT_NAME} SofaImplicitField)
