import QtQuick 2.15 as QQ215

QQ215.Grid {
    id: control

    property int itemPerRow: 1

    flow: QQ215.Grid.TopToBottom
    columns: flow == QQ215.Grid.TopToBottom ? itemPerRow : -1
    rows: flow == QQ215.Grid.TopToBottom ? -1 : itemPerRow
    spacing: 5
    layoutDirection: Qt.RightToLeft
}
