QT += qml quick

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include('Third party/qnanopainter/libqnanopainter/include.pri')

APP_VERSION = ""
TOTAL_COMMIT_COUNT = ""
COMMIT_SHORT_HASH = ""

# If target OS has git installed.
system("git --version") {
    APP_VERSION = $$system('git describe --tags --abbrev=0')
    TOTAL_COMMIT_COUNT = $$system('git rev-list --count main')
    COMMIT_SHORT_HASH = $$system('git rev-parse --short HEAD')
} else {
    warning('git not found, please set app version manually in \".pro\" file..')
}

DEFINES += APP_VERSION='\\"$$APP_VERSION\\"'
DEFINES += MAIN_COMMIT_NUMBER='\\"$$MAIN_COMMIT_NUMBER\\"'
DEFINES += COMMIT_SHORT_HASH='\\"$$COMMIT_SHORT_HASH\\"'

SOURCES += \
    knightcanvas.cpp \
    main.cpp \
    maskwindow.cpp \
    nanopainter.cpp \
    painthelper.cpp \
    shapes/apoint.cpp \
    shapes/ellipseshape.cpp \
    shapes/lineshape.cpp \
    shapes/pathshape.cpp \
    shapes/rectshape.cpp \
    shapes/shape.cpp \
    updater.cpp \
    utils.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    knightcanvas.h \
    maskwindow.h \
    nanopainter.h \
    nanopen.h \
    painthelper.h \
    shapes/apoint.h \
    shapes/ellipseshape.h \
    shapes/lineshape.h \
    shapes/pathshape.h \
    shapes/rectshape.h \
    shapes/shape.h \
    shapes/shapes.h \
    updater.h \
    utils.h
