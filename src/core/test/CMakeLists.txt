cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME sita_core_test)
message("==== ${PROJECT_NAME} ====")
project(${PROJECT_NAME})

sita_add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER sita/core)
target_link_libraries(${PROJECT_NAME} PUBLIC sita_core)