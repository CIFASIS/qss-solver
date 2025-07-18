add_custom_target(
    generate_parsers
    DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/mocc_parser.cpp
    DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/mocc_scanner.cpp
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/mocc_scanner.cpp
                   COMMAND ${CMAKE_COMMAND} -E echo "Generate scanner files."
                   COMMAND flex++ -d 
                   -o${CMAKE_CURRENT_SOURCE_DIR}/mocc_scanner.cpp 
                   ${CMAKE_CURRENT_SOURCE_DIR}/mocc.lex
)

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/mocc_parser.cpp
                    COMMAND ${CMAKE_COMMAND} -E echo "Generate parser files."
                    COMMAND bison++ -t -d -v 
                    -h${CMAKE_CURRENT_SOURCE_DIR}/mocc_parser.h 
                    -o${CMAKE_CURRENT_SOURCE_DIR}/mocc_parser.cpp 
                    ${CMAKE_CURRENT_SOURCE_DIR}/mocc.y
 )

set_source_files_properties(mocc_parser.cpp PROPERTIES GENERATED TRUE)
set_source_files_properties(mocc_scanner.cpp PROPERTIES GENERATED TRUE)

target_sources(
    mmoc-library
    PRIVATE
        parse.cpp
)

add_dependencies(mmoc-library generate_parsers)
