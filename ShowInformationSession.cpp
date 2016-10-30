#include "ShowInformationSession.h"
#include "ui_ShowInformationSession.h"
#include "HallScheme/SettingHallQml.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlProperty>
#include <QDebug>

ShowInformationSession::ShowInformationSession(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::ShowInformationSession)
{
    ui->setupUi(this);

    hallCore = new SettingHallQML(this);
    QQuickView *view = new QQuickView;
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext *context = view->rootContext();
    QWidget *container = QWidget::createWindowContainer(view, NULL);
//    ui->verticalLayout_2->addWidget(container);
    context->setContextProperty("hallCore", hallCore);
    view->setSource(QUrl("qrc:/qml/Scheme.qml"));

    ui->horizontalLayout->addWidget(container);

    hallCore->setSizeHall(15, 10);
}

ShowInformationSession::~ShowInformationSession()
{
    delete ui;
}
