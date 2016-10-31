#include "authentificationuserdialog.h"
#include "ui_authentificationuserdialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QCryptographicHash>
#include <QByteArray>
#include <QSqlError>

AuthentificationUserDialog::AuthentificationUserDialog(user *info, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), infoUser(info),
    ui(new Ui::AuthentificationUserDialog)
{
    ui->setupUi(this);
//    setWindowModality(Qt::WindowModal);
}

AuthentificationUserDialog::~AuthentificationUserDialog()
{
    delete ui;
}

void AuthentificationUserDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
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
    query->prepare("SELECT employees.id_employee, employees.password, employees.surname, employees.name AS nameE, employees.patronymic, posts.id_post, posts.name AS nameP "
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
    if(query->value("password").toString().compare(QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha3_512).toHex()) == 0)
    {
        infoUser->login = login;
        infoUser->idlogin = query->value("id_employee").toInt();
        infoUser->surname = query->value("surname").toString();
        infoUser->name = query->value("nameE").toString();
        infoUser->patronymic = query->value("patronymic").toString();
        infoUser->idpost = query->value("nameP").toInt();
        infoUser->postName = query->value("nameP").toString();
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
    exit(0);
}
