cmake_minimum_required(VERSION 3.14)

add_subdirectory(lib/websocketpp-master)

add_executable(Sla4shioBot
"src/Main.cpp"
"src/Bot.h"
"src/Bot.cpp"
"src/World.h"
"src/World.cpp"
"src/linearmath.h"
"src/DefaultBehavior.h"
"src/DefaultBehavior.cpp"
"src/GameView.h"
"src/GameView.cpp"
)

if(WIN32)
  target_link_libraries(Sla4shioBot wsock32 ws2_32)
endif()

include_directories("lib/websocketpp-master")
include_directories("lib/asio-1.12.2/include")