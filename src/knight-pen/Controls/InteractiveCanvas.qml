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
        enabled: control.enabled

        onWheel: {
            if(wheel.modifiers == Qt.AltModifier) {
                // zoom is disabled for now.
                // knightcanvas.scaleFactor += wheel.angleDelta.x / 120 * 0.2; // 20 precent zooming
            } else {
                const multi = (wheel.modifiers == Qt.ControlModifier ? 0.1 : 1.0);
                knightcanvas.strokeWidth += wheel.angleDelta.y / 120 * multi;
                /**
                 * TODO: write a cleaner code.
                 */
                if(wheel.modifiers != Qt.ControlModifier) {
                    knightcanvas.strokeWidth = wheel.angleDelta.y < 0 ?
                                Math.ceil(knightcanvas.strokeWidth) :
                                Math.floor(knightcanvas.strokeWidth);
                }
            }
        }

        onPositionChanged: {
            knightcanvas.mouse = Qt.point(mouseX, mouseY);
        }

        onPressed: {
            let point = Qt.point(mouseX, mouseY);

            if(mouse.button == Qt.RightButton && knightcanvas.selectedTool === KnightCanvas.Path) {
                knightcanvas.stopDrawing();
            } else {
                if(knightcanvas.drawing === false){
                    knightcanvas.clearSelection();
                }

                switch(knightcanvas.selectedTool) {
                case KnightCanvas.Path:
                    knightcanvas.addPathPoint(point);
                    break;
                case KnightCanvas.Rectangle:
                    knightcanvas.addRect(point);
                    break;
                case KnightCanvas.Line:
                    knightcanvas.addLine(point);
                    break;
                case KnightCanvas.Ellipse:
                    knightcanvas.addEllipse(point);
                    break;
                case KnightCanvas.Selection:
                    knightcanvas.clearSelection();
                    knightcanvas.selectAt(point);
                    break;
                }
            }

            knightcanvas.mouse = point;
            control.forceActiveFocus();
        }

        onReleased: {
            if(knightcanvas.selectedTool !== KnightCanvas.Path) {
                knightcanvas.stopDrawing();
            }
        }
    }

    Keys.onDeletePressed: {
        // erase selected shapes.
        knightcanvas.eraseSelection();
    }

    Keys.onPressed: {
        switch(event.key) {}
    }
}
