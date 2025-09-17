# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\mainwindow_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\mainwindow_autogen.dir\\ParseCache.txt"
  "mainwindow_autogen"
  )
endif()
