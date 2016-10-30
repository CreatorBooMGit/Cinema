#ifndef USERTYPES_H
#define USERTYPES_H

#include <QString>

namespace HallQml {
    enum Status { StatusAdd = 1, StatusUpdate = 2, StatusNew = 3 };
    enum StatusPlace { StatusBooking = 4, StatusSold = 5, StatusFree = 6, StatusChecked = 7 };
}

struct sector {
    int id = -1;
    QString text;
    QString description;
    QString color;
    int status = HallQml::StatusNew;
};

struct place {
    int idPlace = 0;
    int v_place = 0;
    int sectorId = 0;
    int status = HallQml::StatusNew;
};

#endif // USERTYPES_H
