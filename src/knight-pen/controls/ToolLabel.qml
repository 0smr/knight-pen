import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC

QQC.Label {
    id: control
    width: 28
    height: width
    text: '0'
    font: KnightPen.regularFont
    color: '#999'
    verticalAlignment: Qt.AlignVCenter
    horizontalAlignment: Qt.AlignHCenter
    palette { base: '#eee'; button: '#eee' }
    background: Rectangle { color: control.palette.button }
}
