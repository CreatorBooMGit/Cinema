#ifndef SETTINGHALL_H
#define SETTINGHALL_H

#include <QDialog>

namespace Ui {
class settingHall;
}

class settingHall : public QDialog
{
    Q_OBJECT

public:
    explicit settingHall(QWidget *parent = 0);
    ~settingHall();

private:
    Ui::settingHall *ui;
};

#endif // SETTINGHALL_H
