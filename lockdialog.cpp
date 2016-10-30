#include "lockdialog.h"
#include "ui_lockdialog.h"
#include "authentificationuserdialog.h"
#include <QMessageBox>
#include <QCryptographicHash>

LockDialog::LockDialog(QString *log, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), login(log),
    ui(new Ui::LockDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::CustomizeWindowHint);
    setWindowModality(Qt::WindowModal);

    ui->loginLabel->setText("<html><head/><body><p><span style=\" font-size:14pt; font-weight:600; color:#ff5500;\">" + (*login)+ "</span></p></body></html>");
}

void LockDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

LockDialog::~LockDialog()
{
    delete ui;
}

void LockDialog::on_changeButton_clicked()
{
    reject();
    emit sendChangeUser();
}

void LockDialog::on_unlockButton_clicked()
{
    QString password = ui->passEdit->text();
    password.append("x7sgas352da");

    query->clear();
    query->prepare("SELECT employees.password "
                   "FROM employees "
                   "WHERE employees.login = :login");
    query->bindValue(":login", (*login));
    query->exec();
    query->next();
    if(query->value(0).toString().compare(QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha3_512).toHex()) == 0)
    {
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
