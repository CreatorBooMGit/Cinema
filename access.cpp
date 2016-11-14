#include "access.h"
#include <QVariant>
#include <QDebug>

Access::Access()
{

}

Access::Access(QString log, QSqlQuery *q)
{
    setQuery(q);
    setLogin(log);
}

Access::~Access()
{

}

void Access::setLogin(QString log)
{
    login = log;

    query->prepare("SELECT posts.id_post "
                   "FROM posts, employees "
                   "WHERE employees.post = posts.id_post AND employees.login = :login");
    query->bindValue(":login", login);
    query->exec();
    query->next();
    idpost = query->value(0).toInt();
}

void Access::setQuery(QSqlQuery *q)
{
    query = q;
}

bool Access::checkAccess(QString access)
{
    query->prepare("SELECT access_post.idaccess "
                   "FROM access_post, access_level "
                   "WHERE access_post.access = access_level.idaccess AND access_level.name = :name AND access_post.post = :post");
    query->bindValue(":name", access);
    query->bindValue(":post", idpost);
    query->exec();
    if(query->size() <= 0) return false;
    else
        return true;
}

bool Access::checkAccess(int s_idpost, QString access)
{
    query->prepare("SELECT access_post.idaccess "
                   "FROM access_post, access_level "
                   "WHERE access_post.access = access_level.idaccess AND access_level.name = :name AND access_post.post = :post");
    query->bindValue(":name", access);
    query->bindValue(":post", s_idpost);
    query->exec();
    if(query->size() <= 0) return false;
    else
        return true;
}

