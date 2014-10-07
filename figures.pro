QT       += core gui widgets

TARGET = cube
TEMPLATE = app

SOURCES += main.cpp \
    LoaderSaver.cpp \
    mainwindow.cpp \
    StringReader.cpp

    qtHaveModule(opengl) {
    QT += opengl

    SOURCES += main.cpp \
    LoaderSaver.cpp \
    mainwindow.cpp \
    StringReader.cpp\
    mainwidget.cpp \
    geometryengine.cpp \
    Matrix3D.cpp

    HEADERS += \
    mainwidget.h \
    geometryengine.h \
    Matrix3D.h \
    LoaderSaver.h \
    mainwindow.h \
    StringReader.h \
    MatrixData.h

    FORMS += \
    mainwindow.ui

    RESOURCES += \
    shaders.qrc
}






