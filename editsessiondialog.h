#ifndef EDITSESSIONDIALOG_H
#define EDITSESSIONDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QTableWidget>


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
    void on_hallComboBox_activated(int index);
    void on_priceTable_itemChanged(QTableWidgetItem *item);

private:
    Ui::EditSessionDialog *ui;
    QSqlQuery *query;
    int id_session;
    bool updatePrice = false;

    struct hall { int idHall; QString name; };
    struct film { int idFilm; QString name; };
    struct sector { int  idSector; QString name; double price; };

    QVector <hall> halls;
    QVector <film> films;
    QVector <sector> sectors;
};

#endif // EDITSESSIONDIALOG_H
