QT       += core gui

# 为了可以使用QCamera类，导入的配置
QT       += multimedia
# 为了可以使用QCameraViewFinder，导入的配置
QT       += multimediawidgets
# 为了网络请求和应答
QT       += network

INCLUDEPATH += D:/QtTools/openssl/openssl-1.1.1g/include
# INCLUDEPATH += C:/OpenSSL-Win32/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    recognition.cpp \
    worker.cpp

HEADERS += \
    recognition.h \
    worker.h

FORMS += \
    recognition.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
