#ifndef EDITSESSIONDIALOG_H
#define EDITSESSIONDIALOG_H

#include <QDialog>
#include <QSqlQuery>


namespace Ui {
class EditSessionDialog;
}

class EditSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditSessionDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~EditSessionDialog();

private slots:
    void on_cancelButton_clicked();

    void on_editButton_clicked();

private:
    Ui::EditSessionDialog *ui;
    QSqlQuery *query;
    int id_session;
    struct hall { int idHall; QString name; };
    struct film { int idFilm; QString name; };
    QVector <hall> halls;
    QVector <film> films;
};

#endif // EDITSESSIONDIALOG_H
