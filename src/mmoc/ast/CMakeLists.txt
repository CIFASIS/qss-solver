target_sources(
    mmoc-library
    PRIVATE
        ast_builder.cpp
        ast_node.cpp
        class.cpp
        composition.cpp
        element.cpp
        equation.cpp
        expression.cpp
        imports.cpp
        modification.cpp
        statement.cpp
        stored_definition.cpp
)

add_subdirectory(parser)