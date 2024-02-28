cmake_minimum_required(VERSION 3.25)
project(new_infantry)

#set(CMAKE_C_COMPILER clang++)
#set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 17)
set(IMTUI_PATH ../imtui/build/dist)

include_directories(${IMTUI_PATH}/include ${IMTUI_PATH}/include/imgui-for-imtui)
include_directories(./include)

add_compile_options(-Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression)
add_compile_options(-Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments)

file(GLOB_RECURSE SOURCE src/main.cc ./src/*.cc)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_directories(${PROJECT_NAME} PUBLIC ${IMTUI_PATH}/lib)
target_link_libraries(${PROJECT_NAME} libimtui.a)
target_link_libraries(${PROJECT_NAME} libimtui-ncurses.a)
target_link_libraries(${PROJECT_NAME} libimgui-for-imtui.a)
target_link_libraries(${PROJECT_NAME} ncurses)
