#ifndef ADDPOSTDIALOG_H
#define ADDPOSTDIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class AddPostDialog;
}

class AddPostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPostDialog(QSqlQuery *q, QWidget *parent = 0);
    ~AddPostDialog();

private slots:
    void on_addButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::AddPostDialog *ui;
    QSqlQuery *query;

    struct access { int id; QString name, description; };

    QVector <access> accessLevels;
};

#endif // ADDPOSTDIALOG_H
