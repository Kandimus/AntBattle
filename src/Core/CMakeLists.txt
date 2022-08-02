project(Core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE "src/*" "include/*")

add_library(${PROJECT_NAME} STATIC "")

target_include_directories(${PROJECT_NAME} PUBLIC "include" "src")
target_include_directories(${PROJECT_NAME} PUBLIC "../json/include/nlohmann")
target_include_directories(${PROJECT_NAME} PUBLIC "../Interface/")

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE})

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json PUBLIC Log Player Ant BattleLogService FileProvider)

if(TESTS_UNIT)
    add_subdirectory(tests)
endif(TESTS_UNIT)
