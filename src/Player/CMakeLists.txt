project(Player LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE "src/*" "include/*")

add_library(${PROJECT_NAME} STATIC "")

target_include_directories(${PROJECT_NAME} PUBLIC "include" "src")
target_include_directories(${PROJECT_NAME} PRIVATE "../Interface/")

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE})

target_link_libraries(${PROJECT_NAME} PRIVATE Log)
