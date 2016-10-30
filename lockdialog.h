#ifndef LOCKDIALOG_H
#define LOCKDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QSqlQuery>

namespace Ui {
class LockDialog;
}

class LockDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LockDialog(QString *log, QSqlQuery *q, QWidget *parent = 0);
    ~LockDialog();

    void closeEvent(QCloseEvent *event);

signals:
    void sendChangeUser();
    void sendCloseDialog();

private slots:
    void on_changeButton_clicked();

    void on_unlockButton_clicked();

private:
    Ui::LockDialog *ui;
    QSqlQuery *query;
    QString *login;
};

#endif // LOCKDIALOG_H
