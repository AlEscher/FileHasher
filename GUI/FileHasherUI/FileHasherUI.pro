QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$_PRO_FILE_PWD_

SOURCES += \
    FileHasherDelegate.cpp \
    main.cpp \
    filehasher.cpp

SOURCES += \
    C:/Dev/Cpp/FileHasher/Utility/FileUtility.cpp

HEADERS += \
    filehasher.h \
    filehasher_delegate.h

FORMS += \
    filehasher.ui

TRANSLATIONS += \
    FileHasherUI_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
