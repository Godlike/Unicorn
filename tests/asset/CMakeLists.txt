cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(AssetTest)

include(VorpalEngineConfig)

include_directories(${VORPAL_ENGINE_INCLUDE_DIR})

find_package(Threads)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

add_executable(SimpleStorageTest SimpleStorageTest.cpp)
target_link_libraries(SimpleStorageTest ${VORPAL_ENGINE_LIB})
add_test(
    NAME SimpleStorageTest
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/TestInput"
    COMMAND SimpleStorageTest
)

add_executable(StorageTest StorageTest.cpp)
target_link_libraries(StorageTest ${VORPAL_ENGINE_LIB} ${CMAKE_THREAD_LIBS_INIT})
add_test(
    NAME StorageTest
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/TestInput"
    COMMAND StorageTest
)
