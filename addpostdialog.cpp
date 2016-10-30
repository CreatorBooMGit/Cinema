#include "addpostdialog.h"
#include "ui_addpostdialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

AddPostDialog::AddPostDialog(QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::AddPostDialog)
{
    ui->setupUi(this);
    setFixedSize(350, 300);

    query->prepare("SELECT access_level.idaccess, access_level.name, access_level.description "
                   "FROM access_level");
    query->exec();
    while(query->next())
    {
        access tmp;
        tmp.id = query->value(0).toInt();
        tmp.name = query->value(1).toString();
        tmp.description = query->value(2).toString();
        accessLevels.push_back(tmp);
        QListWidgetItem *item = new QListWidgetItem(tmp.description);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Unchecked);
        item->setToolTip(tmp.name);
        ui->accessList->addItem(item);
    }
}

AddPostDialog::~AddPostDialog()
{
    delete ui;
}

void AddPostDialog::on_addButton_clicked()
{
    query->prepare("INSERT INTO `cinema`.`posts` (`name`) "
                   "VALUES (:name); ");
    query->bindValue(":name", ui->postEdit->text());
    if(query->exec())
    {
        int idpost = query->lastInsertId().toInt();
        for(int i = 0; i < ui->accessList->count(); i++)
        {
            if(ui->accessList->item(i)->checkState() == Qt::Checked)
            {
                query->prepare("INSERT INTO `cinema`.`access_post` (`access`, `post`) VALUES (:access, :post)");
                query->bindValue(":access", accessLevels[i].id);
                query->bindValue(":post", idpost);
                query->exec();
            }
        }
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

void AddPostDialog::on_cancelButton_clicked()
{
    close();
}
