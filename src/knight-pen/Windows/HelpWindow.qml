import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

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

            Repeater {
                model: ["About", "Change log", "Help", "FAQ"]
                TabButton {
                    text: modelData
                    width: Math.max(implicitWidth, headBar.width / parent.count)
                    enabled: index < 2
                }
            }
        }

        StackLayout {
            anchors.fill: parent
            currentIndex: headBar.currentIndex

            AboutForm {
                width: parent.width
                height: parent.height
            }

            ChangeLogForm {
                width: parent.width
                height: parent.height
            }

            HelpForm {

            }

            FAQForm {

            }
        }
    }
}
