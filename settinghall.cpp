#include <QQuickView>
#include <QQmlContext>
#include <QWidget>
#include "settinghall.h"
#include "ui_settinghall.h"

settingHall::settingHall(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingHall)
{
    ui->setupUi(this);
    QQuickView *view = new QQuickView();
    QQmlContext *context = view->rootContext();
    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setMinimumSize(500,400);
    container->setMaximumSize(500,400);
    context->setContextProperty("window", this);
    container->setFocusPolicy(Qt::TabFocus);
    view->setSource(QUrl("qrc:/qml/hall.qml"));
}

settingHall::~settingHall()
{
    delete ui;
}
