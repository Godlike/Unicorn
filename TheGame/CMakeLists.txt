cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(TheGame)

include(VorpalEngineConfig)

include_directories(${VORPAL_ENGINE_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} ${VORPAL_ENGINE_LIB})
