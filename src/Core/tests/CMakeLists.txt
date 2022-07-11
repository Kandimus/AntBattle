project(test_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE "*.h" "*.cpp")

add_executable(${PROJECT_NAME} "")

target_include_directories(${PROJECT_NAME}  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE})

target_link_libraries(${PROJECT_NAME} PUBLIC Core)
