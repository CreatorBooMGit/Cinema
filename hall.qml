import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0

Rectangle {
    id: main
    width: 500
    height: 400

    property string chooseColorPlace : "green"
    // Количество мест
    property int countRow: 20
    property int countCol: 35
    // Размеры места
    property int widthPlace : 25
    property int heightPlace : 25
    property int radiusPlace : 5
    property int borderSize : 2
    // Расстояние между местами
    property int spacingBetweenCol : 5
    property int spacingBetweenRow : 5

    Rectangle
    {
        id: places
        anchors.fill: parent

        onWidthChanged: console.log("width-places: " + width);
        onHeightChanged: console.log("height-places: " + height);

        Column
        {
            id: col
            x: 5
            y: 5
            anchors.fill: parent
            spacing: spacingBetweenRow

            Repeater {
                id: rowRep
                model: countRow

                Row {
                    id: row
                    spacing: spacingBetweenRow

                    Place {
                        numberPlace: index + 1
                        colorStandart: "white"
                        borderWidth: 0
                    }
                    Repeater {
                        model: countCol

                        Place {
                            numberPlace: index + 1
                            colorClicked: main.chooseColorPlace

                            MouseArea {
                                anchors.fill: parent

                                onClicked: console.log("INSERT INTO `cinema`.`places` (`sector`, `place`, `g_place`, `row`) VALUES ('1', '1', '" + index + "', " + rowRep.index + " );")
                            }
                        }
                    }
                    Place {
                        numberPlace: index + 1
                        colorStandart: "white"
                        borderWidth: 0
                    }
                }
            }
        }
    }
}
/*Rectangle {
    id: main
    width: 500
    height: 400

    property string chooseColorPlace : "green"
    // Количество мест
    property int countRow: 20
    property int countCol: 35
    // Размеры места
    property int widthPlace : 25
    property int heightPlace : 25
    property int radiusPlace : 5
    property int borderSize : 2
    // Расстояние между местами
    property int spacingBetweenCol : 5
    property int spacingBetweenRow : 5

    SplitView
    {
        id: mapOfPlaces
        anchors.top:  parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: chooseColor.top


        Flickable
        {
            id: flick
            anchors.fill: parent

            onWidthChanged: console.log("width-flick: " + width);
            onHeightChanged: console.log("height-flick: " + height);
            onContentHeightChanged: console.log("contentHeight-flick: " + contentHeight);
            onContentWidthChanged: console.log("contentWidth-flick: " + contentWidth);


            contentHeight: countRow * heightPlace + spacingBetweenCol * countRow
            contentWidth: countCol * widthPlace + spacingBetweenRow * countCol
            boundsBehavior: Flickable.StopAtBounds

            Rectangle
            {
                id: places
                anchors.fill: parent

                onWidthChanged: console.log("width-places: " + width);
                onHeightChanged: console.log("height-places: " + height);

                Column
                {
                    id: col
                    x: 5
                    y: 5
                    anchors.fill: parent
                    spacing: spacingBetweenRow

                    Repeater {
                        model: countRow

                        Row {
                            id: row
                            spacing: spacingBetweenRow

                            Positioner {
                                id: pos

                            }

                            Place {
                                numberPlace: index + 1
                                colorStandart: "white"
                                borderWidth: 0
                            }
                            Repeater {
                                model: countCol

                                Place {
                                    numberPlace: index + 1
                                    colorClicked: main.chooseColorPlace

                                    MouseArea {
                                        anchors.fill: parent

                                        onClicked: console.log("INSERT INTO `cinema`.`places` (`sector`, `place`, `g_place`, `row`) VALUES ('1', '1', '" + index + "', " + pos. + ");")
                                    }
                                }
                            }
                            Place {
                                numberPlace: index + 1
                                colorStandart: "white"
                                borderWidth: 0
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: chooseColor
        anchors.right: main.right
        anchors.left: main.left
        anchors.bottom: main.bottom

        height: 30

        Rectangle {
            color: "red"
            width: 25
            height: 25
            radius: 5

            anchors.verticalCenter: chooseColor.verticalCenter

            Text {
                text: "red"
                anchors.left: parent.right
            }

            MouseArea {
                anchors.fill: parent

                onClicked: main.chooseColorPlace = "red"
            }
        }
    }
}
*/
