#ifndef EDITFILMDIALOG_H
#define EDITFILMDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class EditFilmDialog;
}

class EditFilmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditFilmDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~EditFilmDialog();

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::EditFilmDialog *ui;
    QSqlQuery *query;
    int filmId;
};

#endif // EDITFILMDIALOG_H
