# CMake glue for log.c (this repository is source-only upstream).
#
# Builds the library as a static archive and exports src/ as an include
# directory, so a consumer only has to add_subdirectory() this folder and link
# the `log` target. Nothing here is Pico specific; the pico_stdlib link is
# guarded so the file still works in a plain CMake build.
cmake_minimum_required(VERSION 3.13)

project(log VERSION 0.2.0 LANGUAGES C)

add_library(log STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/log.c
        )

target_include_directories(log PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )

# log.c only needs the C library (stdio, time, string). In a Raspberry Pi Pico
# SDK project pico_stdlib is what provides the wrapped stdio, so link it when it
# exists.
if(TARGET pico_stdlib)
    target_link_libraries(log PUBLIC pico_stdlib)
endif()

# Output options (LOG_USE_COLOR, LOG_STDOUT_INCLUDE_FILE_AND_LINE) are
# deliberately not set here: an ANSI-coloured, absolute-path prefix is noise on
# a 115200 baud serial console. The consuming project decides, and rack-fanctl
# installs its own output callback instead of the built-in stderr one.
