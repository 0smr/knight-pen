import QtQuick 2.15
import knight.pen.canvas 1.0

Item {
    id: control
    visible: true

    property alias knightcanvas: knightcanvas
    property alias mousearea: cvmousearea

    KnightCanvas {
        id: knightcanvas
        anchors.fill: parent
        selectedTool: KnightCanvas.Rectangle
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
                knightcanvas.scaleFactor += wheel.angleDelta.x / 120 * 0.2; // 20 precent zooming
            } else {
                const multi = wheel.modifiers == Qt.ControlModifier ? 0.1 : 1.0;
                knightcanvas.strokeWidth += wheel.angleDelta.y / 120 * multi
            }
        }

        onPositionChanged: {
            knightcanvas.mouse = Qt.point(mouseX, mouseY);
        }

        onPressed: {

            if(mouse.button == Qt.RightButton) {
                if(knightcanvas.selectedTool === KnightCanvas.Path){
                    knightcanvas.stopDrawing();
                }
            } else {
                if(knightcanvas.drawing === false){
                    knightcanvas.clearSelection();
                }

                switch(knightcanvas.selectedTool) {
                case KnightCanvas.Path:
                    knightcanvas.addPathPoint(Qt.point(mouseX, mouseY));
                    break;
                case KnightCanvas.Rectangle:
                    knightcanvas.addRect(Qt.point(mouseX, mouseY));
                    break;
                case KnightCanvas.Line:
                    knightcanvas.addLine(Qt.point(mouseX, mouseY));
                    break;
                case KnightCanvas.Ellipse:
                    knightcanvas.addEllipse(Qt.point(mouseX, mouseY));
                    break;
                }
            }
            knightcanvas.mouse = Qt.point(mouseX, mouseY);
        }

        onReleased: {
            if(knightcanvas.selectedTool !== KnightCanvas.Path) {
                knightcanvas.stopDrawing();
            }
        }
    }
}
