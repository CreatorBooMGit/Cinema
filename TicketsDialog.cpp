#include "TicketsDialog.h"
#include "ui_TicketsDialog.h"

TicketsDialog::TicketsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TicketsDialog)
{
    ui->setupUi(this);
}

TicketsDialog::~TicketsDialog()
{
    delete ui;
}
