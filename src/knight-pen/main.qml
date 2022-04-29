import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.platform 1.1
import QtQml 2.15

import knight.pen.window 1.0
import knight.pen.utils 1.0

import 'windows'
import 'controls'
import 'forms'

MaskWindow {
    id: window

    x: Screen.width - width - 20
    width: 308; height: 308
    sectorSize: Qt.size(width/11, height/11)
    visible: true

    Item {
        anchors.fill: parent
        MainTools {
            id: mainform
            anchors { top: parent.top; right: parent.right }
            anchors.onRightChanged: window.update() // NOTE: to update maskWindow
        }
    }

    HelpWindow { id: help }
    ConfigurationWindow { id: configuration }

    // Drags main window.
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
        id: tryIcons
        visible: true
        icon.source: {
            return Utils.osLightModeEnabled() ? "resources/icons/icon-dark.svg" :
                                                "resources/icons/icon-light.svg";
        }
        tooltip: "%1\nstatus: %2\ncanvas: %3"
                        .arg("Knight pen v0.2.0")
                        .arg(window.visible ? qsTr("hidden") : qsTr("visible"))
                        .arg(mainform.canvasWindow.acceptInputs ? qsTr("on") : qsTr("off"));

        onActivated: {
            if(reason === SystemTrayIcon.Trigger) {
                window.visible = true;
                window.show();
            }
        }

        menu: Menu {
            MenuItem {
                text: window.visible ? qsTr("Hide") : qsTr("Show")
                onTriggered: window.visible = !window.visible;
            }

            MenuItem {
                text: qsTr("Save")
                onTriggered: help.visible = true;
                enabled: false
            }

            MenuItem {
                text: qsTr("Configuration")
                onTriggered: configuration.visible = true;
            }

            MenuItem {
                text: qsTr("Update Available")
                visible: configuration.updateAssetUrl
                onTriggered: {
                    configuration.visible = true;
                    configuration.tabIndex = 1;
                }
            }

            MenuItem {
                text: qsTr("Help")
                onTriggered: help.visible = true;
            }

            MenuItem {
                text: qsTr("Quit")
                onTriggered: Qt.quit();
            }
        }
    }
}
