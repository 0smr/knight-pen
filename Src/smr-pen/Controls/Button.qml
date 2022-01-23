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
    property color backgroundColor: '#eee'

    width: 25
    height: width

    padding: 6
    spacing: 6

    font {
        bold: true
        pixelSize: width * 0.6
        family: 'Font Awesome 5 Free Solid'
    }

    palette {
        base: backgroundColor
        button: backgroundColor
        buttonText: enabled ? control.color : Qt.lighter(control.color, 1.4)
        brightText: enabled ? control.color : Qt.lighter(control.color, 1.4)
    }

    background: Rectangle {
        implicitWidth:  25
        implicitHeight: 25

        color: control.palette.button
        smooth: true

        radius: width/5

        border {
            color: control.checked ? '#89c7f5' : Qt.darker(control.palette.button, 1.3)
            width: control.focus ? 3 : 1

            Behavior on width { NumberAnimation {duration: 100} }
        }
    }
}
