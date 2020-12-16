cmake_minimum_required(VERSION 3.5)

project(libalang)

set(CMAKE_CXX_COMPILER clang++)

add_compile_options(-std=c++17 -stdlib=libc++ -Wall -Werror -O2 -g)

file(GLOB_RECURSE src src/alang/*.cpp)

add_executable(alang ${src} src/main.cpp)
target_include_directories(alang PUBLIC src/)
target_link_libraries(alang /usr/local/lib/liblgpp.a)
target_include_directories(alang PUBLIC /usr/local/include/)

file(GLOB inc src/alang/*.hpp)
install(FILES ${inc} DESTINATION include/alang)

install(FILES build/libalang.a DESTINATION lib)
install(FILES build/alang DESTINATION bin)
