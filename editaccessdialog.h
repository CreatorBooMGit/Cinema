#ifndef EDITACCESSDIALOG_H
#define EDITACCESSDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class EditAccessDialog;
}

class EditAccessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditAccessDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~EditAccessDialog();

private slots:
    void on_editButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::EditAccessDialog *ui;
    QSqlQuery *query;
    int idaccess;
};

#endif // EDITACCESSDIALOG_H
