QT       += core gui network concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatList/chatlist.cpp \
    chatList/chatlistitem.cpp \
    chatMsgList/chatmsgitembase.cpp \
    chatMsgList/chattingrecordslist.cpp \
    chatPage/chatpage.cpp \
    chatmainwindow.cpp \
    components/addfrienddialog.cpp \
    components/buttonlabel.cpp \
    components/roundimage.cpp \
    friendList/contactfriendlist.cpp \
    friendList/friendapplylist.cpp \
    friendList/friendlistitem.cpp \
    global.cpp \
    httpmgr.cpp \
    main.cpp \
    mainwindow.cpp \
    msgBubble/bubbleFrame.cpp \
    msgBubble/pictureBubble.cpp \
    msgBubble/textBubble.cpp \
    qctool.cpp \
    sendmsgtextedit.cpp \
    tcpmgr.cpp \
    timerbutton.cpp \
    userdata.cpp \
    userloginwindow.cpp \
    usermgr.cpp \
    userregisterwindow.cpp

HEADERS += \
    chatList/chatlist.h \
    chatList/chatlistitem.h \
    chatMsgList/chatmsgitembase.h \
    chatMsgList/chattingrecordslist.h \
    chatPage/chatpage.h \
    chatmainwindow.h \
    components/addfrienddialog.h \
    components/buttonlabel.h \
    components/roundimage.h \
    friendList/contactfriendlist.h \
    friendList/friendapplylist.h \
    friendList/friendlistitem.h \
    global.h \
    httpmgr.h \
    mainwindow.h \
    msgBubble/BubbleFrame.h \
    msgBubble/TextBubble.h \
    msgBubble/pictureBubble.h \
    qctool.h \
    sendmsgtextedit.h \
    singleton.h \
    tcpmgr.h \
    timerbutton.h \
    userdata.h \
    userloginwindow.h \
    usermgr.h \
    userregisterwindow.h

FORMS += \
    chatList/chatlistitem.ui \
    chatPage/chatpage.ui \
    chatmainwindow.ui \
    components/addfrienddialog.ui \
    friendList/contactfriendlist.ui \
    mainwindow.ui \
    userloginwindow.ui \
    userregisterwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.ini

RESOURCES += \
    res.qrc

INCLUDEPATH += $$PWD/framelesscore

include ($$PWD/framelesscore/framelesscore.pri)

# 根据构建配置自动选择目标目录
CONFIG(debug, debug|release) {
    targetDir = $$OUT_PWD/Debug
} else {
    targetDir = $$OUT_PWD/Release
}

# 在构建时将 config.ini 文件拷贝到目标目录
win32 {
    CONFIG_FILE = $$PWD/config.ini
    CONFIG_FILE = $$replace(CONFIG_FILE, /, \\)
    QMAKE_POST_LINK += copy /Y \"$$CONFIG_FILE\" \"$$targetDir\"
}
