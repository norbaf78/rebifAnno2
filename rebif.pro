QT += core
QT -= gui

CONFIG += c++11

TARGET = rebif
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += "C:\\opencv2.4.6\\build\\include"
    CONFIG(debug,debug|release) {
       LIBS += -L"C:\\opencv2.4.6\\build\\x64\\vc11\\lib" \
            -lopencv_calib3d246d \
            -lopencv_features2d246d \
            -lopencv_imgproc246d \
            -lopencv_highgui246d \
            -lopencv_core246d \
            -lopencv_video246d \
            -lopencv_objdetect246d \
            -lopencv_ml246d
    }

    CONFIG(release,debug|release) {
        DEFINES += QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT
        LIBS += -L"C:\\opencv2.4.6\\build\\x64\\vc11\\lib" \
            -lopencv_calib3d246 \
            -lopencv_features2d246 \
            -lopencv_imgproc246 \
            -lopencv_highgui246 \
            -lopencv_core246 \
            -lopencv_video246 \
            -lopencv_objdetect246 \
            -lopencv_ml246
    }
