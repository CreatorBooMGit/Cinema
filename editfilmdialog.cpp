#include "editfilmdialog.h"
#include "ui_editfilmdialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

EditFilmDialog::EditFilmDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), filmId(id),
    ui(new Ui::EditFilmDialog)
{
    ui->setupUi(this);
    query->prepare("SELECT * FROM films WHERE id_film = :id");
    query->bindValue(":id", filmId);
    query->exec();
    query->next();
    ui->nameEdit->setText(query->value("name").toString());
    ui->durationTimeEdit->setTime(QTime::fromString(query->value("duration").toString(), "HH:mm:ss"));
    ui->startDateEdit->setDate(QDate::fromString(query->value("start_date_of_hire").toString(), "yyyy-MM-dd"));
    ui->finishDateEdit->setDate(QDate::fromString(query->value("finish_date_of_hire").toString(), "yyyy-MM-dd"));
    ui->distEdit->setText(query->value("distributors_company").toString());
}

EditFilmDialog::~EditFilmDialog()
{
    delete ui;
}

void EditFilmDialog::on_confirmButton_clicked()
{
    query->prepare("UPDATE films "
                   "SET `name`=:name, `duration`=:duration, `start_date_of_hire`=:start_date_of_hire, `finish_date_of_hire`=:finish_date_of_hire, `distributors_company`=:distributors_company "
                   "WHERE id_film = :id");
    query->bindValue(":id", filmId);
    query->bindValue(":name", ui->nameEdit->text());
    query->bindValue(":duration", ui->durationTimeEdit->time().toString("HH:mm:ss"));
    query->bindValue(":start_date_of_hire", ui->startDateEdit->date().toString("yyyy-MM-dd"));
    query->bindValue(":finish_date_of_hire", ui->finishDateEdit->date().toString("yyyy-MM-dd"));
    query->bindValue(":distributors_company", ui->distEdit->text());
    bool exec = query->exec();
    qDebug() << "update:" << exec;
    if(exec)
        close();
    else
    {
        qDebug() << query->boundValues();
        qDebug() << query->lastQuery();
        qDebug() << query->lastError().text();
        QMessageBox msg;
        msg.setInformativeText(tr("Ошибка обновления данных"));
        msg.exec();
    }
}

void EditFilmDialog::on_cancelButton_clicked()
{
    close();
}
