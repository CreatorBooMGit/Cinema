#include "authentificationuserdialog.h"
#include "settingdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addfilmdialog.h"
#include "filmsdialog.h"
#include "addsessiondialog.h"
#include "editsessiondialog.h"
#include "lockdialog.h"
#include "employeesdialog.h"
#include "ShowInformationSession.h"
#include "ShowSessionDialog.h"
#include "TicketInfoDialog.h"
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QAbstractButton>

#define MYSQL 1
#define AUTH 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setEnabled(true);
    setWindowTitle(tr("Кинотеатр"));
    settings = new QSettings("settingUser.ini", QSettings::IniFormat);

    // Установка начальных значений

//    ui->tableWidget->setEditTriggers();

    QTime currentTime;
//    currentTime.setHMS(QTime::currentTime().hour() - 2, 0, 0);
    currentTime.setHMS(0, 0, 0);
    ui->dateFilterFrom->setDate(QDate::currentDate());
    ui->dateFilterBefore->setDate(QDate::currentDate().addDays(1));
    ui->timeFilterFrom->setTime(currentTime);

#if MYSQL >= 1
    db = QSqlDatabase::addDatabase("QMYSQL");

    settings->beginGroup("Database");
    db.setHostName(settings->value("host", QString()).toString());
    db.setPort(settings->value("port", QString()).toInt());
    db.setDatabaseName(settings->value("database", QString()).toString());
    db.setUserName(settings->value("username", QString()).toString());
    db.setPassword(settings->value("password", QString()).toString());
    settings->endGroup();
    qDebug() << db.database();
    qDebug() << db.open();
    query = new QSqlQuery(db);
    updateTable();
#endif

    infoUser = new user;
#if AUTH >= 1
    AuthentificationUserDialog *authDialog = new AuthentificationUserDialog(infoUser, query, this);
    authDialog->exec();
    delete authDialog;
    access = new Access(infoUser->login, query);
    updateAccess();
#endif


    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settShow()));

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onTableContextMenu(QPoint)));

    settings->beginGroup("Size");
    resize(settings->value("mainWidth", QString()).toInt(), settings->value("mainHeight", QString()).toInt());
    ui->tableWidget->resize(settings->value("tableWidth", QString()).toInt(), settings->value("tableHeight", QString()).toInt());
    settings->endGroup();

//    #################StatusBar####################
    hostStatusLabel = new QLabel(this);
    loginStatusLabel = new QLabel(this);
    dateStatusLabel = new QLabel(this);
    timeStatusLabel = new QLabel(this);

    hostStatusLabel->setMinimumWidth(120);
    hostStatusLabel->setAlignment(Qt::AlignVCenter);
    loginStatusLabel->setMinimumWidth(120);
    loginStatusLabel->setAlignment(Qt::AlignVCenter);
    dateStatusLabel->setMinimumWidth(120);
    dateStatusLabel->setAlignment(Qt::AlignVCenter);
    timeStatusLabel->setMinimumWidth(120);
    timeStatusLabel->setAlignment(Qt::AlignVCenter);

    ui->statusBar->addWidget(hostStatusLabel, 0);
    ui->statusBar->addWidget(loginStatusLabel, 0);
    ui->statusBar->addWidget(dateStatusLabel, 0);
    ui->statusBar->addWidget(timeStatusLabel, 0);

    settings->beginGroup("Database");
    hostStatusLabel->setText("<img width=\"15\" height=\"15\" src=\":/icons/icons/host.ico\"/> " + settings->value("host").toString());
    settings->endGroup();

    loginStatusLabel->setText("<img width=\"15\" height=\"15\" src=\":/icons/icons/user.ico\"/> " + infoUser->login);

    timeTimer = new QTimer(this);
    connect(timeTimer, SIGNAL(timeout()), this, SLOT(updateTimeStatusBar()));
    timeTimer->start(1000);
    updateTimeStatusBar();
}

MainWindow::~MainWindow()
{
    db.close();
    delete query;
    delete ui;

}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    settings->beginGroup("Size");
    settings->setValue("mainWidth", width());
    settings->setValue("mainHeight", height());
    settings->setValue("tableWidth", ui->tableWidget->width());
    settings->setValue("tableHeight", ui->tableWidget->height());
    settings->endGroup();

    for(int i = 0; i < ui->tableWidget->columnCount(); i++)
        ui->tableWidget->setColumnWidth(i, ui->tableWidget->size().width() / ui->tableWidget->columnCount());
}

bool MainWindow::updateTable()
{
    ui->tableWidget->clearContents();
    sessions_id.clear();

    QString tmp;
    tmp.push_back("SELECT sessions.id_session, films.name, sessions.date, sessions.time, halls.name "
                  "FROM sessions, films, halls "
                  "WHERE sessions.film = films.id_film AND sessions.hall = halls.id_hall ");
    if(ui->checkFrom->isChecked())
        tmp.push_back("AND sessions.date >= :dateFrom AND sessions.time >= :timeFrom ");
    if(ui->checkBefore->isChecked())
        tmp.push_back("AND sessions.date <= :dateBefore AND sessions.time <= :timeBefore ");
    tmp.push_back("ORDER BY sessions.date, sessions.time");

    query->prepare(tmp);
    query->bindValue(":dateFrom", ui->dateFilterFrom->date().toString("yyyy-MM-dd"));
    query->bindValue(":timeFrom", ui->timeFilterFrom->time().toString("HH:mm:ss"));
    query->bindValue(":dateBefore", ui->dateFilterBefore->date().toString("yyyy-MM-dd"));
    query->bindValue(":timeBefore", ui->timeFilterFrom->time().toString("HH:mm:ss"));
    query->exec();
    int count = query->size();
    int current = 0;
    ui->tableWidget->setRowCount(count);
    while(query->next())
    {
        sessions_id.push_back(query->value(0).toInt());
        for(int i = 1; i < 5; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(query->value(i).toString());
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            ui->tableWidget->setItem(current, i-1, item);
        }
        current++;
    }

    for(int i = 0; i < sessions_id.size(); i++)
    {
        query->prepare("SELECT MIN(price), MAX(price) "
                       "FROM price_of_tickets "
                       "WHERE session = :session");
        query->bindValue(":session", sessions_id[i]);
        query->exec();
        query->next();
        QTableWidgetItem *item = new QTableWidgetItem(query->value(0).toString() + " - " + query->value(1).toString());
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->tableWidget->setItem(i, 4, item);
    }
}

void MainWindow::updateAccess()
{
    access->setLogin(infoUser->login);

    addSessionEnabled = access->checkAccess("addSessionEnabled");
    editSessionEnabled = access->checkAccess("editSessionEnabled");
    removeSessionEnabled = access->checkAccess("removeSessionEnabled");

    ui->menuSession->menuAction()->setVisible(access->checkAccess("sessionsEnabled"));
    ui->menuTickets->menuAction()->setVisible(access->checkAccess("ticketsEnabled"));
    ui->actionEmployees->setVisible(access->checkAccess("employeesEnabled"));
    ui->actionFilms->setVisible(access->checkAccess("filmsEnabled"));

    ui->actionAddSession->setVisible(access->checkAccess("addSessionEnabled"));
    ui->actionEditSession->setVisible(access->checkAccess("editSessionEnabled"));
    ui->actionRemoveSession->setVisible(access->checkAccess("removeSessionEnabled"));

    ui->actionTicketInfo->setVisible(access->checkAccess("ticketInfoEnabled"));
    ui->actionTicketReturn->setVisible(access->checkAccess("ticketReturnEnabled"));

    ui->centralWidget->setVisible(access->checkAccess("sessionsEnabled"));

    ui->menuData->menuAction()->setVisible(ui->menuSession->isVisible() || ui->menuTickets->isVisible() || ui->actionEmployees->isVisible() || ui->actionFilms->isVisible());
}

void MainWindow::authShow()
{
    AuthentificationUserDialog *authDialog = new AuthentificationUserDialog(infoUser, query, this);
//    authDialog->setModal(true);
    authDialog->exec();
    updateAccess();
}

void MainWindow::settShow()
{
    SettingDialog *settDialog = new SettingDialog(query, access, this);
    settDialog->setWindowState(Qt::WindowActive);
    settDialog->setFixedSize(642, 434);
    settDialog->exec();
    updateAccess();
}

void MainWindow::on_pushButton_clicked()
{
    updateTable();
}

void MainWindow::on_actionLogin_triggered()
{

}

void MainWindow::on_actionLogout_triggered()
{
    AuthentificationUserDialog *authDialog = new AuthentificationUserDialog(infoUser, query, this);
//    authDialog->setModal(true);
    authDialog->exec();
    delete authDialog;
    updateAccess();
    loginStatusLabel->setText("<img width=\"15\" height=\"15\" src=\":/icons/icons/user.ico\"/> " + infoUser->login);
}

void MainWindow::onTableContextMenu(QPoint p)
{
    QMenu contextTableMenu;
    contextTableMenu.addAction(ui->actionAddSession);
    contextTableMenu.addAction(ui->actionEditSession);
    contextTableMenu.addAction(ui->actionRemoveSession);
    if(ui->actionAddSession->isVisible() || ui->actionEditSession->isVisible() || ui->actionRemoveSession->isVisible())
        contextTableMenu.addSeparator();
    contextTableMenu.addAction(ui->actionUpdateSessions);

    QPoint globalPos;

    globalPos = ui->tableWidget->mapToGlobal(p);

    contextTableMenu.exec(globalPos);
}

void MainWindow::on_actionFilms_triggered()
{
    FilmsDialog *filmDialog = new FilmsDialog(query, access, this);
    filmDialog->exec();
    delete filmDialog;
    updateTable();
}

void MainWindow::on_actionAboutProgram_triggered()
{
    QMessageBox msg;
    msg.setWindowTitle(tr("Информация о программе"));
    msg.setText(tr("Программа продажи билетов в кинотеатре"));
    msg.exec();
}

void MainWindow::on_actionAddSession_triggered()
{
    AddSessionDialog addSession(query);
    addSession.exec();
    updateTable();
}

void MainWindow::on_actionEditSession_triggered()
{
    EditSessionDialog editSession(sessions_id[ui->tableWidget->currentRow()], query);
    editSession.exec();
    updateTable();
}

void MainWindow::on_actionUpdateSessions_triggered()
{
    updateTable();
}

void MainWindow::on_actionLock_triggered()
{
    LockDialog *lockDialog = new LockDialog(&infoUser->login, query, this);
    connect(lockDialog, SIGNAL(sendChangeUser()), this, SLOT(changeUser()));
    lockDialog->exec();
    delete lockDialog;
}

void MainWindow::on_actionRemoveSession_triggered()
{
    if(ui->tableWidget->currentRow() < 0) return;

    QMessageBox msg;
    msg.setWindowTitle(tr("Удаление"));
    msg.setText(tr("Подтвердить удаление?"));
    QAbstractButton *confirmButton = msg.addButton(tr("Подтвердить"), QMessageBox::YesRole);
    QAbstractButton *cancelButton = msg.addButton(tr("Отмена"), QMessageBox::NoRole);
    msg.exec();
    if(msg.clickedButton() == confirmButton)
    {
        query->prepare("DELETE FROM sessions "
                       "WHERE id_session = :id_session");
        query->bindValue(":id_session", sessions_id[ui->tableWidget->currentRow()]);
        bool exec = query->exec();
        qDebug() << "DELERE FROM sessions:" << exec;
    }
    updateTable();
}

void MainWindow::changeUser()
{
    AuthentificationUserDialog *authDialog = new AuthentificationUserDialog(infoUser, query, this);
//    authDialog->setModal(true);
    authDialog->exec();
}

void MainWindow::on_actionEmployees_triggered()
{
    EmployeesDialog *employeesDialog = new EmployeesDialog(query, access, this);
    employeesDialog->exec();
    delete employeesDialog;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_tableWidget_itemSelectionChanged()
{

}

void MainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    ShowSessionDialog *showSession = new ShowSessionDialog(infoUser, sessions_id[ui->tableWidget->currentRow()], query, this);
    showSession->exec();
    delete showSession;
    //    qDebug() << index.row();
}

void MainWindow::updateTimeStatusBar()
{
    timeStatusLabel->setText("<img width=\"15\" height=\"15\" src=\":/icons/icons/calendar.ico\"/> " + QTime::currentTime().toString("HH:mm:ss"));
    dateStatusLabel->setText("<img width=\"15\" height=\"15\" src=\":/icons/icons/clock.ico\"/> " + QDate::currentDate().toString("dd.MM.yyyy"));
}

void MainWindow::on_actionTicketReturn_triggered()
{
    bool okButton;
    int numberTicket = QInputDialog::getInt(this, tr("Возврат билета"), tr("Введите номер билета"), 0, 0, 50000000, 1, &okButton);

    if(!okButton) return;

    QMessageBox msgConfirmReturn;
    msgConfirmReturn.setWindowTitle(tr("Возврат билета"));
    msgConfirmReturn.setText(tr("Подтвердить возврат билета?"));
    QAbstractButton *msgConfirm = msgConfirmReturn.addButton(tr("Подтвердить"), QMessageBox::YesRole);
    msgConfirmReturn.addButton(tr("Отмена"), QMessageBox::NoRole);
    msgConfirmReturn.exec();

    if(msgConfirmReturn.clickedButton() == msgConfirm)
    {
        query->prepare("UPDATE `tickets` "
                       "SET `status` = :status "
                       "WHERE `id_ticket`= :ticket");
        query->bindValue(":status", HallQml::StatusFree);
        query->bindValue(":ticket", numberTicket);
        query->exec();
        if(query->numRowsAffected() <= 0)
        {
            QMessageBox msgError;
            msgError.setWindowTitle(tr("Ошибка"));
            msgError.setText(tr("Данного билета не существует!"));
            msgError.setIcon(QMessageBox::Warning);
            msgError.exec();
            return;
        }

        query->prepare("INSERT INTO `traffic_tickets` (`date`, `time`, `ticket`, `operation`, `employee`) "
                       "VALUES (:date, :time, :ticket, :operation, :employee)");
        query->bindValue(":date", QDate::currentDate().toString("yyyy-MM-dd"));
        query->bindValue(":time", QTime::currentTime().toString("HH:mm:ss"));
        query->bindValue(":ticket", numberTicket);
        query->bindValue(":operation", TicketOperations::operationReturn);
        query->bindValue(":employee", infoUser->idlogin);
        query->exec();
    }
}

void MainWindow::on_actionTicketInfo_triggered()
{
    bool okButton;
    int numberTicket = QInputDialog::getInt(this, tr("Информация про билет"), tr("Введите номер билета"), 0, 0, 50000000, 1, &okButton);

    if(!okButton) return;

    query->prepare("SELECT id_ticket "
                   "FROM tickets "
                   "WHERE id_ticket = :ticket");
    query->bindValue(":ticket", numberTicket);
    query->exec();

    if(query->size() <= 0)
    {
        QMessageBox msgError;
        msgError.setWindowTitle(tr("Ошибка"));
        msgError.setText(tr("Данного билета не существует!"));
        msgError.setIcon(QMessageBox::Warning);
        msgError.exec();
        return;
    }

    TicketInfoDialog *ticketDialog = new TicketInfoDialog(numberTicket, query, this);
    ticketDialog->exec();
    delete ticketDialog;
}
