#include "EditSectorDialog.h"
#include "ui_EditSectorDialog.h"
#include "usertypes.h"
#include <QColor>
#include <QColorDialog>

EditSectorDialog::EditSectorDialog(sector *s, QWidget *parent) :
    QDialog(parent), sector_info(s),
    ui(new Ui::EditSectorDialog)
{
    ui->setupUi(this);
    QPalette palette = ui->colorEdit->palette();
    QColor tmp_color;
    tmp_color.setNamedColor(sector_info->color);
    palette.setColor(QPalette::Base, tmp_color);
    ui->colorEdit->setPalette(palette);
    ui->colorEdit->setText(tmp_color.name());
    ui->nameEdit->setText(sector_info->text);
    ui->descriptionEdit->setText(sector_info->description);
}

EditSectorDialog::~EditSectorDialog()
{
    delete ui;
}

void EditSectorDialog::on_editButton_clicked()
{
    if(ui->descriptionEdit->text().isEmpty()) ui->descriptionEdit->setText(ui->nameEdit->text());

    sector_info->text = ui->nameEdit->text();
    sector_info->description = ui->descriptionEdit->text();
    sector_info->color = color.name();
    sector_info->status = HallQml::StatusUpdate;
    close();
}

void EditSectorDialog::on_cancelButton_clicked()
{
    close();
}

void EditSectorDialog::on_colorButton_clicked()
{
    color = QColorDialog::getColor(ui->colorEdit->palette().color(QPalette::Base));
    QPalette palette = ui->colorEdit->palette();
    palette.setColor(QPalette::Base, color);
    ui->colorEdit->setPalette(palette);
    ui->colorEdit->setText(color.name());
}
