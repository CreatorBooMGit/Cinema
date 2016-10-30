#include "SectorsDialog.h"
#include "ui_SectorsDialog.h"
#include <QDebug>
#include "HallScheme/usertypes.h"

SectorsDialog::SectorsDialog(QVector<sector> *_sectors, QWidget *parent) :
    QDialog(parent), sectors(_sectors),
    ui(new Ui::SectorsDialog)
{
    ui->setupUi(this);
//    setFixedSize(400, 300);
//    for(int i = 0; i < sectors->size(); i++)
//        ui->listWidget->addItem(sectors[i]);
}

SectorsDialog::~SectorsDialog()
{
    delete ui;
}

void SectorsDialog::on_listWidget_doubleClicked(const QModelIndex &index)
{
    qDebug() << index;
}
