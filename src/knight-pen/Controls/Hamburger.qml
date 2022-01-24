import QtQuick 2.15
import QtQuick.Templates 2.15 as T

T.Control {
    id: control

    property bool checked: false
    spacing: 0.1

    contentItem: Item {
        Rectangle {
            id: sec1
            y: control.checked ? sec2.y : parent.height * control.spacing/2
            width: parent.width; height: sec2.height
            color: control.palette.mid
            radius: width
            Behavior on y { NumberAnimation{duration: 100} }
        }

        Rectangle {
            id: sec2
            y: parent.height * (0.33 + control.spacing/2)
            width: parent.width; height: parent.height * (0.33 - control.spacing)
            color: control.palette.mid
            radius: width
        }

        Rectangle {
            id: sec3
            y: control.checked ? sec2.y : parent.height * (0.66 + control.spacing/2)
            width: parent.width; height: sec2.height
            color: control.palette.mid
            radius: width
            Behavior on y { NumberAnimation{duration: 100} }
        }
    }
}
