#ifndef ADDFILMDIALOG_H
#define ADDFILMDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class AddFilmDialog;
}

class AddFilmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFilmDialog(QSqlQuery *q, QWidget *parent = 0);
    ~AddFilmDialog();

signals:


private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::AddFilmDialog *ui;
    QSqlQuery *query;
};

#endif // ADDFILMDIALOG_H
