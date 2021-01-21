cmake_minimum_required(VERSION 3.16)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set directories


##########################################################################################################################

project(LRUCacheTest)
add_executable(LRUCacheTest)

target_sources(LRUCacheTest PRIVATE ./main.cpp ./cache_test.cpp ./utility_test.cpp ./reader_writer_test.cpp ../src/reader.cpp ../src/writer.cpp)

target_include_directories(LRUCacheTest PRIVATE ${INC_DIR})

target_link_libraries(${PROJECT_NAME} -lboost_filesystem -lgtest -lgtest_main -lpthread)

##########################################################################################################################

install(TARGETS LRUCacheTest RUNTIME DESTINATION ${BUILD_DIR})

add_custom_command(TARGET LRUCacheTest POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy LRUCacheTest ${BUILD_DIR})