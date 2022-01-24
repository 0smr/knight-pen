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

ApplicationWindow {
    id: window

    property var externalItems: []

    width: mainform.width
    height: mainform.height

    visible: true
    flags: Qt.WindowStaysOnTopHint |
           Qt.FramelessWindowHint |
           Qt.Widget
    color: 'transparent'

    contentData:
        MainForm {
            id: mainform
        }

    /**
     * FIXME: use window.startSystemMove() instead of manually change the window position.
     * FIXME: fix draging window using mouse left button.
     */
    MouseArea {
        property point clickPoint
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        cursorShape: containsPress ? Qt.ClosedHandCursor : Qt.ArrowCursor
        onPositionChanged: {
            if(drag.active) {
                var p = mapFromGlobal(Qt.point(-mouseX,-mouseY))
                window.x = -p.x + clickPoint.x
                window.y = -p.y + clickPoint.y
                for(var item of externalItems) {
                    if(item.visible) {
                        item.updateExternalWindowPosition();
                    }
                }
            }
        }

        drag.target: this
        drag.onActiveChanged: clickPoint = Qt.point(-mouseX,-mouseY)
        drag.threshold: 0
    }

    SystemTrayIcon {
        visible: true
        iconName: 'test'
        iconSource: "Resources/Icons/icon-light.svg"
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
