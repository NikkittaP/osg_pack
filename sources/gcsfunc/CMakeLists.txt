cmake_minimum_required(VERSION 2.8.4)
project(GCSfunc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES gcsfunc.cpp)
add_library(gcsfunc SHARED ${SOURCE_FILES} gcsfunc.h)
#add_executable(image_processor ${SOURCE_FILES} gcsfunc.h)

set_target_properties(gcsfunc PROPERTIES PUBLIC_HEADER "gcsfunc.h")

install (TARGETS gcsfunc
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin
         PUBLIC_HEADER DESTINATION include)