#ifndef EDITPOSTDIALOG_H
#define EDITPOSTDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class EditPostDialog;
}

class EditPostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditPostDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~EditPostDialog();

private slots:
    void on_editButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::EditPostDialog *ui;
    QSqlQuery *query;
    int idpost;

    struct access { int id; QString name, description; };

    QVector <access> accessLevels;
};

#endif // EDITPOSTDIALOG_H
