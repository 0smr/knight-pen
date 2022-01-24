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

    property alias canvas: canvasWindow.canvas

    width: mainLayout.width
    height: mainLayout.height

    CanvasWindow {
        id: canvasWindow
        canvas.strokeColor: colorValue.color
    }

    ColorPickerWindow {
        id: colorPickerWindow
        onColorSelected: {
            if(valid === true) {
                colorValue.color = selectedColor
            }
            colorpickerButton.checked = false
        }
    }

    /**
     * TODO: Move Buttons to seprate componnents.
     */

    Grid {
        id: mainLayout

        spacing: 5
        flow: oriention.checked ? Grid.LeftToRight : Grid.TopToBottom

        Button {
            id: collapsebutton

            checkable: true
            checked: true

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
                text: checked ? '\uf141' : '\uf142';
                checkable: true
                onClicked: {
                    /**
                     * FIXME: external windows position don't update on oriention changes.
                     */
                    collapsebutton.checked = false
                    collapsebutton.checked = true
                }
            }

            Button {
                id: drawTools;
                text: '\uf618';
                checkable: true

                WinItem {
                    id: drawToolsSub
                    height: parent.height
                    visible: drawTools.checked
                    spacing: options.spacing
                    alignment: options.subalign
                    contentItem: Grid {
                        flow: options.subflow
                        spacing: options.spacing
                        itemPerRow: 1

                        Button { text: '\uf0c8'; onClicked: canvas.selectedTool = NanoCanvas.Rectangle } // rectangle
//                        Button { text: '\uf312'; onClicked: canvas.selectedTool = NanoCanvas.Polygon } // Polygon
                        Button { text: '\uf111'; onClicked: canvas.selectedTool = NanoCanvas.Ellipse } // ellipse
//                        Button { text: '\uf86c'; onClicked: canvas.selectedTool = NanoCanvas.Line } // line
//                        Button { text: '\uf30b'; onClicked: canvas.selectedTool = NanoCanvas.Arrow } // arrow
                        Button { text: '\uf55b'; onClicked: canvas.selectedTool = NanoCanvas.Path } // path

//                        Button { text: '\uf305'; } // selection
//                        Button { text: '\uf040'; } // marker
//                        Button { text: '\uf591'; } // pencil
//                        Button { text: '\uf245'; } // pen

                        Button { id: eraser; text: '\uf12d'; }
                    }
                }
            }

            Button {
                id: colorValue;
                checkable: true;
                text: '\uf45c'

                property var colorHistory: ["#eee",'#eee','#eee']

                onColorChanged: {
                    // if color is not white nor black or don't exist in color history
                    if( 0 < color.hslLightness && color.hslLightness < 1 &&
                            colorHistory.includes(color) === false) {
                        colorHistory.pop();
                        colorHistory.unshift(Qt.lighter(color,1));

                        colorHistory.forEach((colorItem, i) => {
                            colorHistoryGrid.children[i].color = colorItem;
                        });
                    }
                }

                WinItem {
                    id: colorValueSub
                    height: parent.height
                    visible: parent.checked
                    spacing: options.spacing
                    alignment: options.subalign
                    contentItem: Grid {
                        id: colorHistoryGrid
                        flow: options.subflow
                        spacing: options.spacing
                        itemPerRow: 1

                        Button { text: '\uf45c'; color: '#eee'; onClicked: colorValue.color = color; }
                        Button { text: '\uf45c'; color: '#eee'; onClicked: colorValue.color = color; }
                        Button { text: '\uf45c'; color: '#eee'; onClicked: colorValue.color = color; }
                        Button { text: '\uf45c'; color: 'black'; onClicked: colorValue.color = color; }
                        Button { text: '\uf45c'; color: 'white'; onClicked: colorValue.color = color; }
                    }
                }
            }

            Button {
                id: colorpickerButton;
                text: '\uf1fb';
                checkable: true
                onClicked: {
                    if(checked) {
                        colorPickerWindow.activate();
                    } else {
                        colorPickerWindow.hide();
                    }
                }
            }

            Button {
                id: trash; text: '\uf1f8';
                onClicked: canvas.clearCanvas();
            }
            Button {
                id: lockInputs;
                text: canvasWindow.acceptInputs ? '\uf023' : '\uf09c';
                onClicked: canvasWindow.toggleAcceptInput();
            }
            Button {
                id: visibleCanvas;
                text: canvasWindow.hidden ? '\uf070' : '\uf06e';
                onClicked: canvasWindow.toggleVisiblity();
            }

            Button {
                id: operations;
                text: '\uf364';
                checkable: true

                WinItem {
                    id: operationsSub
                    height: parent.height
                    visible: parent.checked
                    spacing: options.spacing
                    alignment: options.subalign
                    contentItem: Grid {
                        flow: options.subflow
                        spacing: options.spacing
                        itemPerRow: 1

                        Button { id: undo; text: '\uf2ea'; }
                        Button { id: redo; text: '\uf2f9'; }
                    }
                }
            }

            Button {
                id: artworks;
                property int index: 0
                text: '\uf009';
                checkable: true

                WinItem {
                    id: artworksSub
                    height: parent.height
                    visible: parent.checked
                    spacing: options.spacing
                    alignment: options.subalign
                    contentItem: Grid {
                        layoutDirection: Qt.RightToLeft
                        flow: options.subflow
                        spacing: options.spacing

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

                        Button { text: '\uf356'; onPressed: artworks.index = (artworks.index + 1) % 8 }
                        Button { text: '\uf355'; onPressed: artworks.index = (artworks.index + 7) % 8 }
                        Button { text: '\uf15b'; }
                        Button { text: String.fromCharCode(0x2776+artworks.index); enabled: false }
                    }
                }
            }

            Button { id: save; text: '\uf0c7'; }
            Button { id: copy; text: '\uf0ea'; }
        }
    }
}
