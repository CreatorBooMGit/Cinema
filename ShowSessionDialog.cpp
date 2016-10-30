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

    ui->dateSessionEdit->setDate(QDate::fromString(query->value("date").toString(), "yyyy-MM-dd"));

// ##################### Information from table @halls@ #########################
    query->prepare("SELECT * FROM halls "
                   "WHERE id_hall = :hall");
    query->bindValue(":hall", indexHall);
    query->exec();

    query->next();
    hallCore->setSizeHall(query->value("rowCount").toInt(), query->value("columnCount").toInt());

//    ################### Information about sectors ##########################
    query->prepare("SELECT * FROM sectors_halls "
                   "WHERE hall = :hall");
    query->bindValue(":hall", indexHall);
    query->exec();

    while(query->next())
        hallCore->addSector(query->value("id_sector").toInt(), query->value("name").toString(), query->value("description").toString(), query->value("color_places").toString());

    sectors(hallCore->getSectors());
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

    query->prepare("SELECT tickets.session, tickets.status, tickets.place, places.sector, places.place, places.g_place, places.row "
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

void ShowSessionDialog::getCheckedPlace(int s_g_place, int s_row, int s_sectorIndex)
{

}

void ShowSessionDialog::getUncheckedPlace(int s_g_place, int s_row, int s_sectorIndex)
{

}
