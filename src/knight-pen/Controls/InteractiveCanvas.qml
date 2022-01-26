import QtQuick 2.15
import knight.pen.canvas 1.0

Item {
    id: control
    visible: true

    property alias nanocanvas: nanocanvas
    property alias mousearea: cvmousearea

    NanoCanvas {
        id: nanocanvas
        anchors.fill: parent
        selectedTool: NanoCanvas.Ellipse
        fillColor: '#555'
        strokeColor: '#555'
        lineCap: Qt.RoundCap
        lineJoin: Qt.RoundJoin
    }

    MouseArea {
        id: cvmousearea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true

        onWheel: {
            /**
             * TODO: Math.floor() strokeWidth in non modifier.
             */
            if(wheel.modifiers == Qt.AltModifier) {
                nanocanvas.scaleFactor += wheel.angleDelta.x / 120 * 0.2; // 20 precent zooming
            } else {
                const multi = wheel.modifiers == Qt.ControlModifier ? 0.1 : 1.0;
                nanocanvas.strokeWidth += wheel.angleDelta.y / 120 * multi
            }
        }

        onPositionChanged: {
            if(nanocanvas.selectedTool != NanoCanvas.None && containsPress ||
                    nanocanvas.selectedTool == NanoCanvas.Path) {
                nanocanvas.mouse = Qt.point(mouseX, mouseY);
            }
        }

        onPressed: {
            nanocanvas.clearSelection();
            if(mouse.button == Qt.RightButton) {
                if(nanocanvas.selectedTool == NanoCanvas.Path){
                    nanocanvas.stopDrawing();
                }
            } else {
                switch(nanocanvas.selectedTool) {
                case NanoCanvas.Path:
                    nanocanvas.addPathPoint(Qt.point(mouseX, mouseY));
                    break;
                case NanoCanvas.Rectangle:
                    nanocanvas.addRect(Qt.point(mouseX, mouseY));
                    break;
                case NanoCanvas.Line:
                    nanocanvas.addLine(Qt.point(mouseX, mouseY));
                    break;
                case NanoCanvas.Ellipse:
                    nanocanvas.addEllipse(Qt.point(mouseX, mouseY));
                    break;
                }
            }
            nanocanvas.mouse = Qt.point(mouseX, mouseY);
        }

        onReleased: {
            if(nanocanvas.selectedTool != NanoCanvas.Path) {
                nanocanvas.stopDrawing();
            }
        }
    }
}
