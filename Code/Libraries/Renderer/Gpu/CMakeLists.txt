add_library(Gpu)

welder_setup_library(Gpu ${CMAKE_CURRENT_LIST_DIR} TRUE)
welder_use_precompiled_header(Gpu ${CMAKE_CURRENT_LIST_DIR})

target_sources(RHI
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Gpu.hpp
    ${CMAKE_CURRENT_LIST_DIR}/GpuStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GpuStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_link_libraries(Gpu
  PUBLIC
    Common
)