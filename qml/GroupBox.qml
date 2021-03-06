/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

// import QtQuick 2.0 // to target S60 5th Edition or Maemo 5
import QtQuick 2.0

Item {

    property color backgroundColor: "white"
    property alias text: titleText.text
    property int contentHeight
    default property alias contentChildren: contentArea.children

    height: titleText.height + contentHeight + 3

    Style {
        id: style
    }

    Rectangle {
        id: checkBoxGroup
        border.width: 1
        border.color: "gray"
        radius: style.floatingWidgetRadius
        color: "#00000000"

        anchors.top: titleText.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Item {
            id: contentArea
            anchors.top: checkBoxGroup.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            anchors.topMargin: titleText.height / 2
            anchors.leftMargin: 3
            anchors.rightMargin: 3
            anchors.bottomMargin: 3
        }
    }

    Rectangle {
        color: backgroundColor
        anchors.fill: titleText
    }

    Text {
        id: titleText
        anchors.left: checkBoxGroup.left
        anchors.leftMargin: 6

        font.bold: true
    }

}
