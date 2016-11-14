#ifndef AUTHENTIFICATIONUSERDIALOG_H
#define AUTHENTIFICATIONUSERDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QCloseEvent>
#include "usertype.h"

namespace Ui {
class AuthentificationUserDialog;
}

class AuthentificationUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthentificationUserDialog(user *info, QSqlQuery *q, QWidget *parent = 0);
    ~AuthentificationUserDialog();

    void closeEvent(QCloseEvent *event);

    void setExitOnCancel(bool value);

    void setClickedCancel(bool value);
    bool getClickedCancel() const;

private slots:
    void on_closeButton_clicked();
    void on_authButton_clicked();

private:
    Ui::AuthentificationUserDialog *ui;
    QSqlQuery *query;
    user *infoUser;

    bool exitOnCancel = true;
    bool clickedCancel = false;
};

#endif // AUTHENTIFICATIONUSERDIALOG_H
