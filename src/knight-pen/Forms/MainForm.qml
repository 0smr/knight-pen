import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import knight.pen.canvas 1.0

import '../Controls'
import '../Windows'

Control {
    id: control

    width: mainLayout.width
    height: mainLayout.height

    property alias canvas: canvasWindow.knightcanvas
    property alias flow: mainLayout.flow

    CanvasWindow {
        id: canvasWindow
        knightcanvas.strokeColor: colorValue.color
    }

    ColorPickerWindow {
        id: colorPickerWindow
        onColorSelected: {
            if(valid === true) {
                colorValue.color = selectedColor;
            }
            colorpickerButton.checked = false;
        }
    }

    /**
     * TODO: Move Buttons to seprate componnents.
     */
    contentItem:
        Grid {
            id: mainLayout

            spacing: 2
            flow: oriention.checked ? Grid.LeftToRight : Grid.TopToBottom

            Button {
                id: collapsebutton

                checkable: true
                checked: true
                tooltipText: checked ? 'collapse' : 'expand'
                onCheckedChanged: {
                    Window.window.setMaskArea(1, 10, 10, 10, checked);
                }

                Hamburger {
                    palette.mid: parent.palette.buttonText
                    state: parent.checked ? "collapse" : "expand"
                    anchors.fill: parent
                    anchors.margins: 5
                }
            }

            Grid {
                id: options
                flow: mainLayout.flow

                width: childrenRect.width
                height: visible ? childrenRect.height : 0

                visible: collapsebutton.checked
                spacing: 2

                // subgrid flows.
                property int subflow: flow == Grid.TopToBottom ?  Grid.LeftToRight : Grid.TopToBottom
                // subgrid alignment.
                property int subalign: flow == Grid.TopToBottom ?  Qt.AlignLeft : Qt.AlignBottom

                Button {
                    id: oriention;
                    text: '\ue030';
                    checkable: true
                    tooltipText: 'switch to ' + (checked ? 'vertical' : 'horizontal')
                }

                Grid {
                    flow: options.subflow
                    spacing: options.spacing

                    Button {
                        id: drawTools;
                        text: '\ue027';
                        checkable: true
                        tooltipText: checked ? 'draw tools -' : 'draw tools +'
                        onCheckedChanged: {
                            Window.window.setMaskRow(2, checked);
                        }
                    }

                    Grid {
                        flow: options.subflow
                        spacing: options.spacing
                        visible: drawTools.checked
                        itemPerRow: 1

                        Button { // rectangle
                            text: '\ue000';
                            tooltipText: 'rectangle shape'
                            onClicked: canvas.selectedTool = KnightCanvas.Rectangle
                        }
                        // Button { text: '\uf312'; onClicked: canvas.selectedTool = KnightCanvas.Polygon } // Polygon
                        Button { // ellipse
                            text: '\ue003';
                            tooltipText: 'ellipse shape'
                            onClicked: canvas.selectedTool = KnightCanvas.Ellipse
                        }
                        Button { // line
                            text: '\ue005';
                            tooltipText: 'line shape'
                            onClicked: canvas.selectedTool = KnightCanvas.Line
                        }
                        // Button { text: '\uf30b'; onClicked: canvas.selectedTool = KnightCanvas.Arrow } // arrow
                        Button { // path
                            text: '\ue026';
                            tooltipText: 'path tool'
                            onClicked: canvas.selectedTool = KnightCanvas.Path
                        }

                        Button { // selection
                            text: '\ue064';
                            tooltipText: 'select tool'
                            onClicked: canvas.selectedTool = KnightCanvas.Selection
                        }
                        // Button { text: '\ue024'; } // marker
                        // Button { text: '\ue022'; } // pencil
                        Button { // pen
                            text: '\ue023';
                            tooltipText: 'pen';
                            // onClicked: canvas.selectedTool = KnightCanvas.
                        }
                        // Button { id: eraser; tooltipText: 'eraser'; text: '\ue028'; }
                        Label { text: canvas.strokeWidth.toFixed(1); }
                    }
                }

                Grid {
                    flow: options.subflow
                    spacing: options.spacing

                    Button {
                        id: colorValue;
                        checkable: true;
                        text: '\ue004'
                        tooltipText: 'selected color'
                        onCheckedChanged: {
                            Window.window.setMaskRow(3, checked);
                        }
                    }

                    Grid {
                        flow: parent.flow
                        visible: colorValue.checked
                        spacing: parent.spacing
                        itemPerRow: 1

                        Grid {
                            flow: parent.flow
                            spacing: 1
                            itemPerRow: 2

                            Button { width: 12; bgColor: 'black'; onClicked: colorValue.color = bgColor; }
                            Button { width: 12; bgColor: 'white'; onClicked: colorValue.color = bgColor; }
                            Button { width: 12; bgColor: '#00ADB5'; onClicked: colorValue.color = bgColor; }
                            Button { width: 12; bgColor: 'springgreen'; onClicked: colorValue.color = bgColor; }
                            Button { width: 12; bgColor: 'violet'; onClicked: colorValue.color = bgColor; }
                            Button { width: 12; bgColor: 'gold'; onClicked: colorValue.color = bgColor; }
                            Button { width: 12; bgColor: 'tomato'; onClicked: colorValue.color = bgColor; }
                            Button { width: 12; bgColor: 'deeppink'; onClicked: colorValue.color = bgColor; }
                            Button { width: 12; bgColor: 'darkblue'; onClicked: colorValue.color = bgColor; }
                            Button { width: 12; bgColor: 'dodgerblue'; onClicked: colorValue.color = bgColor; }
                        }

                        Button {
                            id: colorpickerButton;
                            text: '\ue020';
                            checkable: true
                            tooltipText: 'screen color picker'
                            onClicked: {
                                if(checked) {
                                    colorPickerWindow.activate();
                                } else {
                                    colorPickerWindow.hide();
                                }
                            }
                        }

                        /**
                         * Button { id: hsvaColorPicker }
                         */
                    }
                }


                Button {
                    id: trash; text: '\ue073';
                    tooltipText: 'clear all canvas'
                    onClicked: canvas.clearCanvas();
                }

                Button {
                    id: lockInputs;
                    text: canvasWindow.acceptInputs ? '\ue074' : '\ue029';
                    tooltipText: 'switch to ' + (canvasWindow.acceptInputs ? 'mouse mode' : 'canvas mode');
                    onClicked: canvasWindow.toggleAcceptInput();
                }

                Button {
                    id: visibleCanvas;
                    text: canvasWindow.hidden ? '\ue061' : '\ue060';
                    tooltipText: canvasWindow.hidden ? 'canvas is hidden' : ''
                    onClicked: {
                        canvasWindow.toggleVisiblity();
                    }
                }

                Grid {
                    flow: options.subflow
                    spacing: options.spacing

                    Button {
                        id: history;
                        text: '\ue054';
                        checkable: true
                        enabled: false
                        tooltipText: 'history'
                        onCheckedChanged: {
                            Window.window.setMaskRow(7, checked);
                        }
                    }

                    Grid {
                        flow: options.subflow
                        spacing: options.spacing
                        visible: history.checked
                        itemPerRow: 1

                        Button { id: redo; text: '\ue056'; }
                        Button { id: undo; text: '\ue055'; }
                    }
                }


                Grid {
                    flow: options.subflow
                    spacing: options.spacing

                    Button {
                        id: artworks;
                        property int index: 0
                        text: '\ue033';
                        checkable: true
                        enabled: false
                        tooltipText: 'recent canvas'
                        onCheckedChanged: {
                            Window.window.setMaskRow(8, checked)
                        }
                    }

                    Grid {
                        flow: options.subflow
                        spacing: options.spacing
                        visible: artworks.checked
                        layoutDirection: Qt.RightToLeft

                        Grid {
                            flow: options.subflow
                            spacing: 1
                            itemPerRow: 2
                            Button { width: 12; text: '0'; onClicked: artworks.index = text }
                            Button { width: 12; text: '1'; onClicked: artworks.index = text }
                            Button { width: 12; text: '2'; onClicked: artworks.index = text }
                            Button { width: 12; text: '3'; onClicked: artworks.index = text }
                            Button { width: 12; text: '4'; onClicked: artworks.index = text }
                            Button { width: 12; text: '5'; onClicked: artworks.index = text }
                            Button { width: 12; text: '6'; onClicked: artworks.index = text }
                            Button { width: 12; text: '7'; onClicked: artworks.index = text }
                        }

                        Button { text: '\ue048'; onPressed: artworks.index = (artworks.index + 1) % 8 }
                        Button { text: '\ue047'; onPressed: artworks.index = (artworks.index + 7) % 8 }
                        Button { text: '\ue050'; }
                        Label { text: artworks.index + 1; }
                    }
                }

                Button { id: save; text: '\ue031'; enabled: false }
                Button { id: copy; text: '\ue037'; enabled: false }
            }
        }
}
