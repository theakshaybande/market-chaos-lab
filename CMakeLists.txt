cmake_minimum_required(VERSION 3.16)
project(market-chaos-lab VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(market-chaos-lab
    src/main.cpp
    src/orderbook.cpp
    src/trader.cpp
    src/market.cpp
)

target_include_directories(market-chaos-lab PRIVATE include)

if(MSVC)
    target_compile_options(market-chaos-lab PRIVATE /W4 /permissive-)
else()
    target_compile_options(market-chaos-lab PRIVATE -Wall -Wextra -Wpedantic)
endif()
