import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC

QQC.Button {
    id: control
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    property Component iconx: Item { enabled: false }
    property bool enableIcon: false

    display: enableIcon ? Button.TextBesideIcon : Button.TextOnly
    height: 30

    padding: 0
    spacing: 10

    palette.buttonText: '#444'
    palette.button: '#fff'

    font {
        pointSize: 9
        family: 'Calibri'
        bold: true
    }

    contentItem: Item {
        implicitWidth: Math.max(85, text.width + iconItem.width + spacing)
        implicitHeight: text.height

        Row {
            anchors.centerIn: parent
            width: text.width + spacing + iconItem.width
            height: text.height
            padding: 0
            spacing: control.display == QQC.Button.TextBesideIcon ? control.spacing : 0

            Text {
                id: text
                height: 10
                visible: control.display == QQC.Button.TextOnly ||
                         control.display == QQC.Button.TextBesideIcon;
                text:  control.text
                color: control.palette.buttonText
                font:  control.font
                elide: Text.ElideRight
                verticalAlignment: Qt.AlignVCenter
            }

            Loader {
                id: iconItem
                enabled: control.display == QQC.Button.IconOnly ||
                         control.display == QQC.Button.TextBesideIcon
                visible: enabled
                height: width
                sourceComponent: iconx

                Behavior on opacity { SmoothedAnimation {} }
            }

            Behavior on width { SmoothedAnimation { velocity: 50 } }
        }
    }

    background: BoxShadow {
        radius: 5
        spread: 5
        color: parent.pressed ? '#11000000' : '#1f000000'
        Rectangle {
            anchors.fill: parent
            anchors.margins: 3
            radius: 2
            color: control.palette.button
        }
    }
}
