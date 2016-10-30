#ifndef FILMSDIALOG_H
#define FILMSDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QVector>
#include "access.h"

namespace Ui {
class FilmsDialog;
}

class FilmsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilmsDialog(QSqlQuery *q, Access *access, QWidget *parent = 0);
    ~FilmsDialog();
    void resizeEvent(QResizeEvent *e);

private slots:
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_removeButton_clicked();
    void on_actionAddPost_triggered();
    void on_actionEditFilm_triggered();
    void on_actionRemoveFilm_triggered();
    void on_actionUpdateFilms_triggered();
    void on_filmsTable_itemSelectionChanged();
    void on_filmsTable_doubleClicked(const QModelIndex &index);
    void on_filmsTable_customContextMenuRequested(const QPoint &pos);

private:
    Ui::FilmsDialog *ui;
    QSqlQuery *query;
    QVector <int> id;

    bool addButtonEnabled, editButtonEnabled, removeButtonEnabled;

    updateTable();
};

#endif // FILMSDIALOG_H
