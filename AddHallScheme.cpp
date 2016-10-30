#include "AddHallScheme.h"
#include "ui_AddHallScheme.h"
#include "HallScheme/SettingHallQml.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlProperty>
#include <QDebug>
#include <QMessageBox>

AddHallScheme::AddHallScheme(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AddHallScheme)
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

    ui->containerQml->addWidget(container);
    hallCore->setSizeHall(0,0);
    ui->containerQml->setEnabled(false);
}

AddHallScheme::~AddHallScheme()
{
    delete ui;
}

void AddHallScheme::on_rowCountSpinBox_valueChanged(int arg1)
{
    if(arg1 > 0 && ui->columnCountSpinBox->value() < 0) return;

    ui->containerQml->setEnabled(true);
    hallCore->setSizeHall(arg1, ui->columnCountSpinBox->value());
}

void AddHallScheme::on_columnCountSpinBox_valueChanged(int arg1)
{
    if(arg1 > 0 && ui->rowCountSpinBox->value() < 0) return;

    ui->containerQml->setEnabled(true);
    hallCore->setSizeHall(ui->rowCountSpinBox->value(), arg1);
}

void AddHallScheme::on_addSectorButton_clicked()
{
    hallCore->addSectorDialog();

    updateSectors();
}

void AddHallScheme::on_editSectorButton_clicked()
{
    if(ui->sectorsList->currentRow() < 0) return;

    hallCore->editSectorDialog(ui->sectorsList->currentRow());

    updateSectors();
}

void AddHallScheme::on_removeSectorButton_clicked()
{
    if(ui->sectorsList->currentRow() < 0) return;

    hallCore->removeSectorDialog(ui->sectorsList->currentRow());

    updateSectors();
}

void AddHallScheme::on_confirmButton_clicked()
{
    int enabled = 0;
    if(ui->nameEdit->text().isEmpty()) return;
    if(ui->descriptionEdit->text().isEmpty()) return;
    if(ui->enabledHallCheckBox->isChecked())
        enabled = 1;
    else
        enabled = 0;

    query->prepare("INSERT INTO `halls` (`name`, `description`, `rowCount`, `columnCount`, `enabled`) "
                   "VALUES (:name, :description, :rowCount, :colCount, :enabled)");
    query->bindValue(":name", ui->nameEdit->text());
    query->bindValue(":description", ui->descriptionEdit->text());
    query->bindValue(":rowCount", ui->rowCountSpinBox->value());
    query->bindValue(":colCount", ui->columnCountSpinBox->value());
    query->bindValue(":enabled", enabled);
    query->exec();

    int lastInsert = query->lastInsertId().toInt();

    QVector<sector> sectors(hallCore->getSectors());
    for(int i = 0; i < sectors.size(); i++)
    {
        query->prepare("INSERT INTO `sectors_halls` (`hall`, `name`, `description`, `color_places`) "
                       "VALUES (:hall, :name, :description, :color)");
        query->bindValue(":hall", lastInsert);
        query->bindValue(":name", sectors[i].text);
        query->bindValue(":description", sectors[i].description);
        query->bindValue(":color", sectors[i].color);
        query->exec();
        sectors[i].id = query->lastInsertId().toInt();
    }

    QVector<QVector<place> > places(hallCore->getPlaces());
    for(int row = 0; row < places.size(); row++)
        for(int col = 0; col < places[row].size(); col++)
            if(places[row][col].v_place != 0)
            {
                query->prepare("INSERT INTO `places` (`sector`, `place`, `g_place`, `row`) "
                               "VALUES (:sector, :place, :g_place, :row)");
                query->bindValue(":sector", sectors[places[row][col].sectorId].id);
                query->bindValue(":place", places[row][col].v_place);
                query->bindValue(":g_place", col);
                query->bindValue(":row", row);
                query->exec();
            }

    reject();
}

void AddHallScheme::on_cancelButton_clicked()
{
    close();
}

void AddHallScheme::updateSectors()
{
    ui->sectorsList->clear();
    QVector<sector> tmp(hallCore->getSectors());
    for(int i = 0; i < tmp.size(); i++)
        ui->sectorsList->addItem(tmp[i].text);
}

void AddHallScheme::closeEvent(QCloseEvent *event)
{
    QMessageBox msgConfirm;
    msgConfirm.setWindowTitle(tr("Подтвердить выход"));
    msgConfirm.setText(tr("Выйти?"));
    QAbstractButton *msgButtonYes = msgConfirm.addButton(tr("Да"), QMessageBox::YesRole);
    QAbstractButton *msgButtonNo = msgConfirm.addButton(tr("Нет"), QMessageBox::NoRole);
    msgConfirm.exec();
    if(msgConfirm.clickedButton() == msgButtonYes)
        event->accept();
    else
        if(msgConfirm.clickedButton() == msgButtonNo)
            event->ignore();
}

void AddHallScheme::resizeEvent(QResizeEvent *event)
{
    if(ui->columnCountSpinBox->value() > 0 && ui->rowCountSpinBox->value() > 0)
    {
//        hallCore->setSizeHall(ui->rowCountSpinBox->value(), ui->columnCountSpinBox->value());
    }
}
