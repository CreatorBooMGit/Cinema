#ifndef SHOWSESSIONDIALOG_H
#define SHOWSESSIONDIALOG_H

#include <QDialog>
#include <QSqlQuery>

#include <HallScheme/SettingHallQml.h>

namespace Ui {
class ShowSessionDialog;
}

class ShowSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowSessionDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~ShowSessionDialog();

public slots:
    void getCheckedPlace(int s_g_place, int s_row, int s_sectorIndex);
    void getUncheckedPlace(int s_g_place, int s_row, int s_sectorIndex);

private:
    Ui::ShowSessionDialog *ui;
    QSqlQuery *query;
    SettingHallQML *hallCore;

    int indexSession;

    struct ticket {
        int g_place;
        int row;
        int sectorIndex;
    };

    QVector <ticket> tickets;
    QVector <sector> sectors;

    void updateSectors();

};

#endif // SHOWSESSIONDIALOG_H
