#include "authentificationuserdialog.h"
#include "ui_authentificationuserdialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QCryptographicHash>
#include <QByteArray>
#include <QSqlError>

AuthentificationUserDialog::AuthentificationUserDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AuthentificationUserDialog)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
}

AuthentificationUserDialog::~AuthentificationUserDialog()
{
    delete ui;
}

void AuthentificationUserDialog::closeEvent(QCloseEvent *event)
{
    emit sendCloseDialog();
//    event->ignore();
}

void AuthentificationUserDialog::on_authButton_clicked()
{
    QString login = ui->loginEdit->text();
    QString password = ui->passEdit->text();
    password.append("x7sgas352da");
//    password.fromLatin1();

//    qDebug() << QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha3_512).toHex();
//    return;
    query->clear();
    query->prepare("SELECT employees.password, employees.surname, employees.name, employees.patronymic, posts.name "
                   "FROM employees, posts "
                   "WHERE employees.login = :login AND employees.post = posts.id_post");
    query->bindValue(":login", login);
    qDebug() << query->exec() << query->size() << query->boundValues();
    qDebug() << query->lastError().text();
    if(query->size() <= 0) {
        QMessageBox msg;
        msg.setWindowTitle(tr("Ошибка"));
        msg.setText(tr("Неверный логин!"));
        msg.exec();
        ui->loginEdit->clear();
        ui->passEdit->clear();
        ui->loginEdit->setFocus();
        return;
    }
    query->next();
    if(query->value(0).toString().compare(QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha3_512).toHex()) == 0)
    {
        user tmp;
        tmp.login = login;
        tmp.surname = query->value(1).toString();
        tmp.name = query->value(2).toString();
        tmp.patronymic = query->value(3).toString();
        tmp.postName = query->value(4).toString();
        emit sendAccessUser(tmp);
        reject();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Ошибка"));
        msg.setText(tr("Неверный пароль!"));
        msg.exec();
        ui->passEdit->clear();
        ui->passEdit->setFocus();
        return;
    }
}

void AuthentificationUserDialog::on_closeButton_clicked()
{
    close();
}
