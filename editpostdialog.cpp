#include "editpostdialog.h"
#include "ui_editpostdialog.h"
#include <QMessageBox>

EditPostDialog::EditPostDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), query(q), idpost(id),
    ui(new Ui::EditPostDialog)
{
    ui->setupUi(this);
    setFixedSize(350, 300);

    query->prepare("SELECT posts.name "
                   "FROM posts "
                   "WHERE posts.id_post = :idpost");
    query->bindValue(":idpost", idpost);
    query->exec();
    query->next();
    ui->postEdit->setText(query->value(0).toString());

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

    for(int i = 0; i < accessLevels.size(); i++)
    {
        query->prepare("SELECT access_post.idaccess "
                       "FROM access_post "
                       "WHERE access_post.access = :idaccess AND access_post.post = :idpost");
        query->bindValue(":idaccess", accessLevels[i].id);
        query->bindValue(":idpost", idpost);
        query->exec();
        if(query->next())
            ui->accessList->item(i)->setCheckState(Qt::Checked);
    }
}

EditPostDialog::~EditPostDialog()
{
    delete ui;
}

void EditPostDialog::on_editButton_clicked()
{
    query->prepare("DELETE FROM `access_post` WHERE `post` = :idpost");
    query->bindValue(":idpost", idpost);
    query->exec();

    query->prepare("UPDATE `posts` "
                   "SET `name`= :name "
                   "WHERE `id_post`= :idpost");
    query->bindValue(":name", ui->postEdit->text());
    query->bindValue(":idpost", idpost);
    if(query->exec())
    {
        for(int i = 0; i < ui->accessList->count(); i++)
        {
            if(ui->accessList->item(i)->checkState() == Qt::Checked)
            {
                query->prepare("INSERT INTO `access_post` (`access`, `post`) VALUES (:access, :post)");
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
        msg.setText(tr("Ошибка обновления данных"));
        msg.setInformativeText(tr("Невозможно обновить данные"));
        msg.exec();
    }
}

void EditPostDialog::on_cancelButton_clicked()
{
    close();
}
