cmake_minimum_required(VERSION 3.14)

project(Sl4shioBots)

## add_subdirectory(lib/websocketpp-master)

#add_subdirectory(src/base)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(server
"src/Bot.hpp"
"src/Bot.cpp"
"src/Server.hpp"
"src/Server.cpp"
"src/linearmath.h"
"src/poolAlloc.c"
"src/poolAlloc.h"
"src/DefaultBehavior.hpp"
"src/DefaultBehavior.cpp"
"src/GameView.hpp"
"src/GameView.cpp"
"src/Utils.cpp"
"src/Utils.hpp"
"src/ClientRequest.cpp"
)

add_executable(client
  "src/Client.cpp"
  )

add_subdirectory(lib)

target_include_directories(server
  PUBLIC "src"
  )
target_link_libraries(server
  lib
  )

target_link_libraries(client
  lib
  )
