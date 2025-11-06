cmake_minimum_required(VERSION 3.20)
project(AlgorithmRitual)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/sfml@2")

# 👇 这行是关键，告诉编译器去 include/ 文件夹里找头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(AlgorithmRitual ${SRC_FILES})

target_link_libraries(AlgorithmRitual
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    pthread
)