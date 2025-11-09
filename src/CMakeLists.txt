cmake_minimum_required(VERSION 3.28)
set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

configure_file(../config_json/config.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
configure_file(../config_json/requests.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(${PROJECT_NAME}
    main.cpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${MY_INCLUDE_DIR}
)
