#ifndef SETTINGHALLQML_H
#define SETTINGHALLQML_H

#include <QObject>
#include <QVector>
#include <QSqlQuery>
#include <QResizeEvent>
#include "usertypes.h"

class SettingHallQML : public QObject
{
    Q_OBJECT

public:
    explicit SettingHallQML(QObject *parent = 0);

    bool setSizeScheme(int width, int height);
    bool setSizeHall(int countRows, int countColumns);

    bool addSector(int id, QString text, QString description, QString colorHEX);
    bool editSector(int id, QString text, QString description, QString colorHEX);
    bool removeSector(int id);

    bool sectorsDialog();
    bool addSectorDialog();
    bool editSectorDialog(int sectorId);
    bool removeSectorDialog(int sectorId);

    void setVisibleUnsignedPlaces(bool visible);
    void setSettingMode(bool enabled);
    void setDefaultSector();

    bool setPlace(int idPlace, int v_place, int g_place, int row, int sectorId);
    bool setStatusPlace(int g_place, int row, int status);
    bool sendPlaces();
    bool sendStatus();

    QVector<sector>& getSectors();
    QVector<QVector<place> >& getPlaces();

signals:
    void sendCheckedPlace(int s_place, int s_g_place, int s_row);
    void sendPlace(int s_place, int s_g_place, int s_row, int s_sectorId);

    void sendSizeHall(int s_countRows, int s_countColumns);
    void sendSizeScheme(int s_width, int s_height);

    void sendVisibleUnsignedPlaces(bool s_visible);
    void sendSettingMode(bool s_mode);

    void sendCountSectors(int s_countSectors);
    void sendEditSector(int s_id, QString s_text, QString s_description, QString s_color);
    void sendClearCheckedSector();
    void sendRemoveSector(int s_id);
    void sendDefaultSector(int s_id, QString s_color);

    void sendStatus(int s_g_place, int s_row, int s_status);

signals:
    void checkedPlace(int s_g_place, int s_row, int s_sectorIndex);
    void uncheckedPlace(int s_g_place, int s_row, int s_sectorIndex);

public slots:
    int getSectorId(int index);
    QString getSectorColor(int index);
    QString getSectorText(int index);
    QString getSectorDescription(int index);
    void receiveCheckedPlace(int g_place, int row, int sectorId);
    void receiveUnCheckedPlace(int g_place, int row);
    void receiveClearCheckedSectors();
    bool receiveCheckedPlace(int g_place, int row);
    bool receiveUncheckedPlace(int g_place, int row);

private slots:
    void addSectorDialogResult(sector sector_info);

private:
    int hallId;
    bool settingMode = true;
    bool visibleUnsignedPlace = true;
    QVector <QVector <place> > places;
    QVector <sector> sectors;
};

#endif // SETTINGHALLQML_H
