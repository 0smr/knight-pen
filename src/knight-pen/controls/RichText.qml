import QtQuick 2.15
import QtQuick.Controls 2.15

TextArea {
    id: control

    selectionColor: '#33000000'

    padding: 7
    topPadding: 12

    readOnly: true
    selectByMouse: true

    wrapMode: Text.WordWrap
    textFormat: Text.RichText
    horizontalAlignment: Text.AlignJustify

    font: KnightPen.regularFont

    onLinkActivated: Qt.openUrlExternally(link);

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.NoButton
        cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
    }
}
