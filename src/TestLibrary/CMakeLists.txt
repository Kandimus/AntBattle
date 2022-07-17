project(TestLibrary LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCE "src/*" "include/*"
#    "TestLibrary.def"
)

add_library(${PROJECT_NAME} MODULE "")

target_include_directories(${PROJECT_NAME} PUBLIC "include" "src" "../Interface")

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE})

target_compile_definitions(${PROJECT_NAME} PRIVATE TESTLIBRARY_PUBLIC_DLL)
target_compile_definitions(${PROJECT_NAME} PRIVATE CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=1)
target_compile_definitions(${PROJECT_NAME} PRIVATE BUILD_SHARED_LIBS=1)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

#target_link_libraries(${PROJECT_NAME} PUBLIC Core Ant IBattleLogProvider)
