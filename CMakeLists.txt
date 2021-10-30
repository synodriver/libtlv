cmake_minimum_required(VERSION 3.20)
project(libtlv C)

set(CMAKE_C_STANDARD 11)

include_directories(./src)
add_library(libtlv SHARED src/tlv.c src/util.c)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(libtlv PROPERTIES OUTPUT_NAME tlv)
    target_compile_definitions(libtlv PUBLIC "__WINDOWS__")
endif ()


add_executable(parser main.c)
target_link_libraries(parser libtlv)

message(${PROJECT_SOURCE_DIR})