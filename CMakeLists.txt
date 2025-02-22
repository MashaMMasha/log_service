cmake_minimum_required(VERSION 3.10)
project(LogServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(log_server
        main.cpp
        src/api/http_server.cpp
)

target_include_directories(log_server PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(log_server PRIVATE ${Boost_LIBRARIES} nlohmann_json::nlohmann_json)