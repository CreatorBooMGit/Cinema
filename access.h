#ifndef ACCESS_H
#define ACCESS_H

#include <QString>
#include <QSqlQuery>

class Access
{
public:
    Access();
    Access(QString log, QSqlQuery *q);
    ~Access();

    void setLogin(QString log);
    void setQuery(QSqlQuery *q);
    bool checkAccess(QString access);

private:
    QSqlQuery *query;
    QString login;
    int idpost;
};

#endif // ACCESS_H
