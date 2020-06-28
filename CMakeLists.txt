cmake_minimum_required(VERSION 3.14)

project(Bots)

## add_subdirectory(lib/websocketpp-master)

#add_subdirectory(src/base)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Sla4shioBot
"src/Main.cpp"
"src/Bot.h"
"src/Bot.cpp"
"src/World.h"
"src/World.cpp"
"src/linearmath.h"
"src/poolAlloc.c"
"src/poolAlloc.h"
"src/DefaultBehavior.h"
"src/DefaultBehavior.cpp"
"src/GameView.h"
"src/GameView.cpp"
"src/Utils.cpp"
"src/Utils.h"
)

add_subdirectory(lib)

target_include_directories(Sla4shioBot
  PUBLIC "src"
  )
target_link_libraries(Sla4shioBot
  lib
  )
