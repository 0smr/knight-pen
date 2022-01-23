import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import knight.pen.utils 1.0

Window {
    id: control

    color: 'transparent'
    visibility: Window.Hidden

    signal colorSelected(color selectedColor, bool valid);

    property color pickedColor: 'black'

    function activate() {
        showFullScreen()
        flags = Qt.Widget
    }
    /**
     * FIXME: Qt bug screen flashes black.
     * FIXME: screen goes black if windows sets to WindowStayOnTopHint and Fullscreen visiblity.
     * NOTE: color picker window sets under main application.
     */

    function rgbString(c) {
        return "%1,%2,%3".arg(Math.round(c.r * 255))
                         .arg(Math.round(c.g * 255))
                         .arg(Math.round(c.b * 255));
    }

    function hslString(c) {
        return "%1,%2,%3".arg(Math.round(Math.max(0, c.hslHue * 360)))
                         .arg(Math.round(c.hslSaturation * 255))
                         .arg(Math.round(c.hslLightness * 255));
    }

    function hsvString(c) {
        return "%1,%2,%3".arg(Math.round(Math.max(0, c.hsvHue * 360)))
                         .arg(Math.round(c.hsvSaturation * 255))
                         .arg(Math.round(c.hsvValue * 255));
    }

    Utils {
        id: utils
    }

    Item {
        id: indicator
        x: mousearea.mouseX
        y: mousearea.mouseY

        ToolTip {
            x: 5;
            visible: control.visible && mousearea.containsMouse
            background: Row {
                spacing: 1
                Rectangle {
                    width: 10
                    height: width;
                    smooth: true
                    color: control.pickedColor
                    border.width: 1
                    border.color: 'gray'
                }

                Rectangle {
                    width: childrenRect.width
                    height: childrenRect.height
                    border {width: 1; color: 'gray'}

                    Column {
                        padding: 3
                        Text {
                            color: 'gray'
                            font.family: utils.systemFixedFont()
                            text: "rgb:" + rgbString(control.pickedColor)
                        }

                        Text {
                            color: 'gray'
                            font.family: utils.systemFixedFont()
                            text: "hsl:" + hslString(control.pickedColor)
                        }

                        Text {
                            color: 'gray'
                            font.family: utils.systemFixedFont()
                            text: "hsv:" + hsvString(control.pickedColor)
                        }
                    }
                }
            }
        }
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        propagateComposedEvents: true
        cursorShape: Qt.CrossCursor

        onPositionChanged: {
            /// get color under cursor and set to @a pickedColor
            control.pickedColor = utils.pickColorAt(mouseX, mouseY);
        }

        onClicked: {
            colorSelected(control.pickedColor, mouse.button == Qt.LeftButton);
            control.hide();
        }
    }
}
