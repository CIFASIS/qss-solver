cmake_minimum_required(VERSION 3.16)

project(MMOC-User-Libs)



# # Output directories
# set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
# set(LIB_DIR ${BUILD_DIR}/lib)

# # Create output directories
# file(MAKE_DIRECTORY
#       ${BUILD_DIR}
#       ${LIB_DIR}      
# )

# Libraries
add_library(mmo-math STATIC)
add_library(mmo-file STATIC)

target_include_directories(mmo-math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${QSS_SOLVER_ROOT})
target_include_directories(mmo-file PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${QSS_SOLVER_ROOT})

# Source files.
add_subdirectory(src)

# install
install(
    TARGETS mmo-file mmo-math
    RUNTIME DESTINATION lib
    COMPONENT mmo-libraries
    LIBRARY
        DESTINATION lib
        PERMISSIONS
            OWNER_READ
            OWNER_WRITE
            OWNER_EXECUTE
            GROUP_READ
            GROUP_EXECUTE
            WORLD_READ
            WORLD_EXECUTE
    COMPONENT mmo-libraries
)
