#include "TicketInfoDialog.h"
#include "ui_TicketInfoDialog.h"

#include <QDate>
#include <QMessageBox>
#include <QTime>

#include <HallScheme/usertypes.h>

TicketInfoDialog::TicketInfoDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), ticketId(id), query(q),
    ui(new Ui::TicketInfoDialog)
{
    ui->setupUi(this);

    query->prepare("SELECT status_tickets.idstatus AS idstatus, status_tickets.name AS status, films.name AS film, halls.name AS hall, places.row, places.place, sessions.date, sessions.time "
                   "FROM status_tickets, films, halls, places, tickets, sessions "
                   "WHERE status_tickets.idstatus = tickets.status "
                   "AND films.id_film = sessions.film "
                   "AND halls.id_hall = sessions.hall "
                   "AND places.id_place = tickets.place "
                   "AND tickets.session = sessions.id_session "
                   "AND tickets.id_ticket = :ticketId");
    query->bindValue(":ticketId", ticketId);
    query->exec();

    query->next();
    ui->ticketNumberText->setText(tr("<html><head/><body><p><span style=\" font-weight:600;\">%1</span></p></body></html>").arg(ticketId));

    if(query->value("idstatus").toInt() == HallQml::StatusFree)
        ui->statusText->setText(tr("<html><head/><body><p><span style=\" font-weight:600; color:#00ff00;\">%1</span></p></body></html>").arg(query->value("status").toString()));
    else
        if(query->value("idstatus").toInt() == HallQml::StatusSold)
            ui->statusText->setText(tr("<html><head/><body><p><span style=\" font-weight:600; color:#ff0000;\">%1</span></p></body></html>").arg(query->value("status").toString()));
    else
            if(query->value("idstatus").toInt() == HallQml::StatusBooking)
                ui->statusText->setText(tr("<html><head/><body><p><span style=\" font-weight:600; color:#ffaa00;\">%1</span></p></body></html>").arg(query->value("status").toString()));

    ui->filmText->setText(tr("<html><head/><body><p><span style=\" font-weight:600;\">%1</span></p></body></html>").arg(query->value("film").toString()));
    ui->hallText->setText(tr("<html><head/><body><p><span style=\" font-weight:600;\">%1</span></p></body></html>").arg(query->value("hall").toString()));
    ui->rowText->setText(tr("<html><head/><body><p><span style=\" font-weight:600;\">%1</span></p></body></html>").arg(query->value("row").toString()));
    ui->placeText->setText(tr("<html><head/><body><p><span style=\" font-weight:600;\">%1</span></p></body></html>").arg(query->value("place").toString()));
    ui->dateText->setText(tr("<html><head/><body><p><span style=\" font-weight:600;\">%1</span></p></body></html>").arg(query->value("date").toDate().toString("dd.MM.yyyy")));
    ui->timeText->setText(tr("<html><head/><body><p><span style=\" font-weight:600;\">%1</span></p></body></html>").arg(query->value("time").toTime().toString("hh:mm")));

    query->prepare("SELECT date, time, operations.name AS operation "
                   "FROM traffic_tickets, operations "
                   "WHERE traffic_tickets.operation = operations.idoperation AND traffic_tickets.ticket = :ticket");
    query->bindValue(":ticket", ticketId);
    query->exec();
    ui->operationsTable->setRowCount(query->size());
    int current = 0;
    while(query->next())
    {
        QTableWidgetItem *dateItem = new QTableWidgetItem;
        QTableWidgetItem *timeItem = new QTableWidgetItem;
        QTableWidgetItem *operationItem = new QTableWidgetItem;

        dateItem->setText(query->value("date").toDate().toString("dd.MM.yyyy"));
        timeItem->setText(query->value("time").toTime().toString("hh:mm"));
        operationItem->setText(query->value("operation").toString());

        dateItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        timeItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        operationItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        ui->operationsTable->setItem(current, 0, dateItem);
        ui->operationsTable->setItem(current, 1, timeItem);
        ui->operationsTable->setItem(current, 2, operationItem);

        current++;
    }
}

TicketInfoDialog::~TicketInfoDialog()
{
    delete ui;
}
