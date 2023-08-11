# These tests can use the Catch2-provided main

set(PROTO_C  "${CMAKE_CURRENT_SOURCE_DIR}/protos/CPP/C")
set(PROTO_H  "${CMAKE_CURRENT_SOURCE_DIR}/protos/CPP/H")


include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS "protos/simple.proto")

add_library(protos_lib ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protos_lib protobuf::protobuf ProtoReflection)

add_executable(tests "TypeWrapper_Test.cpp""Reflection_Test.cpp")
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain protos_lib)