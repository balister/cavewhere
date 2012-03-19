import QtQuick 1.1

MouseArea {
    id: globalMouseArea

    property alias textInput: input
    property alias editor: shadowEditor
    property int minWidth: 0
    property int minHeight: 0
    property alias errorHelpBox: errorHelpBoxItem
    property CoreClickTextInput coreClickInput

    signal enterPressed()
    signal escapePressed()

    anchors.fill: parent
    enabled: false

    onPressed: {
        var commited = coreClickInput.commitChanges()
        if(!commited) {
            coreClickInput.closeEditor()
        }

        mouse.accepted = false
    }

    function clearSelection() {
        input.select(input.cursorPosition, input.cursorPosition)
    }

    ShadowRectangle {
        id: shadowEditor
        visible: false;

        color: "white"

        width:  minWidth > input.width + 6 ? minWidth : input.width  + 6
        height: minHeight > input.height + 6 ? minHeight : input.height + 6

        MouseArea {
            id: borderArea

            anchors.fill: parent

            onPressed: {
                input.forceActiveFocus()
                mouse.accepted = true
            }
        }

        TextInput {
            id: input
            visible: shadowEditor.visible
            anchors.centerIn: parent;

            selectByMouse: activeFocus;
            activeFocusOnPress: false

            function defaultKeyHandling(event) {
                if(event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                    enterPressed()
                    var commited = coreClickInput.commitChanges()
                    event.accepted = commited
                } else if(event.key === Qt.Key_Escape) {
                    escapePressed()
                    coreClickInput.closeEditor();
                    event.accepted = true
                }
            }

            Keys.onPressed: {
                defaultKeyHandling(event)
            }
        }

        ErrorHelpBox {
            id: errorHelpBoxItem
            y: parent.height + 10
            visible: false
            anchors.bottom: undefined
            anchors.bottomMargin: 0
        }
    }


}