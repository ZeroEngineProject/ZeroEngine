add_library(GraphicsNext)

zero_setup_library(GraphicsNext ${CMAKE_CURRENT_LIST_DIR} TRUE)
zero_use_precompiled_header(GraphicsNext ${CMAKE_CURRENT_LIST_DIR})

target_sources(GraphicsNext
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/GraphicsNextEngine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GraphicsNextEngine.hpp
    ${CMAKE_CURRENT_LIST_DIR}/GraphicsNextSpace.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GraphicsNextSpace.hpp
    ${CMAKE_CURRENT_LIST_DIR}/GraphicsNextStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GraphicsNextStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
)

target_link_libraries(GraphicsNext
  PUBLIC
    Common
    Geometry
    Image
    Meta
    Serialization
    SpatialPartition
    ZilchShaders
    Zilch
    FreeType
    Gpu
)
