TEMPLATES = app

CONFIG += release 
UI_DIR=./ui

message(Qt version: $$QT_VERSION)
contains(QT_VERSION,5)
{
	message(Adding Widgets headers)
	QT += widgets
}

# Sources.
SOURCES += 	main.cpp  \
			mmomegui.cpp \
			modeleditor.cpp \
			mmohighlight.cpp \
			treemodel.cpp \
			editor.cpp \
			codeeditor.cpp \
			modelinfo.cpp \
			runform.cpp \
			utils.cpp \
			settings.cpp \
			comboboxdelegate.cpp 

# Headers.
HEADERS += 	mmomegui.hpp \
			modeleditor.hpp \
			mmohighlight.hpp \
			treemodel.hpp \
			editor.hpp \
			codeeditor.hpp \
			modelinfo.hpp \
			runform.hpp \
			utils.hpp \
			settings.hpp \
			comboboxdelegate.hpp 
			
# Forms.
FORMS +=	$$UI_DIR/mmomegui.ui \
			$$UI_DIR/modeleditor.ui\
			$$UI_DIR/run.ui\
    	$$UI_DIR/settings.ui

RESOURCES += mmome.qrc

INCLUDEPATH += $$PWD/3rd-party/qtermwidget-1-0.14.1/usr/include
LIBS += -L $$PWD/3rd-party/qtermwidget-1-0.14.1/usr/lib -lqtermwidget5

OBJECTS_DIR = usr/obj
MOC_DIR = usr/moc
Release:DESTDIR = usr
TARGET= usr/bin/qss-solver
