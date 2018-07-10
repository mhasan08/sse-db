cmake_minimum_required(VERSION 3.5)
project(SSE)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c sse/sse.h crypto/encryption.c crypto/encryption.h crypto/decryption.c crypto/decryption.h)
add_executable(SSE ${SOURCE_FILES})

target_link_libraries(SSE sselib crypto m)

add_library(sselib SHARED IMPORTED)
set_target_properties(sselib PROPERTIES IMPORTED_LOCATION "./lib/libsse.so")
