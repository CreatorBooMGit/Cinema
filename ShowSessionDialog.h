#ifndef SHOWSESSIONDIALOG_H
#define SHOWSESSIONDIALOG_H

#include "usertype.h"

#include <QDialog>
#include <QSqlQuery>

#include <HallScheme/SettingHallQml.h>

namespace Ui {
class ShowSessionDialog;
}

namespace TicketOperations {
    enum { operationSold = 1, operationBooking = 2, operationCancellation = 3, operationReturn = 4};
}

class ShowSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShowSessionDialog(user *info, int id, QSqlQuery *q, QWidget *parent = 0);
    ~ShowSessionDialog();

public slots:
    void getCheckedPlace(int id_place, int place, int g_place, int row, int sectorId);
    void getUncheckedPlace(int s_g_place, int s_row);

private slots:
    void on_cancelButton_clicked();
    void on_buyButton_clicked();

private:
    Ui::ShowSessionDialog *ui;
    QSqlQuery *query;
    SettingHallQML *hallCore;

    user *infoUser;

    int indexSession;

    QString nameFilm;
    QString nameHall;
    QString dateSession, timeSession;

    QString cinemaName, cinemaAddress, cinemaPhone;

    struct ticket {
        int id_ticket = 0;
        int id_place;
        int sectorId;
        int place;
        int g_place;
        int row;
    };

    QVector <ticket> tickets;
    QVector <sector> sectors;
    QVector <double> sectorsPrice;

    void updateSectors();


    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // SHOWSESSIONDIALOG_H
