# Sl4shioBots
## About
Bots for online browser game sl4sh.io writed using C++ with websocketpp and asio libraries.\ 
Command line interface is made as client-server application
## Build
In order to build the CMake and git must be installed\
Requires curl, websocketpp and nlohmann::json libraries installed\
\
From command line:
```
git clone https://github.com/DeNiCoN/Sl4shioBots.git
cd Sl4shioBots
cmake -B build -S .

cd build
cmake --build .
```
## Run
Start the server:
```
./server [port]
```
Start the client:
```
./client <host> [port]
```
Default port is 34534

## Available client commands

`connect <name>` - connects bot named 'name'\
`disconnect <name>` - disconnects bot named 'name'\
`state <name>` - prints current state of bot named 'name'\
`stats <name>` - prints statistics of bot named 'name'
