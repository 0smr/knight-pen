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

    property alias canvas: canvasWindow.canvas
    property alias flow: mainLayout.flow

    CanvasWindow {
        id: canvasWindow
        canvas.strokeColor: colorValue.color
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
                onCheckedChanged: {
                    Window.window.setMaskArea(1, 10, 10, 10, checked);
                }

                Hamburger {
                    palette.mid: parent.palette.buttonText
                    checked: parent.checked
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
                }

                Grid {
                    flow: options.subflow
                    spacing: options.spacing

                    Button {
                        id: drawTools;
                        text: '\ue027';
                        checkable: true
                        onCheckedChanged: Window.window.setMaskRow(2, checked)
                    }

                    Grid {
                        flow: options.subflow
                        spacing: options.spacing
                        visible: drawTools.checked
                        itemPerRow: 1

                        Button { text: '\ue000'; onClicked: canvas.selectedTool = NanoCanvas.Rectangle } // rectangle
                        // Button { text: '\uf312'; onClicked: canvas.selectedTool = NanoCanvas.Polygon } // Polygon
                        Button { text: '\ue003'; onClicked: canvas.selectedTool = NanoCanvas.Ellipse } // ellipse
                        Button { text: '\ue005'; onClicked: canvas.selectedTool = NanoCanvas.Line } // line
                        // Button { text: '\uf30b'; onClicked: canvas.selectedTool = NanoCanvas.Arrow } // arrow
                        Button { text: '\ue026'; onClicked: canvas.selectedTool = NanoCanvas.Path } // path

                        // Button { text: '\uf305'; } // selection
                        // Button { text: '\uf040'; } // marker
                        // Button { text: '\uf591'; } // pencil
                        // Button { text: '\uf245'; } // pen

                        Button { id: eraser; text: '\ue028'; }
                    }
                }

                Grid {
                    flow: options.subflow
                    spacing: options.spacing

                    Button {
                        id: colorValue;
                        checkable: true;
                        text: '\ue004'
                        onCheckedChanged: Window.window.setMaskRow(3, checked)
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
                    onClicked: canvas.clearCanvas();
                }

                Button {
                    id: lockInputs;
                    text: canvasWindow.acceptInputs ? '\ue074' : '\ue029';
                    onClicked: canvasWindow.toggleAcceptInput();
                }

                Button {
                    id: visibleCanvas;
                    text: canvasWindow.hidden ? '\ue061' : '\ue060';
                    onClicked: canvasWindow.toggleVisiblity();
                }

                Grid {
                    flow: options.subflow
                    spacing: options.spacing

                    Button {
                        id: operations;
                        text: '\ue054';
                        checkable: true
                        onCheckedChanged: Window.window.setMaskRow(7, checked)
                    }

                    Grid {
                        flow: options.subflow
                        spacing: options.spacing
                        visible: operations.checked
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
                        onCheckedChanged: Window.window.setMaskRow(8, checked)
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
                        Button { text: artworks.index+1; enabled: false }
                    }
                }

                Button { id: save; text: '\ue031'; }
                Button { id: copy; text: '\ue037'; }
            }
        }
}
