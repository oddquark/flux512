project(Flux-512)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
cmake_minimum_required(VERSION 2.8.12)
include_directories("src")
add_subdirectory("src/sha512")
find_package(Curses REQUIRED)
add_executable(flux-512 src/main.cpp
			src/utils.cpp
			src/encrypt.cpp
			src/decrypt.cpp
			src/sbox.cpp
			src/pbox.cpp)
target_link_libraries(flux-512 ${CURSES_LIBRARIES} sha512)
