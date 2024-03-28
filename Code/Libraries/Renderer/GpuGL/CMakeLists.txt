add_library(GpuGL)

zero_setup_library(GpuGL ${CMAKE_CURRENT_LIST_DIR} TRUE)
zero_use_precompiled_header(GpuGL ${CMAKE_CURRENT_LIST_DIR})

target_sources(GpuGL
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/gl_ext.h
    ${CMAKE_CURRENT_LIST_DIR}/glcorearb.h
    ${CMAKE_CURRENT_LIST_DIR}/khrplatform.h
    ${CMAKE_CURRENT_LIST_DIR}/renderdoc_app.h

    ${CMAKE_CURRENT_LIST_DIR}/GpuGL.cpp  
    ${CMAKE_CURRENT_LIST_DIR}/GpuGLStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GpuGLStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_link_libraries(GpuGL
  PUBLIC
    Gpu
    GL
)