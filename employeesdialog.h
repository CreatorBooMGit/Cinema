#ifndef EMPLOYEESDIALOG_H
#define EMPLOYEESDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include "access.h"

namespace Ui {
class EmployeesDialog;
}

class EmployeesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeesDialog(QSqlQuery *q, Access *access, QWidget *parent = 0);
    ~EmployeesDialog();

private slots:
    void on_addEmployeeButton_clicked();
    void on_editEmployeeButton_clicked();
    void on_removeEmployeeButton_clicked();

    void on_employeesTable_itemSelectionChanged();

    void on_employeesTable_customContextMenuRequested(const QPoint &pos);

    void on_actionAddEmployee_triggered();

    void on_actionEditEmployee_triggered();

    void on_actionRemoveEmployee_triggered();

    void on_actionUpdateEmployees_triggered();

private:
    Ui::EmployeesDialog *ui;
    QSqlQuery *query;

    bool addButtonEnabled, editButtonEnabled, removeButtonEnabled;

    QVector <int> employeesId;

    void updateEmployeesTable();
};

#endif // EMPLOYEESDIALOG_H
