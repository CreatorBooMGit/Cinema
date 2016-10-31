#ifndef USERTYPE_H
#define USERTYPE_H

#include <QString>

struct user {
    int idlogin;
    int idpost;
    QString login, surname, name, patronymic, postName;
};

#endif // USERTYPE_H
