import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import QtQml 2.15

import knight.pen.utils 1.0
import knight.pen.window 1.0

import 'Controls'
import 'Forms'

TileMaskWindow {
    id: window

    property var externalItems: []

    width: 300
    height: 300

    visible: true

    /// mask configuration begin
    rows: 11
    columns: 11
    transposed: mainform.flow == Grid.TopToBottom ?
                    TileMaskWindow.None : TileMaskWindow.SecondaryTranspose
    function setMaskRow(row, val) {
        let to = [0, 0, 7, 4, 0, 0, 0, 2, 6, 0, 0];
        window.setMaskArea(row, 10 - to[row], row, 9, val);
    }
    Component.onCompleted: { window.setMaskArea(0, 10, 10, 10); }
    /// mask configuration end

    Item {
        anchors.fill: parent
        MainForm {
            id: mainform
            anchors {
                top: parent.top
                right: parent.right
                rightMargin: 2
                topMargin: 2
            }
        }
    }

    DragHandler {
        target: null
        dragThreshold: 1
        onActiveChanged: {
            if(active) {
                window.startSystemMove();
            }
        }
    }

    SystemTrayIcon {
        visible: true
        icon.source: "Resources/Icons/icon-light.svg"
        onActivated: {
            if(reason === SystemTrayIcon.Trigger){
                window.visible = true
                window.show()
            }
        }

        menu: Menu {
            MenuItem {
                text: window.visible ? qsTr("Hide") : qsTr("Show")
                onTriggered: window.visible = !window.visible;
            }
            MenuItem { text: qsTr("Configuration") }
            MenuItem { text: qsTr("About") }
            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit();
            }
        }
    }
}
