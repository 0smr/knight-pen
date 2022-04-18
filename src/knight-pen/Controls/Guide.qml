import QtQuick 2.15

Rectangle {
    id: ctrl
    anchors.fill: parent

    property bool e: true
    property bool c: true

    color: 'transparent'
    enabled: e
    border {
        width: 0.8
        color: '#faa'
    }

    onWidthChanged: tooltip.visible = enabled
    onHeightChanged: tooltip.visible = enabled

    ToolTip {
        id: tooltip
        x: (ctrl.width - width) / (ctrl.c ? 2 : 1)
        y: (ctrl.height - height) / (ctrl.c ? 2 : 1)
        visible: true
        delay: 0
        text: "w: %1\nh: %2".arg(ctrl.width).arg(ctrl.height)
        opacity: 0.7
    }
}
