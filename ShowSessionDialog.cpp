#include "ShowSessionDialog.h"
#include "ui_ShowSessionDialog.h"

#include <QMessageBox>
#include <QQmlContext>
#include <QQuickView>
#include <QDebug>
#include <qtrpt.h>

ShowSessionDialog::ShowSessionDialog(user *info, int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), indexSession(id), query(q), infoUser(info),
    ui(new Ui::ShowSessionDialog)
{
    ui->setupUi(this);

    hallCore = new SettingHallQML(this);

    connect(hallCore, SIGNAL(checkedPlace(int,int,int,int,int)), this, SLOT(getCheckedPlace(int,int,int,int,int)));
    connect(hallCore, SIGNAL(uncheckedPlace(int,int)), this, SLOT(getUncheckedPlace(int,int)));

    QQuickView *view = new QQuickView;
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    QQmlContext *context = view->rootContext();
    QWidget *container = QWidget::createWindowContainer(view, NULL);
    context->setContextProperty("hallCore", hallCore);
    view->setSource(QUrl("qrc:/qml/Scheme.qml"));

    ui->containerQml->addWidget(container);

    hallCore->setSettingMode(false);
    hallCore->setVisibleUnsignedPlaces(false);

//    ####################### Information about session ######################
    query->prepare("SELECT sessions.*, films.name FROM sessions, films "
                   "WHERE sessions.film = films.id_film AND id_session = :session");
    query->bindValue(":session", indexSession);
    query->exec();

    query->next();

    int indexHall = query->value("hall").toInt();
    nameFilm = query->value("name").toString();
    ui->SessionName->setText("<html><head/><body><p><span style=\" font-size:16pt; color:#00007f;\">" + nameFilm + "</span></p></body></html>");

    ui->dateSessionEdit->setDate(QDate::fromString(query->value("date").toString(), "yyyy-MM-dd"));
    ui->timeSessionEdit->setTime(QTime::fromString(query->value("time").toString(), "HH:mm:ss"));
    dateSession = query->value("date").toDate().toString("dd.MM.yyyy");
    timeSession = query->value("time").toTime().toString("HH:mm:ss");

// ##################### Information from table @halls@ #########################
    query->prepare("SELECT * FROM halls "
                   "WHERE id_hall = :hall");
    query->bindValue(":hall", indexHall);
    query->exec();

    query->next();

    nameHall = query->value("name").toString();
    hallCore->setSizeHall(query->value("rowCount").toInt(), query->value("columnCount").toInt());

//    ################### Information about sectors ##########################
    query->prepare("SELECT sectors_halls.id_sector, sectors_halls.name, sectors_halls.description, sectors_halls.color_places, price_of_tickets.price "
                   "FROM sectors_halls, price_of_tickets "
                   "WHERE sectors_halls.id_sector = price_of_tickets.sector AND price_of_tickets.session = :session AND sectors_halls.hall = :hall");
    query->bindValue(":session", indexSession);
    query->bindValue(":hall", indexHall);
    qDebug() << query->exec();

    while(query->next())
    {
        hallCore->addSector(query->value("id_sector").toInt(), query->value("price").toString(), query->value("name").toString(), query->value("color_places").toString());
        sectorsPrice.push_back(query->value("price").toDouble());
    }

    sectors = hallCore->getSectors();
    for(int i = 0; i < sectors.size(); i++)
    {
        query->prepare("SELECT * FROM places "
                       "WHERE sector = :sector");
        query->bindValue(":sector", sectors[i].id);
        query->exec();

        while(query->next())
            hallCore->setPlace(query->value("id_place").toInt(), query->value("place").toInt(), query->value("g_place").toInt(), query->value("row").toInt(), i);

    }
    hallCore->sendPlaces();

    query->prepare("SELECT tickets.id_ticket, tickets.session, tickets.status, tickets.place, places.sector, places.place, places.g_place, places.row "
                   "FROM tickets, places "
                   "WHERE tickets.place = places.id_place AND tickets.session = :session");
    query->bindValue(":session", indexSession);
    if(query->exec())
    {
        while(query->next())
        {
            hallCore->setStatusPlace(query->value("g_place").toInt(), query->value("row").toInt(), query->value("status").toInt());
        }
    }
    hallCore->sendStatus();

//    ##################################### Information about Cinema #########################################
    query->prepare("SELECT * "
                   "FROM cinema_information");
    query->exec();
    query->next();

    cinemaName = query->value("cinemaName").toString();
    cinemaAddress = query->value("cinemaAddress").toString();
    cinemaPhone = query->value("cinemaPhone").toString();
}

ShowSessionDialog::~ShowSessionDialog()
{
    delete ui;
}

void ShowSessionDialog::getCheckedPlace(int id_place, int place, int g_place, int row, int sectorId)
{
    ticket tmp;
    tmp.id_place = id_place;
    tmp.place = place;
    tmp.g_place = g_place;
    tmp.row = row;
    tmp.sectorId = sectorId;
    tickets.push_back(tmp);
    ui->ticketsList->addItem(tr("Ряд: %1 Місце: %2").arg(row).arg(place));
}

void ShowSessionDialog::getUncheckedPlace(int s_g_place, int s_row)
{
    for(int i = 0; i < tickets.size(); i++)
        if(tickets[i].g_place == s_g_place && tickets[i].row == s_row)
        {
            tickets.remove(i);
            ui->ticketsList->takeItem(i);
            return;
        }
}

void ShowSessionDialog::on_cancelButton_clicked()
{
    close();
}

void ShowSessionDialog::closeEvent(QCloseEvent *event)
{
    if(tickets.size() > 0)
    {
        QMessageBox msgConfig;
        msgConfig.setWindowTitle(tr("Отмена"));
        msgConfig.setText(tr("Подтвердить отмену?"));
        QAbstractButton *confirmMsgButtonYes = msgConfig.addButton(tr("Да"), QMessageBox::YesRole);
        msgConfig.addButton(tr("Нет"), QMessageBox::NoRole);
        msgConfig.exec();
        if(msgConfig.clickedButton() == confirmMsgButtonYes)
        {
            event->accept();
        }
        else
            event->ignore();
    }
    else
        event->accept();
}

void ShowSessionDialog::on_buyButton_clicked()
{
    if(tickets.size() <= 0) return;

    for(int i = 0; i < tickets.size(); i++)
    {
        query->prepare("UPDATE `tickets` "
                       "SET `status` = :status "
                       "WHERE `session` = :session AND `place` = :place");
        query->bindValue(":session", indexSession);
        query->bindValue(":place", tickets[i].id_place);
        query->bindValue(":status", HallQml::StatusSold);
        query->exec();

        query->prepare("SELECT `tickets`.`id_ticket` "
                       "FROM `tickets` "
                       "WHERE `tickets`.`session` = :session AND `tickets`.`place` = :place");
        query->bindValue(":session", indexSession);
        query->bindValue(":place", tickets[i].id_place);
        query->exec();
        query->next();

        tickets[i].id_ticket = query->value("id_ticket").toInt();

        query->prepare("INSERT INTO `traffic_tickets` (`date`, `time`, `ticket`, `operation`, `employee`) "
                       "VALUES (:date, :time, :ticket, :operation, :employee)");
        query->bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));
        query->bindValue(":time", QTime::currentTime().toString("HH:mm:ss"));
        query->bindValue(":ticket", tickets[i].id_ticket);
        query->bindValue(":operation", TicketOperations::operationSold);
        query->bindValue(":employee", infoUser->idlogin);
        query->exec();
    }

    QtRPT *ticket = new QtRPT(this);
    ticket->recordCount << tickets.size();
    ticket->loadReport(":/reports/reports/ticket.xml");

    connect(ticket, &QtRPT::setValue, [&](const int recNo,
            const QString paramName, QVariant &paramValue,
            const int reportPage) {
        (void) reportPage;
        if(paramName == "ticket") {
            paramValue = tickets[recNo].id_ticket;
        }
        if(paramName == "film") {
            paramValue = nameFilm;
        }
        if(paramName == "hall") {
            paramValue = nameHall;
        }
        if(paramName == "date") {
            paramValue = dateSession;
        }
        if(paramName == "time") {
            paramValue = timeSession;
        }
        if(paramName == "row") {
            paramValue = tickets[recNo].row;
        }
        if(paramName == "place") {
            paramValue = tickets[recNo].place;
        }
        if(paramName == "price") {
            paramValue = sectorsPrice[tickets[recNo].sectorId - 1];
        }
        if(paramName == "cinemaName") {
            paramValue = cinemaName;
        }
        if(paramName == "cinemaAddress") {
            paramValue = cinemaAddress;
        }
        if(paramName == "cinemaPhone") {
            paramValue = cinemaPhone;
        }
    });

    ticket->printExec();

    reject();
}
