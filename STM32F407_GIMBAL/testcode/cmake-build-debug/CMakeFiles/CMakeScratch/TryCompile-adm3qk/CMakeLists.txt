cmake_minimum_required(VERSION 3.29.6.0)
project(CMAKE_TRY_COMPILE CXX)
set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES "")
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_DEFINITIONS}")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EXE_LINKER_FLAGS}")
include_directories(${INCLUDE_DIRECTORIES})
set(CMAKE_SUPPRESS_REGENERATION 1)
link_directories(${LINK_DIRECTORIES})
add_definitions([==[-v]==])
cmake_policy(SET CMP0065 NEW)
cmake_policy(SET CMP0083 NEW)
cmake_policy(SET CMP0155 NEW)
cmake_policy(SET CMP0157 OLD)
include("${CMAKE_ROOT}/Modules/Internal/HeaderpadWorkaround.cmake")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "D:/RM_Projects/STM32F407_GIMBAL/testcode/cmake-build-debug/CMakeFiles/CMakeScratch/TryCompile-adm3qk")
add_library(cmTC_5b0de STATIC)
target_sources(cmTC_5b0de PRIVATE
  "C:/Program Files/JetBrains/CLion 2024.2.1/bin/cmake/win/x64/share/cmake-3.29/Modules/CMakeCXXCompilerABI.cpp"
)
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/cmTC_5b0de_loc"
     CONTENT $<TARGET_FILE:cmTC_5b0de>)
target_link_libraries(cmTC_5b0de ${LINK_LIBRARIES})
