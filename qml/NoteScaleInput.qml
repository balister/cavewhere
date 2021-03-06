/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

import QtQuick 2.0
import Cavewhere 1.0
import "Utils.js" as Utils

Item {
    id: scaleInput

    property NoteTransform noteTransform
    property HelpArea scaleHelp
    property bool autoScaling: false

    signal scaleInteractionActivated()

    height: childrenRect.height
    width: inputRow.width

    Row {
        id: inputRow
        spacing: 5

        Button {
            id: setLength
            anchors.verticalCenter: parent.verticalCenter
            width: 24
            visible: !autoScaling
            onClicked: scaleInteractionActivated()
        }

        Row {
            spacing: 3
            anchors.verticalCenter: parent.verticalCenter

            LabelWithHelp {
                id: scaleLabelId
                helpArea: scaleHelp
                text: "Scale"
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {

                anchors.verticalCenter: parent.verticalCenter
                radius: 5

                width: childrenRect.width + columnOnPaper.x * 2.0
                height: childrenRect.height

                Column {
                    id: columnOnPaper
                    x: 3

                    Text {
                        id: onPaperId
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "On Paper"
                    }

                    UnitValueInput {
                        id: onPaperLengthInput
                        anchors.horizontalCenter: parent.horizontalCenter
                        unitValue: null
                        valueVisible: false
                        valueReadOnly: autoScaling
                        defaultUnit: Units.LengthUnitless
                    }
                }
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "="
            }

            Rectangle {

                anchors.verticalCenter: parent.verticalCenter
                radius: 5

                width: childrenRect.width + columnInCave.x * 2.0
                height: childrenRect.height

                Column {
                    id: columnInCave
                    x: 3

                    Text {
                        id: inCaveId
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "In Cave"
                    }

                    UnitValueInput {
                        id: inCaveLengthInput
                        anchors.horizontalCenter: parent.horizontalCenter
                        unitValue: null
                        valueVisible: false
                        valueReadOnly: autoScaling
                        defaultUnit: Units.LengthUnitless
                    }
                }
            }
        }


        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: "="
        }

        Text {
            id: scaleText
            anchors.verticalCenter: parent.verticalCenter
            visible: !errorText.visible
            text: ""
        }

        Text {
            id: errorText
            color: "red"
            anchors.verticalCenter: parent.verticalCenter
            visible: false
            text: "Weird scaling units"
            font.italic: true
            font.bold: true
        }
    }

    states: [
        State {
            when: noteTransform !== null

            PropertyChanges {
                target: onPaperLengthInput
                unitValue: noteTransform.scaleNumerator
                valueVisible: (!autoScaling || noteTransform.scaleNumerator.unit !== Units.Unitless) && !errorText.visible
            }

            PropertyChanges {
                target: inCaveLengthInput
                unitValue: noteTransform.scaleDenominator
                valueVisible: (!autoScaling || noteTransform.scaleDenominator.unit !== Units.Unitless) && !errorText.visible
            }

            PropertyChanges {
                target: scaleText
                text: "1:" + Utils.fixed(1 / noteTransform.scale, 1)
            }

            PropertyChanges {
                target: errorText
                visible: (noteTransform.scaleDenominator.unit === Units.Unitless ||
                          noteTransform.scaleNumerator.unit === Units.Unitless) &&
                         !(noteTransform.scaleDenominator.unit === Units.Unitless &&
                          noteTransform.scaleNumerator.unit === Units.Unitless)

            }
        }

    ]

}
