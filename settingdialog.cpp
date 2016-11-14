#include <QLayout>
#include <QStackedWidget>
#include <QSpacerItem>
#include <QColorDialog>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QSqlResult>
#include <QMenu>

#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "settinghall.h"
#include "settingschemhalldialog.h"
#include "test.h"
#include "addpostdialog.h"
#include "editpostdialog.h"
#include "editaccessdialog.h"
#include "AddHallScheme.h"
#include "EditHallScheme.h"
#include "authentificationuserdialog.h"

SettingDialog::SettingDialog(QSqlQuery *q, Access *a, QWidget *parent) :
    QDialog(parent), query(q), accessCheck(a),
    sett(new Ui::SettingDialog)
{
    sett->setupUi(this);
    sett->stackedWidget->setCurrentIndex(0);
    setWindowTitle(tr("Настройки"));

    settingRead("settingUser.ini");

    updateAccess();
    updateHallsTable();
    updateAccessLevelTable();
    updatePostsTable();
}

SettingDialog::~SettingDialog()
{
    delete sett;
}

void SettingDialog::resizeEvent(QResizeEvent *event)
{
//    for(int i = 0; i < sett->tableHall->columnCount(); i++)
    //        sett->tableHall->setColumnWidth(i, sett->tableHall->size().width() / sett->tableHall->columnCount());
}

//void SettingDialog::closeEvent(QCloseEvent *event)
//{
//    emit sendSettingsUpdate();
//}

void SettingDialog::settingWrite(QString filename)
{
    QSettings settings(filename, QSettings::IniFormat);       // Файл настроек
    settings.beginGroup("Database");
        settings.setValue("host", sett->hostEdit->text());
        settings.setValue("port", sett->portEdit->text().toInt());
        settings.setValue("database", sett->databaseEdit->text());
        settings.setValue("login", sett->loginEdit->text());
        if(!sett->passwordEdit->text().isEmpty())
            settings.setValue("password", sett->passwordEdit->text());
    settings.endGroup();
}

void SettingDialog::settingRead(QString filename)
{
    QSettings settings(filename, QSettings::IniFormat);       // Файл настроек
    settings.beginGroup("Database");
    sett->hostEdit->setText(settings.value("host", QString()).toString());
    sett->portEdit->setText(settings.value("port", QString()).toString());
    sett->databaseEdit->setText(settings.value("database", QString()).toString());
    sett->loginEdit->setText(settings.value("login", QString()).toString());
    settings.endGroup();

    // База данных

//    QPalette pixmp;
    //    pixmp.setColor();
}

void SettingDialog::updateHallsTable()
{
    halls.clear();
    query->prepare("SELECT * FROM halls");
    query->exec();
    sett->tableHalls->setRowCount(query->size());
    while(query->next())
    {
        hall tmp;
        tmp.id = query->value("id_hall").toInt();
        tmp.name = query->value("name").toString();
        tmp.description = query->value("description").toString();
        tmp.rowCount = query->value("rowCount").toInt();
        tmp.colCount = query->value("columnCount").toInt();
        tmp.enabled = query->value("enabled").toInt();
        halls.push_back(tmp);
        QTableWidgetItem *itemName = new QTableWidgetItem(tmp.name);
        QTableWidgetItem *itemColumnCount = new QTableWidgetItem(QString::number(tmp.colCount));
        QTableWidgetItem *itemRowCount = new QTableWidgetItem(QString::number(tmp.rowCount));
        QTableWidgetItem *itemEnabled = new QTableWidgetItem;
        itemName->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        itemColumnCount->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        itemRowCount->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        itemEnabled->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        if(tmp.enabled == 0)
        {
            itemEnabled->setText(tr("Не доступен"));
            itemEnabled->setCheckState(Qt::Unchecked);
        }
        else
        {
            itemEnabled->setText(tr("Доступен"));
            itemEnabled->setCheckState(Qt::Checked);
        }
        sett->tableHalls->setItem(halls.size() - 1, 0, itemName);
        sett->tableHalls->setItem(halls.size() - 1, 1, itemRowCount);
        sett->tableHalls->setItem(halls.size() - 1, 2, itemColumnCount);
        sett->tableHalls->setItem(halls.size() - 1, 3, itemEnabled);
    }
}

void SettingDialog::updateAccessLevelTable()
{
    accessLevels.clear();
    query->prepare("SELECT access_level.idaccess, access_level.name, access_level.description "
                   "FROM access_level");
    query->exec();
    sett->tableAccess->setRowCount(query->size());
    while(query->next())
    {
        access tmp;
        tmp.id = query->value(0).toInt();
        tmp.name = query->value(1).toString();
        tmp.description = query->value(2).toString();
        accessLevels.push_back(tmp);
        QTableWidgetItem *itemName = new QTableWidgetItem(tmp.name);
        QTableWidgetItem *itemDescription = new QTableWidgetItem(tmp.description);
        itemName->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        itemDescription->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        sett->tableAccess->setItem(accessLevels.size() - 1, 0, itemName);
        sett->tableAccess->setItem(accessLevels.size() - 1, 1, itemDescription);
    }
}

void SettingDialog::updatePostsTable()
{
    posts.clear();
    query->prepare("SELECT posts.id_post, posts.name "
                   "FROM posts");
    query->exec();
    sett->tablePosts->setRowCount(query->size());
    while(query->next())
    {
        post tmp;
        tmp.id = query->value(0).toInt();
        tmp.name = query->value(1).toString();
        posts.push_back(tmp);
        QTableWidgetItem *itemName = new QTableWidgetItem(tmp.name);
        itemName->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        sett->tablePosts->setItem(posts.size() - 1, 0, itemName);
    }

    for(int i = 0; i < posts.size(); i++)
    {
        query->prepare("SELECT access_level.description "
                       "FROM access_level, access_post "
                       "WHERE access_level.idaccess = access_post.access AND access_post.post = :idpost");
        query->bindValue(":idpost", posts[i].id);
        query->exec();
        QString postAccess;
        while(query->next())
            postAccess.append(query->value(0).toString() + ", ");

        postAccess.remove(postAccess.length() - 2, 2);
        QTableWidgetItem *itemName = new QTableWidgetItem(postAccess);
        itemName->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        sett->tablePosts->setItem(i, 1, itemName);
    }
}

void SettingDialog::updateAccess()
{
    hallsEnabled = accessCheck->checkAccess("hallsEnabled");
    addHallButtonEnabled = accessCheck->checkAccess("addHallEnabled");
    editHallButtonEnabled = accessCheck->checkAccess("editHallEnabled");
    removeHallButtonEnabled = accessCheck->checkAccess("removeHallEnabled");

    serverEnabled = accessCheck->checkAccess("serverEnabled");

    accessEnabled = accessCheck->checkAccess("accessEnabled");
    editAccessLevelEnabled = accessCheck->checkAccess("editAccessEnabled");

    postsEnabled = accessCheck->checkAccess("postsEnabled");
    addPostEnabled = accessCheck->checkAccess("addPostEnabled");
    editPostEnabled = accessCheck->checkAccess("editPostEnabled");
    removePostEnabled = accessCheck->checkAccess("removePostEnabled");

    if(!hallsEnabled)
        sett->hallPage->setVisible(hallsEnabled);
    sett->buttonSettingSchem->setVisible(hallsEnabled);
    sett->addHallButton->setEnabled(addHallButtonEnabled);
    sett->actionAddScheme->setEnabled(addHallButtonEnabled);

    if(!serverEnabled)
        sett->databasePage->setVisible(serverEnabled);

    if(!accessEnabled)
        sett->accessLevelPage->setVisible(accessEnabled);
    sett->buttonSettingAccessLevel->setVisible(accessEnabled);

    if(!postsEnabled)
        sett->postsPage->setVisible(postsEnabled);
    sett->buttonSettingPosts->setVisible(postsEnabled);
    sett->addPostButton->setEnabled(addPostEnabled);
    sett->actionAddPost->setEnabled(addPostEnabled);
}

void SettingDialog::on_buttonSettingSchem_clicked()
{
    sett->stackedWidget->setCurrentIndex(0);
}

void SettingDialog::on_buttonSettingDataBase_clicked()
{
    if(serverEnabled)
        sett->stackedWidget->setCurrentIndex(1);
    else
    {
        user *infoUser = new user;
        AuthentificationUserDialog *authDialog = new AuthentificationUserDialog(infoUser, query, this);
        authDialog->setExitOnCancel(false);
        authDialog->exec();
        if(authDialog->getClickedCancel() == false)
        {
            serverEnabled = accessCheck->checkAccess(infoUser->idpost, "serverEnabled");
            delete authDialog;
            on_buttonSettingDataBase_clicked();
        }
        else
        {
            delete authDialog;
        }
    }
}

void SettingDialog::on_buttonSettingAccessLevel_clicked()
{
    sett->stackedWidget->setCurrentIndex(2);
}

void SettingDialog::on_buttonCancel_clicked()
{
    close();
}

void SettingDialog::on_buttonDefault_clicked()
{
    QMessageBox confirmMessage;
    confirmMessage.setText(tr("Восстановление стандартных настроек"));
    confirmMessage.setInformativeText(tr("Вы хотите это сделать?"));
    confirmMessage.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(confirmMessage.exec() == QMessageBox::Yes)
        settingRead("settingDefault.ini");
}

void SettingDialog::on_buttonConfirm_clicked()
{
    settingWrite("settingUser.ini");
}

void SettingDialog::on_butttonOk_clicked()
{
    settingWrite("settingUser.ini");
    close();
}

void SettingDialog::on_addPostButton_clicked()
{
    AddPostDialog *addPost = new AddPostDialog(query, this);
    addPost->exec();
    delete addPost;
    updatePostsTable();
}

void SettingDialog::on_editPostButton_clicked()
{
    qDebug() << sett->tablePosts->currentRow();
    EditPostDialog *editPost = new EditPostDialog(posts[sett->tablePosts->currentRow()].id, query, this);
    editPost->exec();
    delete editPost;
    updatePostsTable();
}

void SettingDialog::on_removePostButton_clicked()
{
    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle(tr("Удаление должности"));
    confirmMsg.setText(tr("Подтвердить удаление?"));
    QAbstractButton *confirmMsgButtonYes = confirmMsg.addButton(tr("Подтвердить"), QMessageBox::YesRole);
    QAbstractButton *confirmMsgButtonNo = confirmMsg.addButton(tr("Отмена"), QMessageBox::NoRole);
//    confirmMsg.setDefaultButton(confirmMsgButtonNo);
    confirmMsg.exec();
    if(confirmMsg.clickedButton() == confirmMsgButtonYes)
    {
        query->prepare("DELETE FROM `posts` WHERE `id_post`= :idpost");
        query->bindValue(":idpost", posts[sett->tablePosts->currentRow()].id);
        query->exec();
        updatePostsTable();
    }
}

void SettingDialog::on_tablePosts_itemSelectionChanged()
{
    sett->editPostButton->setEnabled(editPostEnabled);
    sett->actionEditPost->setEnabled(editPostEnabled);

    sett->removePostButton->setEnabled(removePostEnabled);
    sett->actionRemovePost->setEnabled(removePostEnabled);
}

void SettingDialog::on_tableAccess_itemSelectionChanged()
{
    sett->editAccessLevelButton->setEnabled(editAccessLevelEnabled);
    sett->actionEditAccessLevel->setEnabled(editAccessLevelEnabled);
}

void SettingDialog::on_editAccessLevelButton_clicked()
{
    EditAccessDialog *editAccess = new EditAccessDialog(accessLevels[sett->tableAccess->currentRow()].id, query, this);
    editAccess->exec();
    delete editAccess;
    updateAccessLevelTable();
    updatePostsTable();
}

void SettingDialog::on_tableAccess_cellDoubleClicked(int row, int column)
{
    EditAccessDialog *editAccess = new EditAccessDialog(accessLevels[row].id, query, this);
    editAccess->exec();
    delete editAccess;
    updateAccessLevelTable();
    updatePostsTable();
}

void SettingDialog::on_addHallButton_clicked()
{
    AddHallScheme *addSchemeDialog = new AddHallScheme(query, this);
    addSchemeDialog->exec();
    delete addSchemeDialog;
    updateHallsTable();
}

void SettingDialog::on_editHallButton_clicked()
{
    int indexHall = halls[sett->tableHalls->currentRow()].id;
    EditHallScheme *editDialog = new EditHallScheme(indexHall, query);
    editDialog->exec();
    delete editDialog;
    updateHallsTable();
}

void SettingDialog::on_removeHallButton_clicked()
{
    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle(tr("Удаление схемы"));
    confirmMsg.setText(tr("Подтвердить удаление?"));
    QAbstractButton *confirmMsgButtonYes = confirmMsg.addButton(tr("Подтвердить"), QMessageBox::YesRole);
    confirmMsg.addButton(tr("Отмена"), QMessageBox::NoRole);
    confirmMsg.exec();
    if(confirmMsg.clickedButton() == confirmMsgButtonYes)
    {
        query->prepare("DELETE FROM `halls` WHERE `id_hall`= :idhall");
        query->bindValue(":idhall", halls[sett->tablePosts->currentRow()].id);
        query->exec();
        updatePostsTable();
    }
}

void SettingDialog::on_tableHalls_itemSelectionChanged()
{
    sett->editHallButton->setEnabled(editHallButtonEnabled);
    sett->removeHallButton->setEnabled(removeHallButtonEnabled);
}


void SettingDialog::on_tableHalls_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(sett->actionAddScheme);
    contextTableMenu.addAction(sett->actionEditScheme);
    contextTableMenu.addAction(sett->actionRemoveScheme);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(sett->actionUpdateHalls);

    QPoint globalPos;

    globalPos = sett->tableHalls->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void SettingDialog::on_tableAccess_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(sett->actionEditAccessLevel);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(sett->actionUpdateAccessLevels);

    QPoint globalPos;

    globalPos = sett->tableAccess->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void SettingDialog::on_tablePosts_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(sett->actionAddPost);
    contextTableMenu.addAction(sett->actionEditPost);
    contextTableMenu.addAction(sett->actionRemovePost);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(sett->actionUpdatePosts);

    QPoint globalPos;

    globalPos = sett->tablePosts->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}

void SettingDialog::on_actionAddScheme_triggered()
{
    on_addHallButton_clicked();
}

void SettingDialog::on_actionEditScheme_triggered()
{
    on_editHallButton_clicked();
}

void SettingDialog::on_actionRemoveScheme_triggered()
{
    on_removeHallButton_clicked();
}

void SettingDialog::on_actionUpdateHalls_triggered()
{
    updateHallsTable();
}

void SettingDialog::on_actionEditAccessLevel_triggered()
{
    on_editAccessLevelButton_clicked();
}

void SettingDialog::on_actionUpdateAccessLevels_triggered()
{
    updateAccessLevelTable();
}

void SettingDialog::on_actionAddPost_triggered()
{
    on_addPostButton_clicked();
}

void SettingDialog::on_actionEditPost_triggered()
{
    on_editPostButton_clicked();
}

void SettingDialog::on_actionRemovePost_triggered()
{
    on_removePostButton_clicked();
}

void SettingDialog::on_actionUpdatePosts_triggered()
{
    updatePostsTable();
}

void SettingDialog::on_buttonSettingPosts_clicked()
{
    sett->stackedWidget->setCurrentIndex(3);
}
