cmake_minimum_required(VERSION 2.8.4)
project(PnaGeo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES ngeo.cpp)
add_library(pnageo SHARED ${SOURCE_FILES} ngeo.h)
#add_executable(image_processor ${SOURCE_FILES} ngeo.h)

set_target_properties(pnageo PROPERTIES PUBLIC_HEADER "ngeo.h")

install (TARGETS pnageo
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin
         PUBLIC_HEADER DESTINATION include)