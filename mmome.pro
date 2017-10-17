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
HEADERS += 	mmomegui.h \
			modeleditor.h \
			mmohighlight.h \
			treemodel.h \
			editor.h \
			codeeditor.h \
			modelinfo.h \
			runform.h \
			utils.h \
			settings.h \
			comboboxdelegate.h 
			
# Forms.
FORMS +=	$$UI_DIR/mmomegui.ui \
			$$UI_DIR/modeleditor.ui\
			$$UI_DIR/run.ui\
    	$$UI_DIR/settings.ui

RESOURCES += mmome.qrc

OBJECTS_DIR = usr/obj
MOC_DIR = usr/moc
Release:DESTDIR = usr
TARGET= usr/bin/qss-solver
