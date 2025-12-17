cmake_minimum_required(VERSION 3.10)

project(lab)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы
set(SOURCES
    src/npc.cpp
    src/knight.cpp
    src/dragon.cpp
    src/pegasus.cpp
)

# Включаем директорию с заголовками
include_directories(include)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} 
    main.cpp 
    ${SOURCES}
)

