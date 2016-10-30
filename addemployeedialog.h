#ifndef ADDEMPLOYEEDIALOG_H
#define ADDEMPLOYEEDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class AddEmployeeDialog;
}

class AddEmployeeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEmployeeDialog(QSqlQuery *q, QWidget *parent = 0);
    ~AddEmployeeDialog();

private slots:
    void on_addButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::AddEmployeeDialog *ui;
    QSqlQuery *query;
    QVector <int> postsId;
};

#endif // ADDEMPLOYEEDIALOG_H
