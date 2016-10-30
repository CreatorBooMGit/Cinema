import QtQuick 2.0

Rectangle {
    id: root
    height: 30
    width: 30

    readonly property int statusBooking: 4
    readonly property int statusSold: 5
    readonly property int statusFree: 6
    readonly property int statusChecked: 7

    property int sectorIndex : -1
    property int g_place : -1
    property int place : 0
    property int row : -1
    property int status : -1

    property string colorChecked : "red"
    property string backgroundColor : "white"
    property int sectorIndexChecked : -1

    property bool visibleUnsignedPlace : true
    property bool settingMode : true
    property bool isChecked : false

    border.color: "black"
    border.width: place <= 0 ? visibleUnsignedPlace : true
    radius: 3

    Connections {
        target: hallCore

        onSendCheckedPlace: {
            if(row === s_row)
                if(g_place === s_g_place)
                    place = s_place;
        }

        onSendPlace: {
            if(row === s_row && g_place === s_g_place)
            {
                root.place = s_place;
                root.sectorIndex = s_sectorId;
                root.color = hallCore.getSectorColor(s_sectorId);
            }
        }

        onSendVisibleUnsignedPlaces: {
              visibleUnsignedPlace = s_visible;
        }

        onSendEditSector: {
            if(sectorIndex === s_id)
            {
//                root.sectorIndex = s_id;
                root.color = s_color;
            }
        }

        onSendRemoveSector: {
            if(sectorIndex > s_id)
            {
                root.sectorIndex--;
            } else
            if(sectorIndex === s_id)
            {
                root.color = "white";
                root.sectorIndex = -1;
                root.place = 0;
                hallCore.receiveUnCheckedPlace(g_place, row);
            }
        }

        onSendSettingMode: {
            settingMode = s_mode;
        }

        onSendStatus: {
            if(row === s_row && g_place === s_g_place)
            {
                root.status = s_status;
                if(s_status === statusSold)
                    root.color = hallCore.getSectorColor(0);
            }
        }
    }

    Text {
        id: text
        anchors.centerIn: parent
        visible: place <= 0 ? visibleUnsignedPlace : true

        text: qsTr("%1").arg(place)
    }

    Image {
        id: imgPlace
        anchors.fill: parent
        visible: isChecked
        fillMode: Image.PreserveAspectFit
        source: "qrc:/icons/HallScheme/person.ico"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        enabled: place <= 0 ? visibleUnsignedPlace : true

        onClicked: {
//            console.log("before --- ", "place: ", place, "g_place: ", g_place, "row: ", row, "sector:", sectorIndex);
            if(settingMode)
            {
                if(mouse.button & Qt.LeftButton)
                {
                    root.color = colorChecked;
                    root.sectorIndex = sectorIndexChecked;
                    hallCore.receiveCheckedPlace(g_place, row, sectorIndex);
                } else
                    if(mouse.button & Qt.RightButton)
                    {
                        root.color = "white";
                        root.sectorIndex = -1;
                        root.place = 0;
                        hallCore.receiveUnCheckedPlace(g_place, row);
                    }
            }
            else
            {
                if(mouse.button & Qt.LeftButton && status === statusFree)
                {
                    root.isChecked = hallCore.receiveCheckedPlace(g_place, row);
                    if(isChecked)
                    {
                        status = statusChecked
                    }
                }
                else
                    if(mouse.button & Qt.RightButton && status === statusChecked)
                    {
                        root.isChecked = hallCore.receiveUncheckedPlace(g_place, row);
                        if(isChecked)
                        {
                            status = statusFree
                        }
                    }
            }

//            console.log("after --- ", "place: ", place, "g_place: ", g_place, "row: ", row, "sector:", sectorIndex);
        }
    }
}
