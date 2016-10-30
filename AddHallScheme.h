#ifndef ADDHALLSCHEME_H
#define ADDHALLSCHEME_H

#include <QDialog>
#include <QSqlQuery>

#include "HallScheme/SettingHallQml.h"
#include "HallScheme/usertypes.h"

namespace Ui {
class AddHallScheme;
}

class AddHallScheme : public QDialog
{
    Q_OBJECT

public:
    explicit AddHallScheme(QSqlQuery *q, QWidget *parent = 0);
    ~AddHallScheme();

private slots:
    void on_rowCountSpinBox_valueChanged(int arg1);
    void on_columnCountSpinBox_valueChanged(int arg1);

    void on_addSectorButton_clicked();
    void on_editSectorButton_clicked();
    void on_removeSectorButton_clicked();

    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::AddHallScheme *ui;
    QSqlQuery *query;
    SettingHallQML *hallCore;

    void updateSectors();

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
};

#endif // ADDHALLSCHEME_H
