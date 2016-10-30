#include "SettingHallQml.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>

#include "AddSectorDialog.h"
#include "EditSectorDialog.h"
#include "SectorsDialog.h"
#include "usertypes.h"

SettingHallQML::SettingHallQML(QObject *parent) : QObject(parent)
{
//    countPlacesInRow.reserve(countRows);
    //    countPlacesInRow.fill(0);
//    connect()
}

bool SettingHallQML::setSizeScheme(int width, int height)
{
    emit sendSizeScheme(width, height);
}

bool SettingHallQML::setSizeHall(int countRows, int countColumns)
{
    places.resize(countRows);
    for(int i = 0; i < countRows; i++)
    {
        places[i].resize(countColumns);
//        qDebug() << places[i].size();
    }

//    qDebug() << places.size();
    emit sendSizeHall(countRows, countColumns);
    sendPlaces();
    setSettingMode(settingMode);
    setVisibleUnsignedPlaces(visibleUnsignedPlace);
    return true;
}

bool SettingHallQML::addSector(int id, QString text, QString description, QString colorHEX)
{
    sector tmp;
    tmp.id = id;
    tmp.text = text;
    tmp.description = description;
    tmp.color = colorHEX;
    tmp.status = HallQml::StatusAdd;
    sectors.push_back(tmp);

    emit sendCountSectors(sectors.size());
    return true;
}

bool SettingHallQML::editSector(int id, QString text, QString description, QString colorHEX)
{
    for(int i = 0; i < sectors.size(); i++)
    {
        qDebug() << sectors[i].id;
        if(sectors[i].id == id)
        {
            sectors[i].text = text;
            sectors[i].description = description;
            sectors[i].color = colorHEX;
            sectors[i].status = HallQml::StatusUpdate;
            emit sendEditSector(i, text, description, colorHEX);
            return true;
        }
    }
    return false;
}

bool SettingHallQML::removeSector(int id)
{
//    for(int i = 0; i < sectors.size(); i++)
//        if(sectors[i].id == id)
//        {
//            sectors.remove(i);
//            emit sendCountSectors(sectors.size());
//            emit sendRemoveSector(i);
//            return true;
//        }
    if(sectors.size() < id) return false;

    sectors.remove(id);

    for(int row = 0; row < places.size(); row++)
        for(int col = 0; col < places[row].size(); col++)
            if(places[row][col].sectorId > id)
                places[row][col].sectorId--;

    emit sendRemoveSector(id);
    emit sendCountSectors(sectors.size());
    return true;
}

bool SettingHallQML::addSectorDialog()
{
    AddSectorDialog *addDialog = new AddSectorDialog;
    connect(addDialog, SIGNAL(addSector(sector)), this, SLOT(addSectorDialogResult(sector)));
    addDialog->exec();
    delete addDialog;
    emit sendCountSectors(sectors.size());
    return true;
}

bool SettingHallQML::editSectorDialog(int sectorId)
{
    if(sectors.size() == 0) return false;

//    QStringList list;
//    for(int i = 0; i < sectors.size(); i++)
//        list.push_back(sectors[i].text);
//    int sector_index = QInputDialog::getInt(NULL, tr("Выбор сектора"), tr("Введите индекс сектора"), 0);
    EditSectorDialog *editDialog = new EditSectorDialog(&sectors[sectorId], NULL);
    editDialog->exec();
    delete editDialog;
    emit sendEditSector(sectorId, sectors[sectorId].text, sectors[sectorId].description, sectors[sectorId].color);
    return true;
}

bool SettingHallQML::sectorsDialog()
{
    SectorsDialog *sectors_dialog = new SectorsDialog(&sectors);
    sectors_dialog->exec();
    delete sectors_dialog;
}

bool SettingHallQML::removeSectorDialog(int sectorId)
{
    if(sectors.size() == 0) return false;

//    int sector_index = QInputDialog::getInt(NULL, tr("Выбор сектора"), tr("Введите индекс сектора"), 0);
    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle(tr("Удаление сектора"));
    confirmMsg.setText(tr("Подтвердить удаление?"));
    QPushButton *confirmMsgButtonYes = confirmMsg.addButton(tr("Подтвердить"), QMessageBox::YesRole);
    confirmMsg.addButton(tr("Отмена"), QMessageBox::NoRole);
    confirmMsg.exec();
    if(confirmMsg.clickedButton() == confirmMsgButtonYes)
    {
        removeSector(sectorId);
        return true;
    }
    return false;
}

void SettingHallQML::setVisibleUnsignedPlaces(bool visible)
{
    visibleUnsignedPlace = visible;

    emit sendVisibleUnsignedPlaces(visible);
}

void SettingHallQML::setSettingMode(bool enabled)
{
    settingMode = enabled;
    emit sendSettingMode(enabled);

    if(enabled == false)
    {
        if(sectors.size() > 0)
        {
            if(sectors[0].status != HallQml::StatusSold)
            {
                sector tmp;
                tmp.id = 0;
                tmp.text = "Занято";
                tmp.description = "Место занято";
                tmp.color = "#878b91";
                tmp.status = HallQml::StatusSold;
                sectors.push_front(tmp);

                for(int row = 0; row < places.size(); row++)
                    for(int col = 0; col < places[row].size(); col++)
                        if(places[row][col].sectorId >= 0)
                        {
                            //                        places[row][col].status = HallQml::StatusFree;
                            places[row][col].sectorId++;
                        }

                emit sendCountSectors(sectors.size());
            }
        }
        else
        {
            sector tmp;
            tmp.id = 0;
            tmp.text = "Занято";
            tmp.description = "Место занято";
            tmp.color = "#878b91";
            tmp.status = HallQml::StatusSold;
            sectors.push_front(tmp);

            for(int row = 0; row < places.size(); row++)
                for(int col = 0; col < places[row].size(); col++)
                    if(places[row][col].sectorId >= 0)
                    {
                        //                        places[row][col].status = HallQml::StatusFree;
                        places[row][col].sectorId++;
                    }

            emit sendCountSectors(sectors.size());
        }
    }
}

void SettingHallQML::setDefaultSector()
{
    if(sectors.size() > 0)
        emit sendDefaultSector(0, sectors[0].color);
}

bool SettingHallQML::setPlace(int idPlace, int v_place, int g_place, int row, int sectorId)
{
    if(places.size() <= 0) return false;
    if(places.size() <= row + 1) return false;
    if(places[row].size() <= g_place + 1) return false;

//    bool cont = false;
//    for(int i = 0; i < sectors.size(); i++)
//        if(sectors[i].id == sectorId)
//        {
//            places[row][g_place].sectorId = i;
//            cont = true;
//            break;
//        }
//    if(!cont) return false;

    places[row][g_place].idPlace = idPlace;
    places[row][g_place].v_place = v_place;
    places[row][g_place].sectorId = sectorId;
    places[row][g_place].status = HallQml::StatusAdd;

    return true;
}

bool SettingHallQML::setStatusPlace(int g_place, int row, int status)
{
    if(places.size() <= 0) return false;
    if(places.size() <= row + 1) return false;
    if(places[row].size() <= g_place + 1) return false;

    places[row][g_place].status = status;

    return true;
}

//bool SettingHallQML::setPlaces(QVector<QVector<int> > &s_places)
//{
//    places.clear();
//    places.append(s_places);
//}

bool SettingHallQML::sendPlaces()
{
    for(int row = 0; row < places.size(); row++)
        for(int col = 0; col < places[row].size(); col++)
            if(places[row][col].v_place > 0)
                emit sendPlace(places[row][col].v_place, col, row, places[row][col].sectorId);
    return true;
}

bool SettingHallQML::sendStatus()
{
    for(int row = 0; row < places.size(); row++)
        for(int col = 0; col < places[row].size(); col++)
            if(places[row][col].v_place > 0)
                emit sendStatus(col, row, places[row][col].status);
    return true;
}

QVector<sector>& SettingHallQML::getSectors()
{
    return sectors;
}

QVector<QVector<place> > &SettingHallQML::getPlaces()
{
    return places;
}

int SettingHallQML::getSectorId(int index)
{
    return sectors[index].id;
}

QString SettingHallQML::getSectorColor(int index)
{
    return sectors[index].color;
}

QString SettingHallQML::getSectorText(int index)
{
    return sectors[index].text;
}

QString SettingHallQML::getSectorDescription(int index)
{
    return sectors[index].description;
}

void SettingHallQML::receiveCheckedPlace(int g_place, int row, int sectorId)
{
    int v_place = 1;
    for(int i = g_place - 1; i >= 0; i--)
        if(places[row][i].v_place > 0)
        {
            v_place = places[row][i].v_place + 1;
            break;
        }

    if(places[row][g_place].v_place != 0)
        places[row][g_place].status = HallQml::StatusUpdate;
    else
        places[row][g_place].status = HallQml::StatusNew;

    places[row][g_place].v_place = v_place;
    places[row][g_place].sectorId = sectorId;
//    query.push_back("INSERT INTO `cinema`.`places` (`sector`, `place`, `g_place`, `row`) VALUES ('1', '" + QString::number(place) + "', '" + QString::number(g_place) + "', '" + QString::number(row) + "'); ");
//    qDebug() << query;
    emit sendCheckedPlace(v_place, g_place, row);
    int help = 1;
    for(int i = g_place + 1; i < places[row].size(); i++)
        if(places[row][i].v_place > 0)
        {
            places[row][i].v_place = v_place + help++;
            emit sendCheckedPlace(places[row][i].v_place, i, row);
        }
}

void SettingHallQML::receiveUnCheckedPlace(int g_place, int row)
{
    if(places[row][g_place].v_place == 0)
        return;

    for(int i = g_place + 1; i < places[row].size(); i++)
        if(places[row][i].v_place > 0)
        {
            places[row][i].v_place--;
            emit sendCheckedPlace(places[row][i].v_place, i, row);
        }

    places[row][g_place].status = HallQml::StatusUpdate;
    places[row][g_place].v_place = 0;
    places[row][g_place].sectorId = -1;
}

void SettingHallQML::receiveClearCheckedSectors()
{
    emit sendClearCheckedSector();
}

bool SettingHallQML::receiveCheckedPlace(int g_place, int row)
{
    if(places[row][g_place].status == HallQml::StatusFree)
    {
        places[row][g_place].status = HallQml::StatusChecked;
        emit checkedPlace(g_place, row, places[row][g_place].sectorId);
        return true;
    }
    return false;
}

bool SettingHallQML::receiveUncheckedPlace(int g_place, int row)
{
    if(places[row][g_place].status != HallQml::StatusFree)
    {
        places[row][g_place].status = HallQml::StatusFree;
        emit uncheckedPlace(g_place, row, places[row][g_place].sectorId);
        return false;
    }
    return true;
}

void SettingHallQML::addSectorDialogResult(sector sector_info)
{
    sectors.push_back(sector_info);
}
