cmake_minimum_required(VERSION 3.20)
project(AlgorithmRitual)
set(CMAKE_CXX_STANDARD 17)

# ✅ SFML 3 新语法
find_package(SFML 3 REQUIRED)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(AlgorithmRitual ${SRC_FILES})

# ✅ SFML 3 统一成一个目标：sfml
target_link_libraries(AlgorithmRitual sfml)