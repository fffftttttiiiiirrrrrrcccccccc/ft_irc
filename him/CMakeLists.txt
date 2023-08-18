cmake_minimum_required(VERSION 3.23)
project(ircserv CXX)

# C++ 표준을 설정합니다.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

# 소스 파일 목록을 변수로 저장합니다.
set(SOURCES
        main.cpp
        Client.hpp
        Channel.hpp
        Server.hpp
        Client.cpp
        Channel.cpp
        Server.cpp
        )

# 실행 파일을 생성합니다.
add_executable(${PROJECT_NAME} ${SOURCES})
