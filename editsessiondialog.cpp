#include "editsessiondialog.h"
#include "ui_editsessiondialog.h"
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

EditSessionDialog::EditSessionDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), id_session(id),
    ui(new Ui::EditSessionDialog)
{
    ui->setupUi(this);

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

    query->prepare("SELECT * FROM sessions WHERE id_session = :id");
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
    query->prepare("UPDATE `cinema`.`sessions` "
                   "SET `hall`=:hall, `film`=:film, `date`=:date, `time`=:time "
                   "WHERE `id_session`=:id_session");
    query->bindValue(":id_session", id_session);
    query->bindValue(":hall", halls[ui->hallComboBox->currentIndex()].idHall);
    query->bindValue(":film", films[ui->filmComboBox->currentIndex()].idFilm);
    query->bindValue(":date", ui->dateEdit->date().toString("yyyy-MM-dd"));
    query->bindValue(":time", ui->timeEdit->time().toString("HH:mm:ss"));
    if(query->exec())
        close();
    else
    {
        qDebug() << query->boundValues();
        qDebug() << query->lastQuery();
        qDebug() << query->lastError().text();
        QMessageBox msg;
        msg.setWindowTitle(tr("Ошибка"));
        msg.setText(tr("Ошибка обновления сеанса"));
        msg.exec();
    }
}
