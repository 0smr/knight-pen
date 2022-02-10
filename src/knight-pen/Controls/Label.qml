import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC

QQC.Label {
    id: control
    width: 25
    height: width
    text: '0'
    color: '#999'
    verticalAlignment: Qt.AlignVCenter
    horizontalAlignment: Qt.AlignHCenter
    palette { base: '#eee'; button: '#eee' }

    background: Rectangle {
        color: control.palette.button
        border {
            color: control.checked ? '#89c7f5' : Qt.darker(control.palette.button, 1.3)
            width: control.focus ? 2 : 1
            Behavior on width { NumberAnimation {duration: 100} }
        }
    }
}
