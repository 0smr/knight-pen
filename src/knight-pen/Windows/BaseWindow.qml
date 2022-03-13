import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import '../Controls' as CC

Window {
    id: window

    width: 400; height: 400
    color: 'transparent'
    flags: Qt.Window | Qt.FramelessWindowHint

    default property alias contentData: page.contentData

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton
        propagateComposedEvents: true

        property int edges: 0;
        property int edgeOff: 5;

        function setEdges(x, y) {
            edges = 0;
            if(x < edgeOff) edges |= Qt.LeftEdge;
            if(x > (width - edgeOff))  edges |= Qt.RightEdge;
            if(y < edgeOff) edges |= Qt.TopEdge;
            if(y > (height - edgeOff)) edges |= Qt.BottomEdge;
        }

        cursorShape: {
            return !containsMouse ? Qt.ArrowCursor:
                   edges == 3 || edges == 12 ? Qt.SizeFDiagCursor :
                   edges == 5 || edges == 10 ? Qt.SizeBDiagCursor :
                   edges & 9 ? Qt.SizeVerCursor :
                   edges & 6 ? Qt.SizeHorCursor : Qt.ArrowCursor;
        }

        onPositionChanged: setEdges(mouseX, mouseY);
        onPressed: {
            setEdges(mouseX, mouseY);
            if(edges && containsMouse) {
                startSystemResize(edges);
            }
        }
    }

    CC.BoxShadow {
        anchors.fill: parent
        visible: !(window.visibility === Window.Maximized)
        spread: window.active ? 10 : 15
        color: "#55000000"
        radius: 5

        Behavior on spread { NumberAnimation { duration: 110 } }
    }

    Page {
        id: page

        anchors.fill: parent
        anchors.margins: window.visibility === Window.Maximized ? 0 : 5

        header: Rectangle {
            id: header
            height: 20;
            clip: true
            color: "#353637";

            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    if(window.visibility === Window.Maximized)
                        window.showNormal();
                    else
                        window.showMaximized();
                }
            }

            Row {
                anchors.fill: parent

                Text {
                    id: title
                    width: implicitWidth
                    height: parent.height
                    leftPadding: 3
                    color: '#ddd'
                    text: '\ue006 Knight pen'
                    font.family: 'knight ico'
                    verticalAlignment: Qt.AlignVCenter
                }

                Item {
                    width: header.width - title.width - 20
                    height: parent.height
                }

                Button {
                    padding: 0
                    width: 20
                    height: parent.height

                    text: '\ue057'
                    font.family: 'knight ico'

                    hoverEnabled: true
                    palette {
                        button: hovered ? '#f44' : '#353637'
                        buttonText: '#eee'
                    }

                    onClicked: window.close();
                }
            }

            DragHandler {
                target: null
                dragThreshold: 1

                onActiveChanged: {
                    if(active) {
                        window.startSystemMove();
                    }
                }
            }
        }
    }
}
