import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.settings 1.1

import '../controls'

BaseForm {
    id: control

    Settings {
        id: settings
        category: "Application"
        fileName: "config.ini"
        property alias scaleFactor: scaleFactor.text
    }

    Grid {
        anchors.fill: parent
        padding: 5

        layoutDirection: Qt.LeftToRight
        flow: control.width < control.height * 1.5 ? Grid.TopToBottom : Grid.LeftToRight

        Row {
            spacing: 10
            width: childrenRect.width

            Text {
                text: 'Application Global Scale Ratio:'
                font.family: 'Calibri'
                font.pointSize: 9
            }

            TextInput {
                id: scaleFactor
                width: 40
                text: '1.0'
                inputMask: '0.0'
            }
        }

        Grid {
            width: childrenRect.width
            layoutDirection: Qt.LeftToRight
        }
    }
}
