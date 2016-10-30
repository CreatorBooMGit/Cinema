#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QCommandLinkButton>
#include <QSqlQuery>

#include "access.h"

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QSqlQuery *q, Access *accessCheck, QWidget *parent = 0);
    ~SettingDialog();

    void resizeEvent(QResizeEvent *event);
//    void closeEvent(QCloseEvent *event);

private:
    Ui::SettingDialog *sett;

    QSqlQuery *query;
    bool addHallButtonEnabled, editHallButtonEnabled, removeHallButtonEnabled;

    struct hall { int id; QString name, description; int rowCount, colCount; bool enabled; };
    struct access { int id; QString name, description; };
    struct post { int id; QString name; };

    QVector <access> accessLevels;
    QVector <post> posts;
    QVector <hall> halls;

    void settingWrite(QString filename);
    void settingRead(QString filename);

    void updateHallsTable();
    void updateAccessLevelTable();
    void updatePostsTable();

signals:
    void sendSettingsUpdate();

private slots:
    void on_buttonSettingAccessLevel_clicked();
    void on_buttonSettingEmployeeParametr_clicked();
    void on_buttonSettingSchem_clicked();
    void on_buttonSettingDataBase_clicked();
    void on_buttonCancel_clicked();
    void on_buttonDefault_clicked();
    void on_buttonConfirm_clicked();
    void on_butttonOk_clicked();
    void on_addPostButton_clicked();
    void on_editPostButton_clicked();
    void on_removePostButton_clicked();
    void on_tablePosts_itemSelectionChanged();
    void on_tableAccess_itemSelectionChanged();
    void on_editAccessLevelButton_clicked();
    void on_tableAccess_cellDoubleClicked(int row, int column);
    void on_addHallButton_clicked();
    void on_editHallButton_clicked();
    void on_removeHallButton_clicked();
    void on_tableHalls_itemSelectionChanged();
};

#endif // SETTINGDIALOG_H
