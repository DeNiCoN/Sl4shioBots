include(FetchContent)

add_library(lib INTERFACE)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_compile_definitions(lib INTERFACE
  _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
  _WEBSOCKETPP_CPP11_TYPE_TRAITS_
  ASIO_STANDALONE
  )

find_package(websocketpp QUIET)
if(NOT websocketpp_FOUND)
  message("websocketpp not found. Downloading...")
  FetchContent_Declare(websocketpp
    GIT_REPOSITORY    https://github.com/zaphoyd/websocketpp
    GIT_TAG           master
    GIT_PROGRESS      TRUE
    )

  FetchContent_GetProperties(websocketpp)
  if(NOT websocketpp_POPULATED)
    message("Populating websocketpp")
    FetchContent_Populate(websocketpp)
    add_subdirectory(${websocketpp_SOURCE_DIR} ${websocketpp_BINARY_DIR})
  endif()
  find_package(websocketpp REQUIRE)
endif()

target_link_libraries(lib
  INTERFACE websocketpp::websocketpp
  INTERFACE Threads::Threads
  )

find_package(Boost COMPONENTS system QUIET)
if(NOT Boost_FOUND)
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
  target_include_directories(lib
    INTERFACE ${asio_SOURCE_DIR}/asio/include
    )
endif()


if(BOOST_ASIO_FOUND)
target_link_libraries(lib
  INTERFACE ${BOOST_LIBRARIES}
  )
endif()
