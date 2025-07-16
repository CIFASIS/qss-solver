target_sources(
    mmoc-library
    PRIVATE
        ast_util.cpp
        compile_flags.cpp
        debug.cpp
        error.cpp
        ginac_interface.cpp
        graph.cpp
        graph_profile.cpp
        logger.cpp
        process_statement.cpp
        symbol_table.cpp
        type.cpp
        util.cpp
)

add_subdirectory(visitors)