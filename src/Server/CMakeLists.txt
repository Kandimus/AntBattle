project(AntServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE "src/*" "include/*")

add_executable(${PROJECT_NAME} "")

target_include_directories(${PROJECT_NAME} PUBLIC "include" "src")

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE})

target_link_libraries(${PROJECT_NAME} PUBLIC Core Ant Player Log simpleargs)

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES "${PROJECT_SOURCE_DIR}/test.conf" DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES "${PROJECT_SOURCE_DIR}/stone.template" DESTINATION ${CMAKE_INSTALL_PREFIX})
