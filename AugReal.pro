TEMPLATE = app
CONFIG += console c++11
#CONFIG -= app_bundle
#CONFIG -= qt
#CONFIG += staticlib
QT += 3dcore 3drender 3dinput 3dlogic 3dquickextras qml quick 3dquick 3danimation
QT += multimedia multimediawidgets widgets

SOURCES += main.cpp \
    ARFrame.cpp \
    AREngine.cpp \
    ARFrameProcessor.cpp \
    ARViewer.cpp \
    ARPlainViewer.cpp \
    MySurface.cpp

win32 | win64{
    INCLUDEPATH += "D:/opencv-3.2.0MinGW/include/"
    LIBS += "D:/opencv-3.2.0MinGW/x86/mingw/bin/libopencv*.dll"
}

unix{
    INCLUDEPATH += $$PWD/../../../../usr/local/include

    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_calib3d
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_core
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_features2d
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_flann
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_highgui
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_imgcodecs
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_imgproc
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_ml
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_objdetect
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_photo
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_shape
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_stitching
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_superres
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_videoio
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_video
    LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_videostab
    LIBS += -L$$PWD/../../../../lib64 -lpthread # For multythreading
}

HEADERS += \
    ARFrame.h \
    AREngine.h \
    ARFrameProcessor.h \
    ARViewer.h \
    ARPlainViewer.h \
    MySurface.h

DISTFILES += \
    main.qml \
    Scene.qml
