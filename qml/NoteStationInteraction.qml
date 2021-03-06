/**************************************************************************
**
**    Copyright (C) 2013 by Philip Schuchardt
**    www.cavewhere.com
**
**************************************************************************/

import QtQuick 2.0
import Cavewhere 1.0

BaseNoteStationInteraction {
    id: interaction

    property BasePanZoomInteraction basePanZoomInteraction
    property ImageItem imageItem

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if(pressedButtons == Qt.RightButton) {
                basePanZoomInteraction.panFirstPoint(Qt.point(mouse.x, mouse.y))
            }
        }

        onReleased: {
            if(mouse.button === Qt.LeftButton) {
                var notePoint = imageItem.mapQtViewportToNote(Qt.point(mouse.x, mouse.y))
                interaction.addStation(notePoint)
            }
        }

        onPositionChanged: {
            if(pressedButtons == Qt.RightButton) {
                basePanZoomInteraction.panMove(Qt.point(mouse.x, mouse.y))
            }
        }

        onWheel: basePanZoomInteraction.zoom(wheel.angleDelta.y, Qt.point(wheel.x, wheel.y))
    }

    HelpBox {
        id: stationHelpBox
        text: "Click to add new station"
    }
}
