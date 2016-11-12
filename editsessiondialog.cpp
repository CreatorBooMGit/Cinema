#include "editsessiondialog.h"
#include "ui_editsessiondialog.h"
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

#include "HallScheme/usertypes.h"

EditSessionDialog::EditSessionDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), id_session(id),
    ui(new Ui::EditSessionDialog)
{
    ui->setupUi(this);

    query->exec("SELECT id_hall, name FROM halls "
                "WHERE enabled = 1");
    while(query->next())
    {
        hall tmp;
        tmp.idHall = query->value(0).toInt();
        tmp.name = query->value(1).toString();
        halls.push_back(tmp);
        ui->hallComboBox->addItem(tmp.name);
    }

    query->exec("SELECT id_film, name "
                "FROM films");
    while(query->next())
    {
        film tmp;
        tmp.idFilm = query->value(0).toInt();
        tmp.name = query->value(1).toString();
        films.push_back(tmp);
        ui->filmComboBox->addItem(tmp.name);
    }

    query->prepare("SELECT * "
                   "FROM sessions "
                   "WHERE id_session = :id");
    query->bindValue(":id", id_session);
    query->exec();
    query->next();

    int indexHall = -1, indexFilm = -1;
    for(int i = 0; i < halls.size(); i++)
        if(halls[i].idHall == query->value("hall").toInt())
        {
            indexHall = i;
            break;
        }
    for(int i = 0; i < films.size(); i++)
        if(films[i].idFilm == query->value("film").toInt())
        {
            indexFilm = i;
            break;
        }

    ui->hallComboBox->setCurrentIndex(indexHall);
    ui->filmComboBox->setCurrentIndex(indexFilm);
    ui->dateEdit->setDate(QDate::fromString(query->value("date").toString(), "yyyy-MM-dd"));
    ui->timeEdit->setTime(QTime::fromString(query->value("time").toString(), "hh:mm:ss"));

    query->prepare("SELECT sectors_halls.name, price_of_tickets.sector, price_of_tickets.price "
                   "FROM sectors_halls, price_of_tickets "
                   "WHERE price_of_tickets.sector = sectors_halls.id_sector AND price_of_tickets.session = :session");
    query->bindValue(":session", id_session);
    query->exec();
    sectors.clear();
    ui->priceTable->setRowCount(query->size());
    while(query->next())
    {
        sector tmp;
        tmp.idSector = query->value("sector").toInt();
        tmp.name = query->value("name").toString();
        tmp.price = query->value("price").toString().toDouble();
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

EditSessionDialog::~EditSessionDialog()
{
    delete ui;
}

void EditSessionDialog::on_cancelButton_clicked()
{
    close();
}

void EditSessionDialog::on_editButton_clicked()
{
    if(updatePrice)
    {
        query->prepare("DELETE FROM tickets "
                       "WHERE session = :session");
        query->bindValue(":session", id_session);
        query->exec();

        query->prepare("DELETE FROM price_of_tickets "
                       "WHERE session = :session");
        query->bindValue(":session", id_session);
        query->exec();
    }

    query->prepare("UPDATE `cinema`.`sessions` "
                   "SET `hall`=:hall, `film`=:film, `date`=:date, `time`=:time "
                   "WHERE `id_session`=:id_session");
    query->bindValue(":id_session", id_session);
    query->bindValue(":hall", halls[ui->hallComboBox->currentIndex()].idHall);
    query->bindValue(":film", films[ui->filmComboBox->currentIndex()].idFilm);
    query->bindValue(":date", ui->dateEdit->date().toString("yyyy-MM-dd"));
    query->bindValue(":time", ui->timeEdit->time().toString("HH:mm:ss"));
    if(query->exec())
    {
        int lastSession = query->lastInsertId().toInt();
        if(updatePrice)
        {
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
        }
        else
        {
            for(int i = 0; i < sectors.size(); i++)
            {
                query->prepare("UPDATE `price_of_tickets` "
                               "SET `price` = :price "
                               "WHERE `session` = :session AND `sector` = :sector");
                query->bindValue(":price", sectors[i].price);
                query->bindValue(":session", id_session);
                query->bindValue(":sector", sectors[i].idSector);
                query->exec();
            }
        }
        close();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Ошибка"));
        msg.setText(tr("Ошибка обновления сеанса"));
        msg.exec();
    }
}

void EditSessionDialog::on_hallComboBox_activated(int index)
{
    updatePrice = true;

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

void EditSessionDialog::on_priceTable_itemChanged(QTableWidgetItem *item)
{
    sectors[item->row()].price = item->text().toDouble();
}
