pragma Singleton

import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.settings 1.1

Control {
    id: control

    property color toolButtonBack
    property color toolButtonFor
    property color windowButtonBack
    property color windowButtonFor

    property font iconFont
    property font monoFont
    property font regularFont
    property font subscriptFont
    property font boldFont
    property font headFont

    Component.onCompleted: {
        iconFont.pointSize = 9;
        monoFont.pointSize = 9;
        subscriptFont.pointSize = 7;
        regularFont.pointSize = 9;
        boldFont.pointSize = 9;
        headFont.pointSize = 16;

        iconFont.family = knightIconSolid.name;
        monoFont.family = courierPrimeCode.name;
        subscriptFont.family = carlito.name;
        regularFont.family = carlito.name;
        boldFont.family = carlito.name;
        headFont.family = carlito.name;

        boldFont.bold = true;
        headFont.bold = true;
    }

    FontLoader {
        id: carlito
        source: "qrc:/resources/font/Carlito-Regular.ttf"
    }

    FontLoader {
        id: courierPrimeCode
        source: "qrc:/resources/font/Courier Prime Code.ttf"
    }

    FontLoader {
        id: knightIconSolid
        source: "qrc:/resources/font/knight-icon-solid.ttf"
    }

    Settings {
        fileName: "config.ini"
        category: "Theme/Font"

        property alias regularFamily: control.regularFont.family
        property alias regularPointSize: control.regularFont.pointSize
        property alias monoFamily: control.monoFont.family
        property alias monoPointSize: control.monoFont.pointSize
    }
}
