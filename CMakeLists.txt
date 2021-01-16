cmake_minimum_required(VERSION 3.17)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set directories
set(WORKSPACE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_DIR ${WORKSPACE_DIR}/src/)
set(INC_DIR ${WORKSPACE_DIR}/include/)
set(READERS_DIR ${WORKSPACE_DIR}/readers/)
set(WRITERS_DIR ${WORKSPACE_DIR}/writers/)
set(READERS_OUTPUT_DIR ${WORKSPACE_DIR}/reader_output/)

##########################################################################################################################

project(LRUCache)
add_executable(LRUCache)

configure_file( configuration.h.in ${INC_DIR}/configuration.h)

target_sources(LRUCache PRIVATE ./src/main.cpp)

target_include_directories(LRUCache PRIVATE ${INC_DIR})

target_link_libraries(${PROJECT_NAME} -lpthread)



