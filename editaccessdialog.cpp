#include "editaccessdialog.h"
#include "ui_editaccessdialog.h"
#include <QMessageBox>

EditAccessDialog::EditAccessDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), idaccess(id),
    ui(new Ui::EditAccessDialog)
{
    ui->setupUi(this);
    setFixedSize(250, 100);

    query->prepare("SELECT name, description "
                   "FROM access_level "
                   "WHERE idaccess = :id");
    query->bindValue(":id", idaccess);
    query->exec();
    query->next();
    ui->nameEdit->setText(query->value(0).toString());
    ui->descriptionEdit->setText(query->value(1).toString());
}

EditAccessDialog::~EditAccessDialog()
{
    delete ui;
}

void EditAccessDialog::on_editButton_clicked()
{
    query->prepare("UPDATE `access_level` "
                   "SET `description`= :description "
                   "WHERE `idaccess`= :idaccess");
    query->bindValue(":idaccess", idaccess);
    query->bindValue(":description", ui->descriptionEdit->text());
    if(query->exec())
    {
        close();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Ошибка"));
        msg.setText(tr("Ошибка обновления данных"));
        msg.setInformativeText(tr("Невозможно обновить данные"));
        msg.exec();
    }
}

void EditAccessDialog::on_cancelButton_clicked()
{
    close();
}
