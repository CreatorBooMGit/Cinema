#ifndef SETTINGSCHEMHALLDIALOG_H
#define SETTINGSCHEMHALLDIALOG_H

#include <QDialog>

namespace Ui {
class settingSchemHallDialog;
}

class settingSchemHallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingSchemHallDialog(QWidget *parent = 0);
    ~settingSchemHallDialog();

private:
    Ui::settingSchemHallDialog *hall;

private slots:
    void buttonCancelClicked();
};

#endif // SETTINGSCHEMHALLDIALOG_H
