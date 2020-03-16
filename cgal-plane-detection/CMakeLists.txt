find_package(CGAL REQUIRED)
find_package(VTK)
find_package(Boost REQUIRED COMPONENTS filesystem program_options)

if(NOT ${CGAL_MAJOR_VERSION} GREATER_EQUAL "5")
    message(FATAL_ERROR "You need to install version of CGAL greather or equal than 5.0. Yout version is " ${CGAL_VERSION})
endif()

set(_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE _headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${_include_dir}/*.h*)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/objreader.cpp ${_headers} ${CMAKE_CURRENT_SOURCE_DIR}/src/savemesh.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/regiongrow.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/planefit.cpp  ${CMAKE_CURRENT_SOURCE_DIR}/src/vtk_vis.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/intersections.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/polygon_helper.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/processing.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/saveplanes.cpp)

set(_precompiled_header "stdafx.h")

if(MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE /Yu${_precompiled_header} PRIVATE /FI${_precompiled_header})
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.cpp PROPERTIES COMPILE_FLAGS /Yc${_precompiled_header})
endif()

if(${VTK_FOUND})
    target_compile_definitions(${PROJECT_NAME} PRIVATE VTK_VIS)
else()
    message(AUTHOR_WARNING "Cannot find vtk library. Disbale visualisation.")
endif()


target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_include_directories(${PROJECT_NAME} PRIVATE ${_include_dir} PRIVATE ${VTK_USE_FILE})
unset(_include_dir)
target_link_libraries(${PROJECT_NAME} PRIVATE CGAL::CGAL PRIVATE ${VTK_LIBRARIES} PRIVATE Boost::filesystem PRIVATE Boost::boost PRIVATE Boost::program_options)
