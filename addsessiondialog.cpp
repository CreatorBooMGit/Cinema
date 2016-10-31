#include "addsessiondialog.h"
#include "ui_addsessiondialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

#include "HallScheme/usertypes.h"

AddSessionDialog::AddSessionDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AddSessionDialog)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());

    query->exec("SELECT id_hall, name FROM halls WHERE enabled = 1");
    while(query->next())
    {
        hall tmp;
        tmp.idHall = query->value(0).toInt();
        tmp.name = query->value(1).toString();
        halls.push_back(tmp);
        ui->hallComboBox->addItem(tmp.name);
    }

    query->exec("SELECT id_film, name FROM films");
    while(query->next())
    {
        film tmp;
        tmp.idFilm = query->value(0).toInt();
        tmp.name = query->value(1).toString();
        films.push_back(tmp);
        ui->filmComboBox->addItem(tmp.name);
    }
}

AddSessionDialog::~AddSessionDialog()
{
    delete ui;
}

void AddSessionDialog::on_hallComboBox_currentIndexChanged(int index)
{
    sectors.clear();

    query->prepare("SELECT sectors_halls.id_sector, sectors_halls.name "
                   "FROM sectors_halls "
                   "WHERE hall = :hall");
    query->bindValue(":hall", halls[index].idHall);
    query->exec();
    ui->priceTable->setRowCount(query->size());
    while(query->next())
    {
        sector tmp;
        tmp.idSector = query->value("id_sector").toInt();
        tmp.name = query->value("name").toString();
        tmp.price = 0;
        sectors.push_back(tmp);

        QTableWidgetItem *itemName = new QTableWidgetItem;
        QTableWidgetItem *itemPrice = new QTableWidgetItem;
        itemName->setText(tmp.name);
        itemPrice->setText(QString::number(tmp.price));
        itemName->setFlags(Qt::ItemIsEnabled);
        itemPrice->setFlags(Qt::ItemIsEnabled  | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        ui->priceTable->setItem(sectors.size() - 1, 0, itemName);
        ui->priceTable->setItem(sectors.size() - 1, 1, itemPrice);
    }
}

void AddSessionDialog::on_hallComboBox_activated(int index)
{
    qDebug() << "actived:" << index;
}

void AddSessionDialog::on_addButton_clicked()
{
    query->prepare("INSERT INTO `sessions` (`hall`, `film`, `date`, `time`) "
                   "VALUES (:hall, :film, :date, :time)");
    query->bindValue(":hall", halls[ui->hallComboBox->currentIndex()].idHall);
    query->bindValue(":film", films[ui->filmComboBox->currentIndex()].idFilm);
    query->bindValue(":date", ui->dateEdit->date().toString("yyyy-MM-dd"));
    query->bindValue(":time", ui->timeEdit->time().toString("HH:mm:ss"));
    if(query->exec())
    {
        int lastSession = query->lastInsertId().toInt();
        query->prepare("SELECT places.id_place "
                       "FROM places, sectors_halls "
                       "WHERE sectors_halls.id_sector = places.sector AND sectors_halls.hall = :hall");
        query->bindValue(":hall", halls[ui->hallComboBox->currentIndex()].idHall);
        if(query->exec())
        {
            QVector<int> placesId;
            while(query->next())
                placesId.push_back(query->value("id_place").toInt());

            for(int i = 0; i < placesId.size(); i++)
            {
                query->prepare("INSERT INTO `tickets` (`session`, `place`, `status`) "
                               "VALUES (:session, :place, :status)");
                query->bindValue(":session", lastSession);
                query->bindValue(":place", placesId[i]);
                query->bindValue(":status", HallQml::StatusFree);
                query->exec();
            }
        }

        for(int i = 0; i < sectors.size(); i++)
        {
            query->prepare("INSERT INTO `price_of_tickets` (`session`, `sector`, `price`) "
                           "VALUES (:session, :sector, :price)");
            query->bindValue(":session", lastSession);
            query->bindValue(":sector", sectors[i].idSector);
            query->bindValue(":price", sectors[i].price);
            query->exec();
        }
        close();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Ошибка"));
        msg.setText(tr("Ошибка добавления сеанса"));
        msg.exec();
    }
}

void AddSessionDialog::on_cancelButton_clicked()
{
    close();
}

void AddSessionDialog::on_priceTable_itemChanged(QTableWidgetItem *item)
{
    sectors[item->row()].price = item->text().toDouble();
}
