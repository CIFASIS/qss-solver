target_sources(
    mmoc-library
    PRIVATE
        jacobian_matrices.cpp
        model_dependencies.cpp
        sb_dependencies.cpp
)

add_subdirectory(builders)
add_subdirectory(sbg_graph)

