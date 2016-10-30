#ifndef EDITHALLSCHEME_H
#define EDITHALLSCHEME_H

#include <QDialog>
#include <QSqlQuery>

#include <HallScheme/SettingHallQml.h>

namespace Ui {
class EditHallScheme;
}

class EditHallScheme : public QDialog
{
    Q_OBJECT

public:
    explicit EditHallScheme(int id, QSqlQuery *q, QWidget *parent = 0);
    ~EditHallScheme();

private slots:
    void on_rowCountSpinBox_valueChanged(int arg1);
    void on_columnCountSpinBox_valueChanged(int arg1);

    void on_addSectorButton_clicked();
    void on_editSectorButton_clicked();
    void on_removeSectorButton_clicked();

    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

    void on_sectorsList_itemSelectionChanged();

private:
    Ui::EditHallScheme *ui;
    QSqlQuery *query;
    SettingHallQML *hallCore;
    int indexHall;

    void updateSectors();

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // EDITHALLSCHEME_H
