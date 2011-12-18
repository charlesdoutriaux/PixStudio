#-------------------------------------------------
#
# Project created by Charles Doutriaux 2011-10-17
#
#-------------------------------------------------


macx {
QMAKE_CXXFLAGS += -arch x86_64
QMAKE_CFLAGS += -arch x86_64
}

RESOURCES = PixStudio.qrc

QMAKE_CXXFLAGS += -IInclude
QMAKE_CFLAGS += -IInclude
QT       += core gui

TARGET = PixStudio
TEMPLATE = app
          
SOURCES += Src/main.cpp \
	   Src/mainwindow.cpp \
           Src/preferences.cpp \
           Src/QPathListWidget.cpp \
           Src/files_parsing.cpp \
           Src/pix_entries.cpp \
           Src/QGallery.cpp \
           Src/QGalleryTab.cpp \
           Src/QExifImageHeader.cpp
           

HEADERS += \
    Include/mainwindow.h \
    Include/preferences.h \
    Include/QPathListWidget.h \
    Include/files_parsing.h \
    Include/pix_entries.h \
    Include/QGallery.h \
    Include/QGalleryTab.h \
    Include/QExifImageHeader.h




 message(Qt version: $$[QT_VERSION])
 message(Qt is installed in $$[QT_INSTALL_PREFIX])
 message(Qt resources can be found in the following locations:)
 message(Documentation: $$[QT_INSTALL_DOCS])
 message(Header files: $$[QT_INSTALL_HEADERS])
 message(Libraries: $$[QT_INSTALL_LIBS])
 message(Binary files (executables): $$[QT_INSTALL_BINS])
 message(Plugins: $$[QT_INSTALL_PLUGINS])
 message(Data files: $$[QT_INSTALL_DATA])
 message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
 message(Settings: $$[QT_INSTALL_SETTINGS])
 message(Examples: $$[QT_INSTALL_EXAMPLES])
 message(Demonstrations: $$[QT_INSTALL_DEMOS])


