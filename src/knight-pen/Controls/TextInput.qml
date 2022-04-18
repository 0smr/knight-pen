import QtQuick 2.15 as QQ

QQ.TextInput {
    id: control
    width: 40
    height: implicitHeight

    clip: true

    font.family: 'Calibri'

    color: '#444'
    selectionColor: '#11000000'
    selectedTextColor: color
    selectByMouse: true

    horizontalAlignment: Qt.AlignHCenter
    cursorDelegate: QQ.Rectangle { color: '#55000000'; width: 1 }

    QQ.Rectangle {
        width: parent.width
        height: 0.8
        color: parent.color
        anchors.bottom: parent.bottom
        opacity: parent.focus ? 0.3 : 0.1
    }
}
