#ifndef SECTORSDIALOG_H
#define SECTORSDIALOG_H

#include <QDialog>
#include "usertypes.h"

namespace Ui {
class SectorsDialog;
}

class SectorsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SectorsDialog(QVector <sector> *_sectors, QWidget *parent = 0);
    ~SectorsDialog();

private slots:
    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::SectorsDialog *ui;
    QVector <sector> *sectors;
};

#endif // SECTORSDIALOG_H
