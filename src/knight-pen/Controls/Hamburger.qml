import QtQuick 2.15
import QtQuick.Templates 2.15 as T

T.Control {
    id: control

    spacing: 0.2

    QtObject {
        id: items
        readonly property real width: control.availableWidth
        readonly property real height: control.availableHeight * (0.33 - control.spacing/2)
        readonly property real xcenter: control.availableWidth / 2
        readonly property real ycenter: control.availableHeight * (0.33 + control.spacing/2)
    }

    state: "expand"

    states: [
        State {
            name: "expand"
            PropertyChanges {
                target: sec1
                y: parent.height * control.spacing/2
                width: items.width
            }
            PropertyChanges {
                target: sec2
                y: items.ycenter
                width: items.width
            }
            PropertyChanges {
                target: sec3
                y: parent.height * (0.66 + control.spacing/2)
                width: items.width
            }
        },
        State {
            name: "collapse"
            PropertyChanges {
                target: sec1
                y: items.ycenter
                width: items.width
            }
            PropertyChanges {
                target: sec2
                y: items.ycenter
                width: items.width
            }
            PropertyChanges {
                target: sec3
                y: items.ycenter
                width: items.width
            }
        },
        State {
            name: "close"
            PropertyChanges {
                target: sec1
                x: items.xcenter - width/2
                y: items.ycenter
                width: items.height
            }
            PropertyChanges {
                target: sec2
                x: items.xcenter - width/2
                y: items.ycenter
                width: items.height
            }
            PropertyChanges {
                target: sec3
                x: items.xcenter - width/2
                y: items.ycenter
                width: items.height
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"; to: "*"
            SmoothedAnimation {
                property: "y"
                velocity: 50
            }
        }
    ]

    contentItem: Item {
        Rectangle {
            id: sec1
            height: items.height
            color: control.palette.mid
            radius: width
        }

        Rectangle {
            id: sec2
            height: items.height
            color: control.palette.mid
            radius: width
        }

        Rectangle {
            id: sec3
            height: items.height
            color: control.palette.mid
            radius: width
        }
    }
}
