import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC
import QtGraphicalEffects 1.15

QQC.Button {
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    property color color: '#555'
    property color bgColor: '#eee'
    property alias tooltipText: tooltip.text

    width: 25
    height: width

    padding: 6
    spacing: 6

    FontLoader {
        id: knightFont
        source: '../Resources/Font/knight-icon.ttf'
    }

    font {
        bold: true
        pixelSize: width * 0.65
        family: knightFont.name
    }

    palette {
        base: bgColor
        button: bgColor
        buttonText: enabled ? control.color : Qt.lighter(control.color, 1.4)
        brightText: enabled ? control.color : Qt.lighter(control.color, 1.4)
    }

    background: Rectangle {
        color: control.palette.button
        border {
            color: control.checked ? '#89c7f5' : Qt.darker(control.palette.button, 1.3)
            width: control.focus ? 2 : 1
            Behavior on width { NumberAnimation {duration: 100} }
        }
    }

    onPressed: tooltip.terminate();

    WToolTip {
        id: tooltip
        delay: 500
        timeout: 2500
        visible: control.hovered
        anchors.right: parent.right
    }
}
