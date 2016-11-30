cmake_minimum_required(VERSION 2.8)
 
PROJECT(compare_vtk)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


add_executable(compare_vtk compare_vtk.cxx)
add_executable(WritePointScalarData WritePointScalarData.cxx)
 
if(VTK_LIBRARIES)
  target_link_libraries(compare_vtk ${VTK_LIBRARIES})
  target_link_libraries(WritePointScalarData ${VTK_LIBRARIES})
endif()