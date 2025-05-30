cmake_minimum_required(VERSION 3.10)
project(QSSSolver)

# Set the C and C++ compilers
# Set the C and C++ compilers
# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_CXX_COMPILER g++)

# Set the build type
set(CMAKE_BUILD_TYPE Debug)

# Add subdirectories
add_subdirectory(src/engine)
add_subdirectory(src/mmoc)
add_subdirectory(src/usr)
add_subdirectory(src/gui)

# Custom commands for building documentation
add_custom_target(generate_doc ALL
    COMMAND doxygen ${CMAKE_SOURCE_DIR}/src/gui/QSSSolverGUI.doxyfile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/gui
)
