QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += \
    c++17 \
    CONFIG+= static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$_PRO_FILE_PWD_
INCLUDEPATH += "Style/framelesswindow"

SOURCES += \
    Controller.cpp \
    FileHasherDelegate.cpp \
    exportdialog.cpp \
    main.cpp \
    filehasher.cpp \
    ../../Utility/FileUtility.cpp \
    ../../Utility/BitwiseUtility.cpp \
    ../../Hashing/SHA256Hash.cpp \
    ../../Hashing/SHA512Hash.cpp \
    ../../Hashing/MD5Hash.cpp \
    ../../Hashing/SHA1Hash.cpp \
    ../../Hashing/HashingAlgorithm.cpp \
    Style/framelesswindow/framelesswindow.cpp \
    Style/DarkStyle.cpp \
    Style/framelesswindow/windowdragger.cpp

HEADERS += \
    Controller.h \
    FileHasherDelegate.h \
    exportdialog.h \
    filehasher.h \
    Style/DarkStyle.h \
    Style/framelesswindow/framelesswindow.h \
    Style/framelesswindow/windowdragger.h

FORMS += \
    exportdialog.ui \
    filehasher.ui \
    Style/framelesswindow/framelesswindow.ui

TRANSLATIONS += \
    FileHasherUI_en_US.ts

RESOURCES += \
    Style/framelesswindow.qrc \
    Style/darkstyle.qrc \
    Resources/resources.qrc

RC_ICONS = Resources/appico.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
