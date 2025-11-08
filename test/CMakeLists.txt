cmake_minimum_required(VERSION 3.29)

#Скачиваем библиотеку Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        #https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
        # Т.к. установлен более свежий cmake, отсутствует совместимость с минимальной версией, поэтому использую свежий googletest
        https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_disable_pthreads ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Создаем исполняемый файл с тестами
add_executable(Search_engine_tests
        test.cpp
        InvertedIndex-test.cpp
        SearchServer-test.cpp
)
# Добавляем исходные файлы проекта в тесты
target_sources(Search_engine_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/src/ConverterJSON.cpp
        ${CMAKE_SOURCE_DIR}/src/InvertedIndex.cpp
        ${CMAKE_SOURCE_DIR}/src/SearchServer.cpp
)
# Подключаем библиотеки
target_link_libraries(Search_engine_tests PRIVATE
        gtest_main
        nlohmann_json::nlohmann_json
)
# Подключаем заголовки
target_include_directories(Search_engine_tests PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}/include
)

#Сообщаем CTest, какие тесты есть в нашем проекте
include(GoogleTest)
gtest_discover_tests(Search_engine_tests)
