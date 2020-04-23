cmake_minimum_required(VERSION 3.14)

project(Bots)

## add_subdirectory(lib/websocketpp-master)

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

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Sla4shioBot Threads::Threads)

include_directories("lib/websocketpp-master")
include_directories("lib/asio-1.12.2/include")
