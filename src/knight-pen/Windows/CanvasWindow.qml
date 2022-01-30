import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import '../Controls'

ApplicationWindow {
    id: control

    property alias knightcanvas : interactiveCanvas.knightcanvas;

    property bool acceptInputs : true;
    property bool hidden : visibility == Window.Hidden;

    function toggleVisiblity() {
        if(hidden == true) {
            showFullScreen();
            flags = Qt.Widget;
        }
        else {
            hide();
        }
        acceptInputs = flags & Qt.WindowTransparentForInput
    }

    function toggleAcceptInput() {
        flags ^= Qt.WindowTransparentForInput;
        flags ^= Qt.WindowStaysOnTopHint;
        acceptInputs = flags & Qt.WindowTransparentForInput
    }

    color: 'transparent'

    InteractiveCanvas {
        id: interactiveCanvas
        anchors.fill: parent
    }
}
