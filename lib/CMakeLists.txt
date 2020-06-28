include(FetchContent)

add_library(lib INTERFACE)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_compile_definitions(lib INTERFACE
  _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
  _WEBSOCKETPP_CPP11_TYPE_TRAITS_
  ASIO_STANDALONE
  )

find_package(websocketpp REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

FetchContent_Declare(asio
  GIT_REPOSITORY    https://github.com/chriskohlhoff/asio/
  GIT_TAG           master
  GIT_PROGRESS      TRUE
  )

FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
  message("Populating asio")
  FetchContent_Populate(asio)
endif()

target_link_libraries(lib
  INTERFACE websocketpp::websocketpp
  INTERFACE Threads::Threads
  INTERFACE CURL::libcurl
  INTERFACE nlohmann_json::nlohmann_json
  )

target_include_directories(lib
  INTERFACE ${asio_SOURCE_DIR}/asio/include
  )
