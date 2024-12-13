QT += quick quickcontrols2 core sql gui svg xml

CONFIG += c++17

TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/categoryinfo.cpp \
    src/listsmodel.cpp \
        main.cpp \
    src/noteinfo.cpp \
    src/notesmodel.cpp \
    src/resourcesmanager.cpp \
    src/richtexthandler.cpp \
        src/tasklistmodel.cpp \
        src/Target.cpp \
        src/TargetList.cpp \
        src/commontarget.cpp \
        src/iterabletask.cpp \
        #src/schedules.cpp \
        src/scheduletarget.cpp \
        src/targetsfactory.cpp\
        src/executor.cpp \
        src/connectionmanager.cpp \
        src/iterabletaskmodel.cpp \
        src/taskstimer.cpp \


HEADERS += \
    include/Target.h \
    include/TargetList.h \
    include/categoryinfo.h \
    include/iterabletask.h \
    #include/schedules.h \
    include/noteinfo.h \
    include/notesmodel.h \
    include/resourcesmanager.h \
    include/richtexthandler.h \
    include/targets/commontarget.h \
    include/targets/scheduletarget.h \
    include/listsmodel.h \
    include/tasklistmodel.h \
    include/executor.h \
    include/connectionmanager.h \
    include/iterabletaskmodel.h \
    include/taskstimer.h \

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $$PWD/qml
QML2_IMPORT_PATH += $$PWD/qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/include

