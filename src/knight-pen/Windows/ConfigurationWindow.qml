import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import '../Forms'

BaseWindow {
    id: window

    Page {
        implicitWidth: parent.width
        anchors.fill: parent
        clip: true
        header: TabBar {
            id: headBar
            spacing: 0
            contentHeight: 20
            implicitWidth: availableWidth
            currentIndex: 1

            Repeater {
                model: ["Configuration", "Updater"]
                TabButton {
                    text: modelData
                    width: Math.max(implicitWidth, headBar.width / parent.count);
                }
            }
        }

        StackLayout {
            anchors.fill: parent
            currentIndex: headBar.currentIndex

            ConfigurationForm {
                width: parent.width
                height: parent.height
            }

            UpdaterForm {
                width: parent.width
                height: parent.height
            }
        }
    }
}
