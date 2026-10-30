cmake_minimum_required(VERSION 3.10)

project(mysh
    VERSION 1.0.0
    LANGUAGES C
)

add_executable(mysh
)

set_target_properties(mysh PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_include_directories(mysh PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(mysh PRIVATE
        -Wall
        -Wextra
        -pedantic
#       -Werror
    )
endif()
