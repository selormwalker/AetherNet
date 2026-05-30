cmake_minimum_required(VERSION 3.10)
project(AetherNet VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Standard Socket libraries for Windows (WinSock)
if(WIN32)
    set(SOCKET_LIBS ws2_32)
endif()

add_executable(AetherNet src/main.cpp)
target_link_libraries(AetherNet PRIVATE ${SOCKET_LIBS})

if(MSVC)
    target_compile_options(AetherNet PRIVATE /W4 /O2 /permissive-)
else()
    target_compile_options(AetherNet PRIVATE -Wall -Wextra -O3 -march=native)
endif()
