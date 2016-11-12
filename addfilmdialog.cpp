#include "addfilmdialog.h"
#include "ui_addfilmdialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

AddFilmDialog::AddFilmDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AddFilmDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::WindowModal);
}

AddFilmDialog::~AddFilmDialog()
{
    delete ui;
}

void AddFilmDialog::on_confirmButton_clicked()
{
    if(ui->nameEdit->text().isEmpty()) return;

    query->prepare("INSERT INTO `films` (`name`, `duration`, `start_date_of_hire`, `finish_date_of_hire`, `distributors_company`) "
                   "VALUES (:name, :duration, :start_date_of_hire, :finish_date_of_hire, :distributors_company)");
    query->bindValue(":name", ui->nameEdit->text());
    query->bindValue(":duration", ui->durationTimeEdit->time().toString("HH:mm:ss"));
    query->bindValue(":start_date_of_hire", ui->startDateEdit->date().toString("yyyy-MM-dd"));
    query->bindValue(":finish_date_of_hire", ui->finishDateEdit->date().toString("yyyy-MM-dd"));
    query->bindValue(":distributors_company", ui->distEdit->text());
    bool exec = query->exec();
    qDebug() << "Insert Film:" << exec;
    if(exec)
        close();
    else
    {
        qDebug() << query->boundValues();
        qDebug() << query->lastQuery();
        qDebug() << query->lastError().text();
        QMessageBox msg;
        msg.setInformativeText(tr("Ошибка ввода данных"));
        msg.exec();
    }
}

void AddFilmDialog::on_cancelButton_clicked()
{
    close();
}
