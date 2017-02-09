cmake_minimum_required(VERSION 3.6)
project(emp_framework)

find_package(Boost 1.63.0 COMPONENTS chrono REQUIRED)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 14)

include_directories(. ${Boost_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp http/router.cpp http/router.h http/request.cpp http/request.h http/response.cpp http/response.h 3d-party/route.cc 3d-party/route.h middleware/render.cpp middleware/render.h validation/validation_result.cpp validation/validation_result.h json/json_fast_writer.cpp json/json_fast_writer.h snapshot_db/writer.cpp snapshot_db/writer.h test_model.h)
add_executable(emp_framework ${SOURCE_FILES} )