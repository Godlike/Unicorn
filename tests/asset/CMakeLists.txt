cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(AssetTest)

include(UnicornRenderConfig)

include_directories(${UNICORN_RENDER_INCLUDE_DIRS})

find_package(Threads)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

add_executable(SimpleStorageTest SimpleStorageTest.cpp)
target_link_libraries(SimpleStorageTest ${UNICORN_RENDER_LIB})
add_test(
    NAME SimpleStorageTest
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/TestInput"
    COMMAND SimpleStorageTest
)

add_executable(StorageTest StorageTest.cpp)
target_link_libraries(StorageTest ${UNICORN_RENDER_LIB})
add_test(
    NAME StorageTest
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/TestInput"
    COMMAND StorageTest
)
