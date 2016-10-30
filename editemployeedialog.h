#ifndef EDITEMPLOYEEDIALOG_H
#define EDITEMPLOYEEDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class EditEmployeeDialog;
}

class EditEmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditEmployeeDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~EditEmployeeDialog();

private slots:
    void on_editButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::EditEmployeeDialog *ui;
    QSqlQuery *query;
    QVector <int> postsId;
    int employeeId;

};

#endif // EDITEMPLOYEEDIALOG_H
