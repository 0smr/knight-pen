import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import '../controls'

ApplicationWindow {
    id: window

    property alias knightcanvas : interactiveCanvas.knightcanvas;

    property bool acceptInputs : false;
    property bool hidden : visibility == Window.Hidden;

    visibility: Window.Hidden
    color: 'transparent'

    function toggleVisiblity() {
        if(hidden == true) {
            showFullScreen();
            flags = Qt.Widget;
        } else {
            hide();
        }
        // flag is non-NOTIFYable property, so can't be used to bind with acceptInputs.
        acceptInputs = flags & Qt.WindowTransparentForInput;
    }

    function toggleAcceptInput() {
        flags ^= Qt.WindowTransparentForInput;
        flags ^= Qt.WindowStaysOnTopHint;
        acceptInputs = flags & Qt.WindowTransparentForInput;
    }

    InteractiveCanvas {
        id: interactiveCanvas
        anchors.fill: parent
        knightcanvas.enabled: !acceptInputs
    }
}
