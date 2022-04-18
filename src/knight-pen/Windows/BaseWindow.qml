import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import '../controls'

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

    BoxShadow {
        anchors.fill: parent
        visible: !(window.visibility === Window.Maximized)
        spread: window.active ? 10 : 15
        color: "#55000000"
        radius: 5

        Behavior on spread { NumberAnimation { duration: 110 } }
    }

    FontLoader {
        id: knightFont
        source: '../resources/Font/knight-icon-solid.ttf'
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
                    window.visibility === Window.Maximized ?
                                window.showNormal() : window.showMaximized();
                }
            }

            Row {
                anchors.fill: parent

                Text {
                    id: logo
                    height: parent.height
                    leftPadding: 3
                    color: '#ddd'
                    text: '\ue006'
                    font.family: knightFont.name
                    verticalAlignment: Qt.AlignVCenter
                }

                Text {
                    id: title
                    height: parent.height
                    leftPadding: 3
                    color: '#ddd'
                    text: 'Knight Pen'
                    font.family: 'Calibri'
                    verticalAlignment: Qt.AlignVCenter
                }

                Item {
                    width: header.width - title.width - logo.width - 20
                    height: parent.height
                }

                AbstractButton {
                    id: closeBtn
                    width: 20
                    height: parent.height
                    text: 'x'
                    contentItem: Text {
                        width: 20; text: 'x'; color: "#eee"
                        font.family: knightFont.name
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                    }
                    background: Rectangle { color: closeBtn.hovered ? '#f44' : '#353637' }
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
