add_library(Pretty)

zero_setup_library(Pretty ${CMAKE_CURRENT_LIST_DIR} TRUE)
zero_use_precompiled_header(Pretty ${CMAKE_CURRENT_LIST_DIR})

target_sources(Pretty
  PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/Node.hpp
  ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
  ${CMAKE_CURRENT_LIST_DIR}/PrettyStandard.cpp
  ${CMAKE_CURRENT_LIST_DIR}/PrettyStandard.hpp
)

target_link_libraries(Pretty
  PUBLIC
    Foundation
    Engine
    Graphics
    ZilchShaders
)
