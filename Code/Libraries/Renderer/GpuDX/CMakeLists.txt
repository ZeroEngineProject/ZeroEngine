add_library(GpuDX)

zero_setup_library(GpuDX ${CMAKE_CURRENT_LIST_DIR} TRUE)
zero_use_precompiled_header(GpuDX ${CMAKE_CURRENT_LIST_DIR})

target_sources(GpuDX
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/GpuDXStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GpuDXStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_link_libraries(GpuDX
  PUBLIC
    Gpu
)