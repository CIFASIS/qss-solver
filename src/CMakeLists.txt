cmake_minimum_required(VERSION 3.10)

project(QSSSolver)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(mmoc)
add_subdirectory(engine)
add_subdirectory(gui)
add_subdirectory(interfaces/sbml)
add_subdirectory(usr)

# Optionally add tests and documentation targets here if needed
