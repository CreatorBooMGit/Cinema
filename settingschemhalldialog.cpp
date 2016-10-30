#include <QtQml/QQmlEngine>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QQuickView>
#include <QWidget>
#include "settingschemhalldialog.h"
#include "ui_settingschemhalldialog.h"

settingSchemHallDialog::settingSchemHallDialog(QWidget *parent) :
    QDialog(parent),
    hall(new Ui::settingSchemHallDialog)
{
    hall->setupUi(this);
    QQuickView *view = new QQuickView();
    QQmlContext *context = view->rootContext();
    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setMinimumSize(500,400);
    container->setMaximumSize(500,400);
    context->setContextProperty("window", this);
    container->setFocusPolicy(Qt::TabFocus);
    view->setSource(QUrl("qrc:/qml/hall.qml"));
    hall->widget = container;
    container->setEnabled(true);

    connect(hall->buttonCancel, SIGNAL(clicked(bool)), SLOT(buttonCancelClicked()));
}

settingSchemHallDialog::~settingSchemHallDialog()
{
    delete hall;
}


void settingSchemHallDialog::buttonCancelClicked()
{
    close();
}
