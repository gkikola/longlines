cmake_minimum_required (VERSION 2.8)
project (longlines)

set (LONGLINES_VERSION_MAJOR 1)
set (LONGLINES_VERSION_MINOR 0)
set (
  LONGLINES_VERSION
  "${LONGLINES_VERSION_MAJOR}.${LONGLINES_VERSION_MINOR}"
  )

configure_file (
  "version.h.in"
  "${PROJECT_BINARY_DIR}/version.h"
  )

include_directories("${PROJECT_BINARY_DIR}")

add_executable(longlines src/main.cpp)
