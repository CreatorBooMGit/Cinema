import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id: scheme
    width: 640
    height: 480

    property int countRows : 15
    property int countColumns : 15

    property int countSectors: 0
//    property vector colors

    property int checkedSectorIndex: -1
    property string checkedSectorColor: "#fff"

    property bool settingMode: true

    Connections {
        target: hallCore

        onSendSizeHall: {
            countColumns = s_countColumns;
            countRows = s_countRows;
        }

        onSendCountSectors: {
            countSectors = s_countSectors;
        }

        onSendSettingMode: {
            settingMode = s_mode;
        }

        onSendDefaultSector: {
            scheme.checkedSectorIndex = s_id;
            scheme.checkedSectorColor = s_color;
        }

        onSendRemoveSector: {
//            scheme.checkedSectorIndex =
        }

        onSendSizeScheme: {
            scheme.width = s_width;
            scheme.height = s_height;
        }
    }

    Rectangle {
        id: sectorPanel

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
//        width: parent.width
        height: 30

        enabled: true

        Row {
            id: sectors
            spacing: 5

            Repeater {
                id: repeaterSectors
                model: countSectors

                Rectangle {
                    id: sectorFlat
                    width: sectorPanel.width / countSectors
                    height: 20
                    enabled: true

                    ToolTip {
                        id: sectorToolTip
                        delay: 500
                        text: hallCore.getSectorDescription(index)
                    }

                    Connections {
                        target: hallCore

                        onSendEditSector: {
                            if(index === s_id)
                            {
                                sector.colorSector = s_color;
                                sector.textSector = s_text;
                                sectorToolTip.text = s_description;
                            }

                            if(checkedSectorIndex === s_id)
                                scheme.checkedSectorColor = s_color;
                        }

                        onSendClearCheckedSector: {
                            sector.checkedSector = false;
                        }
                    }

                    Rectangle {
                        id: sector
                        width: 20
                        height: 20
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        enabled: true

                        property int indexSector: hallCore.getSectorId(index)
                        property string colorSector: hallCore.getSectorColor(index)
                        property string textSector: hallCore.getSectorText(index)
                        property bool checkedSector: false

                        color: colorSector

                        Text {
                            anchors {
                                leftMargin: 2
                                left: parent.right
                                verticalCenter: parent.verticalCenter
                            }

                            font.pixelSize: 14
                            text: sector.textSector
                        }

                        Image {
                            id: checkedIcon
                            anchors.fill: parent
                            visible: index === scheme.checkedSectorIndex ? true : false
                            source: "qrc:/icons/HallScheme/checked.ico"
                        }

                    }
                    MouseArea {
                        id: sectorMouseArea
                        enabled: settingMode
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            scheme.checkedSectorColor = sector.colorSector
                            scheme.checkedSectorIndex = index
//                            checkedIcon.visible = true;
                            console.log(scheme.checkedSectorColor, index);
                        }
                        onEntered: {
//                            checkedIcon.visible = true;
                            if(hallCore.getSectorDescription(index) !== "")
                                sectorToolTip.open();
                        }
                        onExited: {
//                            checkedIcon.visible = false;
                            sectorToolTip.close();
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: gridPlaces

        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: sectorPanel.bottom
        }

        Column {
            id: gridColumns

            spacing: 5

            Repeater {
                id: gridColumnsRepeater
                model: countRows
    //            Text {
    //                idq: textNumberRow
    //                text: qsTr("Ряд %1").arg(index)
    //            }
                Row {
                    id: gridRows
                    spacing: 5
                    property int currentRow: index

                    Repeater {
                        id: gridRowsRepeater
                        model: countColumns
                        Place {
                            g_place: index
                            row: currentRow
                            sectorIndexChecked: checkedSectorIndex
                            colorChecked: checkedSectorColor
                        }
                    }
                }
            }
        }
    }
}
