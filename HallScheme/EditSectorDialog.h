#ifndef EDITSECTORDIALOG_H
#define EDITSECTORDIALOG_H

#include <QDialog>
#include "usertypes.h"

namespace Ui {
class EditSectorDialog;
}

class EditSectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditSectorDialog(sector *s, QWidget *parent = 0);
    ~EditSectorDialog();

private slots:
    void on_editButton_clicked();
    void on_cancelButton_clicked();
    void on_colorButton_clicked();

private:
    Ui::EditSectorDialog *ui;
    sector *sector_info;
    QColor color;
    QString name, description;
};

#endif // EDITSECTORDIALOG_H
