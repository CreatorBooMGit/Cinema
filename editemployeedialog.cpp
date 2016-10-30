#include "editemployeedialog.h"
#include "ui_editemployeedialog.h"
#include <QCryptographicHash>
#include <QMessageBox>

EditEmployeeDialog::EditEmployeeDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), employeeId(id),
    ui(new Ui::EditEmployeeDialog)
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

    query->prepare("SELECT employees.surname, employees.name, employees.patronymic, posts.name, employees.login "
                   "FROM employees, posts "
                   "WHERE posts.id_post = employees.post AND employees.id_employee = :id_employee");
    query->bindValue(":id_employee", employeeId);
    query->exec();
    query->next();
    ui->surnameEdit->setText(query->value(0).toString());
    ui->nameEdit->setText(query->value(1).toString());
    ui->patronymicEdit->setText(query->value(2).toString());
    ui->postComboBox->setCurrentText(query->value(3).toString());
    ui->loginEdit->setText(query->value(4).toString());
}

EditEmployeeDialog::~EditEmployeeDialog()
{
    delete ui;
}

void EditEmployeeDialog::on_editButton_clicked()
{
    if(ui->surnameEdit->text().isEmpty()) return;
    if(ui->nameEdit->text().isEmpty()) return;
    if(ui->patronymicEdit->text().isEmpty()) return;
    if(ui->postComboBox->currentIndex() < 0) return;
    if(ui->loginEdit->text().isEmpty()) return;

    if(ui->passwordEdit->text().isEmpty())
        query->prepare("UPDATE `employees` "
                       "SET `login` = :login, `surname` = :surname, `name` = :name, `patronymic` = :patronymic, `post` = :post "
                       "WHERE `id_employee`= :idemployee");
    else
        query->prepare("UPDATE `employees` "
                       "SET `login` = :login, `password` = :password, `surname` = :surname, `name` = :name, `patronymic` = :patronymic, `post` = :post "
                       "WHERE `id_employee`= :idemployee");

    query->bindValue(":idemployee", employeeId);
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
        msg.setText(tr("Ошибка обновления данных"));
        msg.setInformativeText(tr("Данные не удалось обновить"));
        msg.exec();
    }
}

void EditEmployeeDialog::on_cancelButton_clicked()
{
    close();
}
