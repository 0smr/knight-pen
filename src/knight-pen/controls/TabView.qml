import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import '.'

Row {
    id: control

    default property alias items: stacklayout.children
    property alias tabs: repeater.model
    property alias currentIndex: sideBar.currentIndex

    spacing: 5
    padding: 5

    children: [
        Column {
            id: sideBar

            spacing: 5
            height: parent.height
            width: childrenRect.width
            clip: true

            property real currentIndex: 0

            Repeater {
                id: repeater
                TabButton {
                    text: modelData.text ?? modelData
                    iconx.font.family: 'knight'
                    iconx.text: modelData.icon ?? ""

                    font.family: 'Calibri'
                    font.pointSize: 9

                    display: control.width < 400 ?
                                 TabButton.IconOnly : TabButton.TextBesideIcon
                    width: implicitWidth
                }
            }

            Behavior on width { SmoothedAnimation {} }
        },
        GridSeprator { length: parent.height - parent.padding * 2; fill: 1 },
        StackLayout {
            id: stacklayout
            width: parent.width - sideBar.width -
                   (control.spacing + control.padding) * 2
            height: parent.height - 20
            currentIndex: sideBar.currentIndex
        }
    ]
}
