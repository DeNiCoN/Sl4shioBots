cmake_minimum_required(VERSION 3.14)

add_subdirectory(lib/websocketpp-master)

add_executable(Sla4shioBot
"Main.cpp"
"Bot.h"
"Bot.cpp"
"World.h"
"World.cpp"
"linearmath.h"
"DefaultBehavior.h"
"DefaultBehavior.cpp"
"GameView.h"
"GameView.cpp"
)

if(WIN32)
  target_link_libraries(Sla4shioBot wsock32 ws2_32)
endif()

include_directories("lib/websocketpp-master")
include_directories("lib/asio-1.12.2/include")