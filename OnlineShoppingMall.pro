QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    custom_query.cpp \
    delete_query.cpp \
    delete_s1.cpp \
    delete_s2.cpp \
    delete_s3.cpp \
    delete_s4.cpp \
    func.cpp \
    global.cpp \
    insert_query.cpp \
    insert_s1.cpp \
    insert_s2.cpp \
    insert_s3.cpp \
    insert_s4.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    schema_check.cpp \
    select_query.cpp \
    select_s1.cpp \
    select_s2.cpp \
    select_s3.cpp \
    select_s4.cpp \
    select_s5.cpp \
    select_s6.cpp \
    server_connect.cpp \
    update_query.cpp \
    update_s1.cpp \
    update_s2.cpp \
    update_s3.cpp \
    update_s4.cpp

HEADERS += \
    custom_query.h \
    delete_query.h \
    delete_s1.h \
    delete_s2.h \
    delete_s3.h \
    delete_s4.h \
    func.h \
    global.h \
    insert_query.h \
    insert_s1.h \
    insert_s2.h \
    insert_s3.h \
    insert_s4.h \
    loginwindow.h \
    mainwindow.h \
    schema_check.h \
    select_query.h \
    select_s1.h \
    select_s2.h \
    select_s3.h \
    select_s4.h \
    select_s5.h \
    select_s6.h \
    server_connect.h \
    update_query.h \
    update_s1.h \
    update_s2.h \
    update_s3.h \
    update_s4.h

FORMS += \
    custom_query.ui \
    delete_query.ui \
    delete_s1.ui \
    delete_s2.ui \
    delete_s3.ui \
    delete_s4.ui \
    insert_query.ui \
    insert_s1.ui \
    insert_s2.ui \
    insert_s3.ui \
    insert_s4.ui \
    loginwindow.ui \
    mainwindow.ui \
    schema_check.ui \
    select_query.ui \
    select_s1.ui \
    select_s2.ui \
    select_s3.ui \
    select_s4.ui \
    select_s5.ui \
    select_s6.ui \
    update_query.ui \
    update_s1.ui \
    update_s2.ui \
    update_s3.ui \
    update_s4.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
