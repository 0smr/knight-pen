import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

Item {
    id: control

    property alias delay: delayTimer.interval
    property alias text: tooltiptext.text
    property alias font: tooltiptext.font
    property alias contentItem: tooltiptext
    property alias timeout: timeoutTimer.interval

    function terminate() {
        delayTimer.stop();
        timeoutTimer.stop();
    }

    visible: false
    onVisibleChanged: {
        if(visible) {
            delayTimer.restart();
        }
        else {
            terminate();
        }
    }

    Timer {
        id: delayTimer
        interval: 500

        onTriggered: {
            let coord = mapToGlobal(0, 0);
            win.x = Math.min(coord.x + 5, Screen .width - win.width - 10);
            win.y = Math.min(Math.max(10, coord.y - 10), Screen.height - win.height - 10);

            timeoutTimer.restart();
        }
    }

    Timer {
        id: timeoutTimer
        interval: 1500
    }

    Window {
        id: win
        width: tooltiptext.width
        height: tooltiptext.height
        visible: control.visible && timeoutTimer.running && tooltip.text;
        color: 'transparent'
        flags: Qt.FramelessWindowHint | Qt.WindowTransparentForInput

        Rectangle {
            anchors.fill: parent
            color: '#f8f9fa'
            border { color: '#e9ecef'; width: 1 }
            radius: 2
            Text {
                id: tooltiptext
                text: ''
                color: 'gray'
                padding: 5
                font.pixelSize: 10
            }
        }
    }
}
