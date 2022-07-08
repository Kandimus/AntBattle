project(Log LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE "src/*" "include/*")

add_library(${PROJECT_NAME} "")

target_include_directories(${PROJECT_NAME} PUBLIC "include" "src")

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE})

#target_compile_definitions(${PROJECT_NAME} PRIVATE -DANT_BATTLE_LOG_PUBLIC_DLL)

