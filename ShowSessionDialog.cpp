#include "ShowSessionDialog.h"
#include "ui_ShowSessionDialog.h"

#include <QMessageBox>
#include <QQmlContext>
#include <QQuickView>
#include <QDebug>

ShowSessionDialog::ShowSessionDialog(int id, QSqlQuery *q, QWidget *parent) :
    QDialog(parent), indexSession(id), query(q),
    ui(new Ui::ShowSessionDialog)
{
    ui->setupUi(this);

    hallCore = new SettingHallQML(this);

    connect(hallCore, SIGNAL(checkedPlace(int,int,int)), this, SLOT(getCheckedPlace(int,int,int)));
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
    QString nameFilm = query->value("name").toString();
    ui->SessionName->setText("<html><head/><body><p><span style=\" font-size:16pt; color:#00007f;\">" + nameFilm + "</span></p></body></html>");

    ui->dateSessionEdit->setDate(QDate::fromString(query->value("date").toString(), "yyyy-MM-dd"));

// ##################### Information from table @halls@ #########################
    query->prepare("SELECT * FROM halls "
                   "WHERE id_hall = :hall");
    query->bindValue(":hall", indexHall);
    query->exec();

    query->next();
    hallCore->setSizeHall(query->value("rowCount").toInt(), query->value("columnCount").toInt());

//    ################### Information about sectors ##########################
    query->prepare("SELECT sectors_halls.id_sector, sectors_halls.name, sectors_halls.description, sectors_halls.color_places, price_of_tickets.price "
                   "FROM sectors_halls, price_of_tickets "
                   "WHERE sectors_halls.id_sector = price_of_tickets.sector AND price_of_tickets.session = :session AND sectors_halls.hall = :hall");
    query->bindValue(":session", indexSession);
    query->bindValue(":hall", indexHall);
    query->exec();

    while(query->next())
        hallCore->addSector(query->value("id_sector").toInt(), query->value("price").toString(), query->value("name").toString(), query->value("color_places").toString());

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
}

ShowSessionDialog::~ShowSessionDialog()
{
    delete ui;
}

void ShowSessionDialog::getCheckedPlace(int id_place, int g_place, int row)
{
    ticket tmp;
    tmp.id_place = id_place;
    tmp.g_place = g_place;
    tmp.row = row;
    tickets.push_back(tmp);
    ui->ticketsList->addItem(tr("Ряд: %1 Місце: %2").arg(row).arg(g_place));
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
        query->prepare("INSERT INTO `tickets` (`session`, `place`, `status`) "
                       "VALUES (:session, :place, :status)");
        query->bindValue(":session", indexSession);
        query->bindValue(":place", tickets[i].id_place);
        query->bindValue(":status", HallQml::StatusSold);
        query->exec();
    }
    reject();
}
