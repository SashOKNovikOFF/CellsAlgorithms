# Проверка версии CMake
cmake_minimum_required(VERSION 3.5)

# Вывод отладочной информации
SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_COLOR_MAKEFILE ON)

# Использовать стандарт C++11
set(CMAKE_CXX_STANDARD 11)

set(SFML_LIBRARIES
        sfml-system
        sfml-window
        sfml-graphics
        sfml-network
        sfml-audio)

# Подключить OpenMP
message(STATUS "Checking OpenMP")
find_package(OpenMP)
IF(OPENMP_FOUND)
    message("Found OpenMP! ^_^")
    # Добавить флаги для OpenMP
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE()
    message("Missed OpenMP! x_x")
ENDIF()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/output)

# Собрать приложение
set(APP_SOURCE
        main.cpp
        Model.cpp Road1D.cpp Road1D.hpp)

set(APP_NAME SFML_Project)
add_executable(${APP_NAME} ${APP_SOURCE})

target_link_libraries(${APP_NAME}
    ${SFML_LIBRARIES})
