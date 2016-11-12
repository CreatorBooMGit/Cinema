#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>

#include "usertype.h"
#include "access.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *e);

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlQuery *query;
    QSettings *settings;
    user *infoUser;
    Access *access;

    QLabel *hostStatusLabel, *hostStatusIcon;
    QLabel *loginStatusLabel, *loginStatusIcon;
    QLabel *dateStatusLabel, *dateStatusIcon;
    QLabel *timeStatusLabel, *timeStatusIcon;

    QTimer *timeTimer;

    bool addSessionEnabled, editSessionEnabled, removeSessionEnabled;

//    struct session { int id_session, hall, film; QString date, time; };

    QVector <int> sessions_id;

    bool updateTable();
    void updateAccess();

private slots:
    void authShow();
    void settShow();
    void on_pushButton_clicked();
    void on_actionLogin_triggered();
    void on_actionLogout_triggered();
    void onTableContextMenu(QPoint p);
    void on_actionFilms_triggered();
    void on_actionAboutProgram_triggered();
    void on_actionAddSession_triggered();
    void on_actionEditSession_triggered();
    void on_actionUpdateSessions_triggered();
    void on_actionLock_triggered();
    void on_actionRemoveSession_triggered();
    void changeUser();
    void on_actionEmployees_triggered();
    void on_actionExit_triggered();
    void on_tableWidget_itemSelectionChanged();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void updateTimeStatusBar();
    void on_actionTicketReturn_triggered();
    void on_actionTicketInfo_triggered();
};

#endif // MAINWINDOW_H
