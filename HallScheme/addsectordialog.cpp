#include "AddSectorDialog.h"
#include "ui_AddSectorDialog.h"
#include <QColorDialog>
#include "usertypes.h"

AddSectorDialog::AddSectorDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::AddSectorDialog)
{
    ui->setupUi(this);
    setFixedSize(250, 130);
}

AddSectorDialog::~AddSectorDialog()
{
    delete ui;
}

void AddSectorDialog::on_colorButton_clicked()
{
    color = QColorDialog::getColor(ui->colorEdit->palette().color(QPalette::Base));
    QPalette palette = ui->colorEdit->palette();
    palette.setColor(QPalette::Base, color);
    ui->colorEdit->setPalette(palette);
    ui->colorEdit->setText(color.name());
}

void AddSectorDialog::on_cancelButton_clicked()
{
    close();
}

void AddSectorDialog::on_addButton_clicked()
{
    if(ui->descriptionEdit->text().isEmpty()) ui->descriptionEdit->setText(ui->nameEdit->text());

    sector tmp;
    tmp.text = ui->nameEdit->text();
    tmp.description = ui->descriptionEdit->text();
    tmp.color = color.name();
    tmp.status = HallQml::StatusNew;
    emit addSector(tmp);
    close();
}
