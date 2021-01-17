cmake_minimum_required(VERSION 3.16)

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
set(DATA_DIR ${WORKSPACE_DIR}/data/)
set(BUILD_DIR ${WORKSPACE_DIR}/build/)

##########################################################################################################################

project(LRUCache)
add_executable(LRUCache)

configure_file( configuration.h.in ${INC_DIR}/configuration.h)

target_sources(LRUCache PRIVATE ./src/main.cpp ./src/reader.cpp ./src/writer.cpp)

target_include_directories(LRUCache PRIVATE ${INC_DIR})

target_link_libraries(${PROJECT_NAME} -lpthread -lboost_filesystem)

##########################################################################################################################
#Copy data to build folder
add_custom_command(TARGET LRUCache POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DATA_DIR}/* ${BUILD_DIR})



