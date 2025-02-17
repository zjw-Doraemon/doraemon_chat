QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bubbleframe.cpp \
    chat_dialog.cpp \
    chatitembase.cpp \
    chatpage.cpp \
    chatuserlist.cpp \
    chatuserwid.cpp \
    chatview.cpp \
    clickedlabel.cpp \
    custom_edit.cpp \
    forget_password.cpp \
    global.cpp \
    httpmgr.cpp \
    listitembase.cpp \
    loaddlg.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    register_dialog.cpp \
    tcpmgr.cpp \
    test_widget.cpp \
    timerbtn.cpp \
    usermgr.cpp

HEADERS += \
    bubbleframe.h \
    chat_dialog.h \
    chatitembase.h \
    chatpage.h \
    chatuserlist.h \
    chatuserwid.h \
    chatview.h \
    clickedlabel.h \
    custom_edit.h \
    forget_password.h \
    global.h \
    httpmgr.h \
    listitembase.h \
    loaddlg.h \
    logindialog.h \
    mainwindow.h \
    register_dialog.h \
    singleton.h \
    tcpmgr.h \
    test_widget.h \
    timerbtn.h \
    usermgr.h

FORMS += \
    chat_dialog.ui \
    chatpage.ui \
    chatuserwid.ui \
    forget_password.ui \
    loaddlg.ui \
    logindialog.ui \
    mainwindow.ui \
    register_dialog.ui \
    test_widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    config.ini


win32:CONFIG(debug, debug | release)

{
    CONFIG(debug, debug|release) {
        # 调试模式
        BUILD_DIR = debug
    } else {
        # 发布模式
        BUILD_DIR = release
    }
    #指定要拷贝的文件目录为工程目录下release目录下的所有dll、lib文件，例如工程目录在D:\QT\Test
    #PWD就为D:/QT/Test，DllFile = D:/QT/Test/release/*.dll
    TargetConfig = $${PWD}/config.ini
    #将输入目录中的"/"替换为"\"
    TargetConfig = $$replace(TargetConfig, /, \\)
    #将输出目录中的"/"替换为"\"
    OutputDir =  $${OUT_PWD}/$${DESTDIR}/$$BUILD_DIR
    OutputDir = $$replace(OutputDir, /, \\)
    //执行copy命令
    QMAKE_POST_LINK += copy /Y \"$$TargetConfig\" \"$$OutputDir\"
}

