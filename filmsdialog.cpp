#include "filmsdialog.h"
#include "ui_filmsdialog.h"
#include "addfilmdialog.h"
#include "editfilmdialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QMenu>

FilmsDialog::FilmsDialog(QSqlQuery *q, Access *access, QWidget *parent) :
    QDialog(parent), query(q),
    ui(new Ui::FilmsDialog)
{
    ui->setupUi(this);

    addButtonEnabled = access->checkAccess("addFilmEnabled");
    editButtonEnabled = access->checkAccess("editFilmEnabled");
    removeButtonEnabled = access->checkAccess("removeFilmEnabled");

    ui->addButton->setEnabled(addButtonEnabled);
    ui->actionAddPost->setEnabled(addButtonEnabled);
//    if(!editButtonEnabled)
//    {
//        ui->editButton->setEnabled(editButtonEnabled);
//        ui->actionEditFilm->setEnabled(editButtonEnabled);
//    }
//    if(!removeButtonEnabled)
//    {
//        ui->removeButton->setEnabled(removeButtonEnabled);
//        ui->actionRemoveFilm->setEnabled(removeButtonEnabled);
//    }

    updateTable();
}

FilmsDialog::~FilmsDialog()
{
    delete ui;
}

void FilmsDialog::resizeEvent(QResizeEvent *e)
{
    for(int i = 0; i < ui->filmsTable->columnCount(); i++)
        ui->filmsTable->setColumnWidth(i, ui->filmsTable->size().width() / ui->filmsTable->columnCount());
}

void FilmsDialog::on_addButton_clicked()
{
    AddFilmDialog *addFilm = new AddFilmDialog(query, this);
    addFilm->exec();
    delete addFilm;
    updateTable();
}

void FilmsDialog::on_editButton_clicked()
{
    if(ui->filmsTable->currentRow() < 0) return;

    EditFilmDialog *editFilm = new EditFilmDialog(id[ui->filmsTable->currentRow()], query, this);
    editFilm->exec();
    delete editFilm;
    updateTable();
}

void FilmsDialog::on_removeButton_clicked()
{
    if(ui->filmsTable->currentRow() < 0) return;

    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle(tr("Удаление фильма"));
    confirmMsg.setText(tr("Подтвердить удаление?"));
    QAbstractButton *confirmMsgButtonYes = confirmMsg.addButton(tr("Подтвердить"), QMessageBox::YesRole);
    confirmMsg.addButton(tr("Отмена"), QMessageBox::NoRole);
//    confirmMsg.setDefaultButton(confirmMsgButtonNo);
    confirmMsg.exec();
    if(confirmMsg.clickedButton() == confirmMsgButtonYes)
    {
        query->prepare("DELETE FROM `films` WHERE `id_film`=:id");
        query->bindValue(":id", id[ui->filmsTable->currentRow()]);
        query->exec();
        updateTable();
    }
}

FilmsDialog::updateTable()
{
    id.clear();
    query->prepare("SELECT * FROM films");
    query->exec();
    int count = query->size();
    int current = 0;
    ui->filmsTable->setRowCount(count);
    while(query->next())
    {
        id.push_back(query->value("id_film").toInt());
        for(int i = 1; i < 6; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(query->value(i).toString());
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            ui->filmsTable->setItem(current, i-1, item);
        }
        current++;
    }
}

void FilmsDialog::on_actionAddPost_triggered()
{
    on_addButton_clicked();
}

void FilmsDialog::on_actionEditFilm_triggered()
{
    on_editButton_clicked();
}

void FilmsDialog::on_actionRemoveFilm_triggered()
{
    on_removeButton_clicked();
}

void FilmsDialog::on_actionUpdateFilms_triggered()
{
    updateTable();
}

void FilmsDialog::on_filmsTable_itemSelectionChanged()
{
    ui->editButton->setEnabled(editButtonEnabled);
    ui->actionEditFilm->setEnabled(editButtonEnabled);

    ui->removeButton->setEnabled(removeButtonEnabled);
    ui->actionRemoveFilm->setEnabled(removeButtonEnabled);
}

void FilmsDialog::on_filmsTable_doubleClicked(const QModelIndex &index)
{
    if(editButtonEnabled)
        on_editButton_clicked();
}

void FilmsDialog::on_filmsTable_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddPost);
    contextTableMenu.addAction(ui->actionEditFilm);
    contextTableMenu.addAction(ui->actionRemoveFilm);
    contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdateFilms);

    QPoint globalPos = ui->filmsTable->mapToGlobal(pos);

    contextTableMenu.exec(globalPos);
}
