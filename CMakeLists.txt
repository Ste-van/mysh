cmake_minimum_required(VERSION 3.10)

project(mysh
    VERSION 1.0.0
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(mysh
    src/main.c
    src/shell.c
)

set_target_properties(mysh PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_include_directories(mysh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(mysh PRIVATE _POSIX_C_SOURCE=200809L)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mysh PRIVATE
        -Wall
        -Wextra
        -pedantic
#       -Werror
    )
endif()
