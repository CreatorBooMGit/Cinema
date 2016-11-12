#include "EditHallScheme.h"
#include "ui_EditHallScheme.h"

#include <QMessageBox>
#include <QQmlContext>
#include <QQuickView>
#include <QDebug>

EditHallScheme::EditHallScheme(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), indexHall(id),
    ui(new Ui::EditHallScheme)
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
//    hallCore->setSizeHall(0, 0);
//    ui->containerQml->setEnabled(false);

    query->prepare("SELECT * FROM halls "
                   "WHERE id_hall = :hall");
    query->bindValue(":hall", indexHall);
    query->exec();
    query->next();
    ui->nameEdit->setText(query->value("name").toString());
    ui->descriptionEdit->setText(query->value("description").toString());
    ui->rowCountSpinBox->setValue(query->value("rowCount").toInt());
    ui->columnCountSpinBox->setValue(query->value("columnCount").toInt());
    ui->enabledHallCheckBox->setChecked(query->value("enabled").toBool());

    hallCore->setSizeHall(query->value("rowCount").toInt(), query->value("columnCount").toInt());

    query->prepare("SELECT * FROM sectors_halls "
                   "WHERE hall = :hall");
    query->bindValue(":hall", indexHall);
    query->exec();
    while(query->next())
        hallCore->addSector(query->value("id_sector").toInt(), query->value("name").toString(), query->value("description").toString(), query->value("color_places").toString());
    updateSectors();

    QVector<sector> sectors(hallCore->getSectors());
    for(int i = 0; i < sectors.size(); i++)
    {
        query->prepare("SELECT * FROM places "
                       "WHERE sector = :sector");
        query->bindValue(":sector", sectors[i].id);
        qDebug() << query->exec();
        while(query->next())
        {
            qDebug() << "place:" << query->value("place").toInt() << "g_place:" << query->value("g_place").toInt() << "row:" << query->value("row").toInt() << "sector:" << i;
            hallCore->setPlace(query->value("id_place").toInt(), query->value("place").toInt(), query->value("g_place").toInt(), query->value("row").toInt(), i);
        }
    }
    hallCore->sendPlaces();
}

EditHallScheme::~EditHallScheme()
{
    delete ui;
}

void EditHallScheme::on_rowCountSpinBox_valueChanged(int arg1)
{
    if(arg1 > 0 && ui->columnCountSpinBox->value() < 0) return;

    ui->containerQml->setEnabled(true);
    hallCore->setSizeHall(arg1, ui->columnCountSpinBox->value());
}

void EditHallScheme::on_columnCountSpinBox_valueChanged(int arg1)
{
    if(arg1 > 0 && ui->rowCountSpinBox->value() < 0) return;

    ui->containerQml->setEnabled(true);
    hallCore->setSizeHall(ui->rowCountSpinBox->value(), arg1);
}

void EditHallScheme::on_addSectorButton_clicked()
{
    hallCore->addSectorDialog();

    updateSectors();
}

void EditHallScheme::on_editSectorButton_clicked()
{
    if(ui->sectorsList->currentRow() < 0) return;

    hallCore->editSectorDialog(ui->sectorsList->currentRow());

//    updateSectors();
}

void EditHallScheme::on_removeSectorButton_clicked()
{
    if(ui->sectorsList->currentRow() < 0) return;

    hallCore->removeSectorDialog(ui->sectorsList->currentRow());

    updateSectors();
}

void EditHallScheme::on_confirmButton_clicked()
{
    QMessageBox msgConfirm;
    msgConfirm.setWindowTitle(tr("Подтвердить изменение"));
    msgConfirm.setText(tr("Изменить?"));
    msgConfirm.addButton(tr("Да"), QMessageBox::YesRole);
    QAbstractButton *msgButtonNo = msgConfirm.addButton(tr("Нет"), QMessageBox::NoRole);
    msgConfirm.exec();
    if(msgConfirm.clickedButton() == msgButtonNo) return;

    int enabled = 0;
    if(ui->nameEdit->text().isEmpty()) return;
    if(ui->descriptionEdit->text().isEmpty()) return;
    if(ui->enabledHallCheckBox->isChecked())
        enabled = 1;
    else
        enabled = 0;

    query->prepare("UPDATE `halls` "
                   "SET `name` = :name, `description` = :description, `rowCount` = :rowCount, `columnCount` = :colCount, `enabled` = :enabled "
                   "WHERE `id_hall`= :hall");
    query->bindValue(":name", ui->nameEdit->text());
    query->bindValue(":description", ui->descriptionEdit->text());
    query->bindValue(":rowCount", ui->rowCountSpinBox->value());
    query->bindValue(":colCount", ui->columnCountSpinBox->value());
    query->bindValue(":enabled", enabled);
    query->bindValue(":hall", indexHall);
    query->exec();

    QVector<sector> sectors(hallCore->getSectors());
    for(int i = 0; i < sectors.size(); i++)
    {
        if(sectors[i].status == HallQml::StatusNew)
        {
            query->prepare("INSERT INTO `sectors_halls` (`hall`, `name`, `description`, `color_places`) "
                           "VALUES (:hall, :name, :description, :color)");
            query->bindValue(":hall", indexHall);
            query->bindValue(":name", sectors[i].text);
            query->bindValue(":description", sectors[i].description);
            query->bindValue(":color", sectors[i].color);
            query->exec();
            sectors[i].id = query->lastInsertId().toInt();
        }
        else
            if(sectors[i].status == HallQml::StatusUpdate)
            {
                query->prepare("UPDATE `sectors_halls` "
                               "SET `name` = :name, `description` = :description, `color_places` = :color "
                               "WHERE `id_sector` = :sector");
                query->bindValue(":name", sectors[i].text);
                query->bindValue(":description", sectors[i].description);
                query->bindValue(":color", sectors[i].color);
                query->bindValue(":sector", sectors[i].id);
                query->exec();
            }
    }

    QVector<QVector<place> > places(hallCore->getPlaces());
    for(int row = 0; row < places.size(); row++)
        for(int col = 0; col < places[row].size(); col++)
        {
            if(places[row][col].v_place != 0 && places[row][col].status == HallQml::StatusNew)
            {
                query->prepare("INSERT INTO `places` (`sector`, `place`, `g_place`, `row`) "
                               "VALUES (:sector, :place, :g_place, :row)");
                query->bindValue(":sector", sectors[places[row][col].sectorId].id);
                query->bindValue(":place", places[row][col].v_place);
                query->bindValue(":g_place", col);
                query->bindValue(":row", row);
                query->exec();
            }
            else
                if(places[row][col].v_place != 0 && places[row][col].status == HallQml::StatusUpdate)
                {
                    query->prepare("UPDATE `places` "
                                   "SET `sector` = :sector, `place` = :place, `g_place` = :g_place, `row` = :row "
                                   "WHERE `id_place` = :id_place");
                    query->bindValue(":sector", sectors[places[row][col].sectorId].id);
                    query->bindValue(":place", places[row][col].v_place);
                    query->bindValue(":g_place", col);
                    query->bindValue(":row", row);
                    query->bindValue(":id_place", places[row][col].idPlace);
                    query->exec();
                }
            else
                    if(places[row][col].v_place == 0 && places[row][col].status == HallQml::StatusFree)
                    {
                        query->prepare("DELETE FROM `places` "
                                       "WHERE `id_place` = :id_place");
                        query->bindValue(":id_place", places[row][col].idPlace);
                        query->exec();
                    }
        }

    reject();
}

void EditHallScheme::on_cancelButton_clicked()
{
    close();
}

void EditHallScheme::updateSectors()
{
    ui->sectorsList->clear();
    QVector<sector> tmp(hallCore->getSectors());
    for(int i = 0; i < tmp.size(); i++)
        ui->sectorsList->addItem(tmp[i].text);
}

void EditHallScheme::closeEvent(QCloseEvent *event)
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


void EditHallScheme::on_sectorsList_itemSelectionChanged()
{
    ui->editSectorButton->setEnabled(true);
    ui->removeSectorButton->setEnabled(true);
}
