#ifndef TICKETINFODIALOG_H
#define TICKETINFODIALOG_H

#include <QDialog>
#include <QSqlQuery>

namespace Ui {
class TicketInfoDialog;
}

class TicketInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TicketInfoDialog(int id, QSqlQuery *q, QWidget *parent = 0);
    ~TicketInfoDialog();

private:
    Ui::TicketInfoDialog *ui;
    QSqlQuery *query;
    int ticketId;
};

#endif // TICKETINFODIALOG_H
