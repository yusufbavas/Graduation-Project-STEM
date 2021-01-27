QT       += core gui widgets serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codeeditor.cpp \
    highlighter.cpp \
    lex.yy.c \
    lexer.c \
    main.cpp \
    mainwindow.cpp \
    serialconnection.cpp \
    stem_lang.tab.c \
    textEditor.cpp

HEADERS += \
    codeeditor.h \
    highlighter.h \
    lexer.h \
    mainwindow.h \
    serialconnection.h \
    stem_lang.tab.h \
    textEditor.h

FORMS += \
    mainwindow.ui

OTHER_FILES +=  \
    stem_lang.l \
    stem_lang.y

TRANSLATIONS += \
    Bitirme_QT_tr_TR.ts
RESOURCES     = application.qrc \
    application.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
