#include "test.h"
#include "ui_test.h"
#include <QQuickView>
#include <QQmlContext>
#include <QWidget>

test::test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::test)
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

test::~test()
{
    delete ui;
}
