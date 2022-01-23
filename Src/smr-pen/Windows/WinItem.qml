import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Item {
    id: control

    implicitWidth: 1
    implicitHeight: 1

    default property QtObject contentItem
    property int alignment: Qt.AlignLeft
    property real spacing: 0

    function updateExternalWindowPosition() {
        var p = mapFromGlobal(Qt.point(0,0))
        switch(alignment) {
        case Qt.AlignRight:
            externalWin.x = -p.x + spacing + control.width
            externalWin.y = -p.y
            break;

        case Qt.AlignLeft:
            externalWin.x = -p.x - spacing - externalWin.width
            externalWin.y = -p.y
            break;

        case Qt.AlignBottom:
            externalWin.x = -p.x
            externalWin.y = -p.y + spacing + height
            break;
        default:
            externalWin.x = -p.x
            externalWin.y = -p.y
        }
    }

    onVisibleChanged: updateExternalWindowPosition();
    Component.onCompleted: Window.window.externalItems.push(this)

    ApplicationWindow {
        id: externalWin
        flags: Qt.FramelessWindowHint |
               Qt.WindowStaysOnTopHint |
               Qt.Widget
        visible: control.visible;
        width: control.contentItem.width
        height: control.contentItem.height
        contentData: [control.contentItem]
        color: 'transparent'
    }
}
