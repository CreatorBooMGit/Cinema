#ifndef ADDSESSIONDIALOG_H
#define ADDSESSIONDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>

namespace Ui {
class AddSessionDialog;
}

class AddSessionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSessionDialog(QSqlQuery *q, QWidget *parent = 0);
    ~AddSessionDialog();

private slots:
    void on_hallComboBox_currentIndexChanged(int index);
    void on_hallComboBox_activated(int index);
    void on_addButton_clicked();
    void on_cancelButton_clicked();
    void on_priceTable_itemChanged(QTableWidgetItem *item);

private:
    Ui::AddSessionDialog *ui;
    QSqlQuery *query;

    struct hall { int idHall; QString name; };
    struct film { int idFilm; QString name; };
    struct sector { int  idSector; QString name; double price; };

    QVector <hall> halls;
    QVector <film> films;
    QVector <sector> sectors;
};

#endif // ADDSESSIONDIALOG_H
