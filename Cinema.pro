#-------------------------------------------------
#
# Project created by QtCreator 2016-03-14T12:58:12
#
#-------------------------------------------------

QT       += core gui sql qml quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cinema
TEMPLATE = app

QMAKE_CXXFLAGS += -std=gnu++14

include($$PWD/QtRptProject/QtRPT/QtRPT.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    settingdialog.cpp \
    settingschemhalldialog.cpp \
    settinghall.cpp \
    test.cpp \
    addfilmdialog.cpp \
    filmsdialog.cpp \
    editfilmdialog.cpp \
    addsessiondialog.cpp \
    editsessiondialog.cpp \
    lockdialog.cpp \
    authentificationuserdialog.cpp \
    editaccessdialog.cpp \
    addpostdialog.cpp \
    editpostdialog.cpp \
    employeesdialog.cpp \
    editemployeedialog.cpp \
    addemployeedialog.cpp \
    access.cpp \
    HallScheme/addsectordialog.cpp \
    HallScheme/EditSectorDialog.cpp \
    HallScheme/SectorsDialog.cpp \
    HallScheme/settinghallqml.cpp \
    ShowInformationSession.cpp \
    AddHallScheme.cpp \
    EditHallScheme.cpp \
    ShowSessionDialog.cpp

HEADERS  += mainwindow.h \
    settingdialog.h \
    usertype.h \
    settingschemhalldialog.h \
    settinghall.h \
    test.h \
    addfilmdialog.h \
    filmsdialog.h \
    editfilmdialog.h \
    addsessiondialog.h \
    editsessiondialog.h \
    lockdialog.h \
    authentificationuserdialog.h \
    editaccessdialog.h \
    addpostdialog.h \
    editpostdialog.h \
    employeesdialog.h \
    editemployeedialog.h \
    addemployeedialog.h \
    accessemployee.h \
    access.h \
    HallScheme/AddSectorDialog.h \
    HallScheme/EditSectorDialog.h \
    HallScheme/SectorsDialog.h \
    HallScheme/SettingHallQml.h \
    HallScheme/usertypes.h \
    ShowInformationSession.h \
    AddHallScheme.h \
    EditHallScheme.h \
    ShowSessionDialog.h

FORMS    += mainwindow.ui \
    settingdialog.ui \
    settingschemhalldialog.ui \
    settinghall.ui \
    test.ui \
    addfilmdialog.ui \
    filmsdialog.ui \
    editfilmdialog.ui \
    addsessiondialog.ui \
    editsessiondialog.ui \
    lockdialog.ui \
    authentificationuserdialog.ui \
    editaccessdialog.ui \
    addpostdialog.ui \
    editpostdialog.ui \
    employeesdialog.ui \
    editemployeedialog.ui \
    addemployeedialog.ui \
    HallScheme/AddSectorDialog.ui \
    HallScheme/EditSectorDialog.ui \
    HallScheme/SectorsDialog.ui \
    ShowInformationSession.ui \
    AddHallScheme.ui \
    EditHallScheme.ui \
    ShowSessionDialog.ui

RESOURCES += \
    icons.qrc \
    HallScheme/qml.qrc \
    reports.qrc

DISTFILES +=

TRANSLATIONS =  cinema_ru.ts \
                cinema_eng.ts \
                cinema_ukr.ts
