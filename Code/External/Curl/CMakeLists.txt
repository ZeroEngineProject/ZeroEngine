if(NOT EMSCRIPTEN)
  set(ENABLE_MANUAL OFF CACHE BOOL "")
  set(BUILD_CURL_EXE OFF CACHE BOOL "")
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "")
  set(BUILD_TESTING OFF CACHE BOOL "")
  if(WIN32)
    set(CURL_USE_SCHANNEL ON CACHE BOOL "")
    set(CURL_STATIC_CRT ON CACHE BOOL "")
  endif()

  add_subdirectory(Repo)

  add_library(Curl INTERFACE)

  target_link_libraries(Curl
    INTERFACE
      libcurl
  )
endif()
