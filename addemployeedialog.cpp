#include "addemployeedialog.h"
#include "ui_addemployeedialog.h"
#include <QCryptographicHash>
#include <QMessageBox>

AddEmployeeDialog::AddEmployeeDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AddEmployeeDialog)
{
    ui->setupUi(this);
    setFixedSize(250, 210);

    query->prepare("SELECT posts.id_post, posts.name "
                   "FROM posts");
    query->exec();
    while(query->next())
    {
        postsId.push_back(query->value(0).toInt());
        ui->postComboBox->addItem(query->value(1).toString());
    }
}

AddEmployeeDialog::~AddEmployeeDialog()
{
    delete ui;
}

void AddEmployeeDialog::on_addButton_clicked()
{
    if(ui->surnameEdit->text().isEmpty()) return;
    if(ui->nameEdit->text().isEmpty()) return;
    if(ui->patronymicEdit->text().isEmpty()) return;
    if(ui->postComboBox->currentIndex() < 0) return;
    if(ui->loginEdit->text().isEmpty()) return;
    if(ui->passwordEdit->text().isEmpty()) return;

    query->prepare("INSERT INTO employees  (`login`, `password`, `surname`, `name`, `patronymic`, `post`) "
                   "VALUES (:login, :password, :surname, :name, :patronymic, :post)");
    query->bindValue(":login", ui->loginEdit->text());
    query->bindValue(":password", QCryptographicHash::hash(ui->passwordEdit->text().append("x7sgas352da").toLatin1(), QCryptographicHash::Sha3_512).toHex());
    query->bindValue(":surname", ui->surnameEdit->text());
    query->bindValue(":name", ui->nameEdit->text());
    query->bindValue(":patronymic", ui->patronymicEdit->text());
    query->bindValue(":post", postsId[ui->postComboBox->currentIndex()]);
    if(query->exec())
    {
        close();
    }
    else
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Ошибка"));
        msg.setText(tr("Ошибка добавления данных"));
        msg.setInformativeText(tr("Данная должность уже существует"));
        msg.exec();
    }
}

void AddEmployeeDialog::on_cancelButton_clicked()
{
    close();
}
