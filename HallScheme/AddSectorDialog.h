#ifndef ADDSECTORDIALOG_H
#define ADDSECTORDIALOG_H

#include <QDialog>
#include "usertypes.h"

namespace Ui {
class AddSectorDialog;
}

class AddSectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSectorDialog(QWidget *parent = 0);
    ~AddSectorDialog();

signals:
    void addSector(sector sector_info);

private slots:
    void on_colorButton_clicked();
    void on_cancelButton_clicked();
    void on_addButton_clicked();

private:
    Ui::AddSectorDialog *ui;
    QColor color;
    QString name, description;
};

#endif // ADDSECTORDIALOG_H
