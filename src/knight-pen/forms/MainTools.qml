import QtQuick 2.15
import QtQuick.Window 2.15
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Qt.labs.settings 1.1

import knight.pen.canvas 1.0

import '../controls'
import '../windows'

Control {
    id: control

    width: mainLayout.width
    height: mainLayout.height

    property alias canvasWindow: canvasWindow
    property alias canvas: canvasWindow.knightcanvas
    property alias flow: mainLayout.flow

    CanvasWindow {
        id: canvasWindow
        knightcanvas.strokeColor: colorValue.color
        knightcanvas.onStrokeColorChanged: {
            // change color on selecting a shape.
            if(knightcanvas.strokeColor != colorValue.color) {
                colorValue.color = knightcanvas.strokeColor
            }
        }
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

    // TODO: Move Buttons to seprate componnents.
    contentItem: Grid {
        id: mainLayout

        flow: oriention.checked ? Grid.LeftToRight : Grid.TopToBottom

        onPositioningComplete: Window.window.update() // NOTE: to update maskWindow

        ToolButton {
            id: collapsebutton

            checkable: true
            checked: true
            tooltipText: checked ? 'collapse' : 'expand'

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

            // subgrid flows.
            property int subflow: flow == Grid.TopToBottom ?  Grid.LeftToRight : Grid.TopToBottom

            GridSeprator {}
            ToolButton {
                id: oriention;
                text: '\ue030';
                checkable: true
                tooltipText: 'switch to ' + (checked ? 'vertical' : 'horizontal')
            }

            GridSeprator {}
            Grid {
                flow: options.subflow

                ToolButton {
                    id: drawTools;
                    text: '\ue007';
                    checkable: true
                    tooltipText: checked ? 'draw tools -' : 'draw tools +'
                }

                Grid {
                    flow: parent.flow
                    width: childrenRect.width
                    height: childrenRect.height
                    visible: drawTools.checked
                    // update mask window.
                    onVisibleChanged: Window.window.update() // NOTE: to update maskWindow

                    GridSeprator {}
                    ToolButton { // rectangle
                        text: '\ue000'
                        tooltipText: 'rectangle shape'
                        checked: true
                        checkable: true
                        autoExclusive: true
                        onClicked: canvas.selectedTool = KnightCanvas.Rectangle
                    }

                    GridSeprator {}
                    ToolButton { // ellipse
                        text: '\ue003'
                        tooltipText: 'ellipse shape'
                        checkable: true
                        autoExclusive: true
                        onClicked: canvas.selectedTool = KnightCanvas.Ellipse
                    }

                    GridSeprator {}
                    ToolButton { // line
                        id: lineShapeTool
                        text: '\ue005'
                        tooltipText: 'line shape'
                        checkable: true
                        autoExclusive: true
                        onClicked: canvas.selectedTool = KnightCanvas.Line
                    }

                    GridSeprator {}
                    ToolButton { // path
                        text: '\ue026'
                        tooltipText: 'path tool'
                        checkable: true
                        autoExclusive: true
                        onClicked: canvas.selectedTool = KnightCanvas.Path
                    }

                    GridSeprator {}
                    ToolButton { // selection
                        text: '\ue065'
                        tooltipText: 'select tool'
                        checkable: true
                        autoExclusive:  true
                        onClicked: canvas.selectedTool = KnightCanvas.Selection
                    }

                    GridSeprator {}
                    /*
                    ToolButton { text: '\uf312'; onClicked: canvas.selectedTool = KnightCanvas.Polygon } // Polygon
                    ToolButton { text: '\uf30b'; onClicked: canvas.selectedTool = KnightCanvas.Arrow } // arrow
                    ToolButton { text: '\ue024'; } // marker
                    ToolButton { text: '\ue022'; } // pencil
                    ToolButton { // pen
                        text: '\ue023';
                        tooltipText: 'pen'
                        checkable: true
                        autoExclusive: true
                        // onClicked: canvas.selectedTool = KnightCanvas.
                    }
                    ToolButton { id: eraser; tooltipText: 'eraser'; text: '\ue028'; }
                    */
                    ToolLabel {
                        text: canvas.strokeWidth.toFixed(1)
                        palette.button: text == '0.0' ? '#ecc' : '#eee'
                    }
                }
            }

            GridSeprator {}
            Grid {
                flow: options.subflow

                ToolButton {
                    id: colorValue
                    checkable: true
                    text: '\ue004'
                    tooltipText: 'selected color'
                    palette {
                        highlight: color
                    }
                }

                Grid {
                    flow: parent.flow
                    width: childrenRect.width
                    height: childrenRect.height
                    visible: colorValue.checked
                    // update mask window.
                    onVisibleChanged: Window.window.update() // NOTE: to update maskWindow

                    Grid {
                        flow: parent.flow
                        preferredRows: 2
                        spacing: parent.spacing/2
                        ToolButton { width: 14; palette.button: 'black'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: 'white'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: '#00ADB5'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: 'springgreen'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: 'violet'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: 'gold'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: 'tomato'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: 'deeppink'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: 'darkblue'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: 'dodgerblue'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: '#7f7f7f'; onClicked: colorValue.color = palette.button; }
                        ToolButton { width: 14; palette.button: '#99d9ea'; onClicked: colorValue.color = palette.button; }
                    }

                    ToolButton {
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
                    /// ToolButton { id: hsvaColorPicker }
                }
            }

            GridSeprator {}
            ToolButton {
                id: trash; text: '\ue073'
                tooltipText: 'clear all canvas'
                onClicked: canvas.clearCanvas()
            }

            GridSeprator {}
            ToolButton {
                id: lockInputs
                text: canvasWindow.acceptInputs ? '\ue074' : '\ue029'
                tooltipText: 'switch to ' + (canvasWindow.acceptInputs ? 'mouse mode' : 'canvas mode')
                onClicked: canvasWindow.toggleAcceptInput();
            }

            GridSeprator {}
            ToolButton {
                id: visibleCanvas;
                text: canvasWindow.hidden ? '\ue061' : '\ue060';
                tooltipText: canvasWindow.hidden ? 'hide canvas' : 'show canvas'
                onClicked: {
                    canvasWindow.toggleVisiblity();
                }
            }

            Grid {
                flow: options.subflow
                visible: false

                ToolButton {
                    id: history;
                    text: '\ue054'
                    checkable: true
                    enabled: false
                    tooltipText: 'history'
                }

                Grid {
                    flow: parent.flow
                    visible: history.checked

                    ToolButton { id: redo; text: '\ue056'; }
                    ToolButton { id: undo; text: '\ue055'; }
                }
            }

            Grid {
                flow: options.subflow
                visible: false

                ToolButton {
                    id: artworks;
                    property int index: 0
                    text: '\ue033';
                    checkable: true
                    enabled: false
                    tooltipText: 'recent canvas'
                }

                Grid {
                    flow: parent.flow
                    visible: artworks.checked
                    layoutDirection: Qt.RightToLeft

                    Grid {
                        flow: parent.flow
                        preferredRows: 2
                        spacing: parent.spacing/2
                        ToolButton { width: 14; text: '0'; onClicked: artworks.index = text }
                        ToolButton { width: 14; text: '1'; onClicked: artworks.index = text }
                        ToolButton { width: 14; text: '2'; onClicked: artworks.index = text }
                        ToolButton { width: 14; text: '3'; onClicked: artworks.index = text }
                        ToolButton { width: 14; text: '4'; onClicked: artworks.index = text }
                        ToolButton { width: 14; text: '5'; onClicked: artworks.index = text }
                        ToolButton { width: 14; text: '6'; onClicked: artworks.index = text }
                        ToolButton { width: 14; text: '7'; onClicked: artworks.index = text }
                    }

                    ToolButton { text: '\ue048'; onPressed: artworks.index = (artworks.index + 1) % 8 }
                    ToolButton { text: '\ue047'; onPressed: artworks.index = (artworks.index + 7) % 8 }
                    ToolButton { text: '\ue050'; }
                    Label { text: artworks.index + 1; }
                }
            }

            ToolButton { id: save; visible: false; text: '\ue031'; enabled: false }
            ToolButton { id: copy; visible: false; text: '\ue037'; enabled: false }
        }
    }

    Settings {
        category: 'Application'
        fileName: 'config.ini'

        //property alias uiTourIndex: uiTour.index
    }

    UITour {
        id: uiTour
        messages: [
            { target: drawTools, text: "<big>Hi there</big>,<br>" +
                                       "I'm delighted to see you're using my app,<br>" +
                                       "to begin drawing, first open the tool pan." },
            { target: lineShapeTool, text: "Good, now pick a drawing tool. &rarr;" },
            { target: visibleCanvas, text: "Okay, now click here and begin drawing; good luck!;. &nbsp;&rarr;" },
        ]

        Timer {
            running: true
            interval: 300
            onTriggered: {
                if(uiTour.index < uiTour.messages.length && control.visible && control.enabled) {
                    uiTour.showFullScreen();
                    uiTour.showNextMessage();
                }
            }
        }
    }
}
