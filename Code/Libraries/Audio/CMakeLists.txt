add_library(Audio)

zero_setup_library(Audio ${CMAKE_CURRENT_LIST_DIR} TRUE)
zero_use_precompiled_header(Audio ${CMAKE_CURRENT_LIST_DIR})

#Stub
#Emscripten
#SDLSTDEmpty
#Windows
#Linux

SET(AudioPlatformSources "")
SET(AudioPlatformLibraries "")

if(${ZERO_PLATFORM} STREQUAL "Windows")
  set(AudioPlatformSources ${CMAKE_CURRENT_LIST_DIR}/Windows/Audio.cpp)
elseif(${ZERO_PLATFORM} STREQUAL "SDLSTDEmpty" OR ${ZERO_PLATFORM} STREQUAL "Emscripten" OR ${ZERO_PLATFORM} STREQUAL "Linux")
  set(AudioPlatformSources ${CMAKE_CURRENT_LIST_DIR}/SDL/Audio.cpp)
  set(AudioPlatformLibraries SDL)
elseif(${ZERO_PLATFORM} STREQUAL "Stub")
  set(AudioPlatformSources ${CMAKE_CURRENT_LIST_DIR}/Empty/Audio.cpp)
else()
  message(FATAL_ERROR "No audio implementation available for target '${ZERO_PLATFORM}'on '${ZERO_TARGETOS}'.")
endif()

target_sources(Audio
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Audio.hpp
    ${CMAKE_CURRENT_LIST_DIR}/AudioStandard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/AudioStandard.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Precompiled.hpp
    ${AudioPlatformSources}
)

target_link_libraries(Audio
  PUBLIC
    Common
    ${AudioPlatformLibraries}
)
