cmake_minimum_required(VERSION 3.16)

project(QSS-Solver-GUI)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

qt5_add_resources(RESOURCES_RCC mmome.qrc)

add_executable(qss-solver)

target_sources(
    qss-solver
    PRIVATE
    codeeditor.cpp
    comboboxdelegate.cpp
    editor.cpp
    main.cpp
    mmohighlight.cpp
    mmomegui.cpp
    modeleditor.cpp
    modelinfo.cpp
    runform.cpp
    settings.cpp
    treemodel.cpp
    ui/mmomegui.ui
    ui/modeleditor.ui
    ui/run.ui
    ui/settings.ui
    utils.cpp
    ${RESOURCES_RCC}
)

target_include_directories(qss-solver PRIVATE 3rd-party/qtermwidget-1-0.14.1/usr/include)

target_include_directories(qss-solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

target_link_libraries(qss-solver PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

target_link_directories(qss-solver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/qtermwidget-1-0.14.1/usr/lib)

# Link the library
target_link_libraries(qss-solver PRIVATE qtermwidget5)

# Resources:
# set(mmome_resource_files
#     "images/ImportFile.png"
#     "images/cancel.png"
#     "images/clear.png"
#     "images/compile.png"
#     "images/copy.png"
#     "images/cut.png"
#     "images/debug.png"
#     "images/delete.png"
#     "images/document_save.png"
#     "images/editfile.png"
#     "images/folder.svg"
#     "images/gnuplot.png"
#     "images/integrator.svg"
#     "images/log.png"
#     "images/multiplier.svg"
#     "images/newFile.png"
#     "images/run.png"
#     "images/run_copy.png"
#     "images/save_all.png"
#     "images/saveas.png"
#     "images/settings.png"
# )

# qt5_add_resources(qss-solver "mmome"
#      PREFIX"/"
#      FILES
#        ${mmome_resource_files}
# )

install(TARGETS qss-solver
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
