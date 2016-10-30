#ifndef SHOWINFORMATIONSESSION_H
#define SHOWINFORMATIONSESSION_H

#include <QDialog>
#include <QSqlQuery>

#include "HallScheme/SettingHallQml.h"

namespace Ui {
class ShowInformationSession;
}

class ShowInformationSession : public QDialog
{
    Q_OBJECT

public:
    explicit ShowInformationSession(int id, QSqlQuery *q, QWidget *parent = 0);
    ~ShowInformationSession();

private:
    Ui::ShowInformationSession *ui;
    QSqlQuery *query;
    SettingHallQML *hallCore;
    int indexSession;
};

#endif // SHOWINFORMATIONSESSION_H
