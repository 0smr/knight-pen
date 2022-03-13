import QtQuick 2.15
import QtQuick.Controls 2.15

import '../Controls'

ScrollView {
    id: control
    clip: true

    implicitWidth: 100
    implicitHeight: 100

    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

    RichText {
        text:
            "<style>
                a { color: #55f; text-decoration: none }
                p { margin: 2px }
            </style>
            <h1>FAQ <small>frequency asked questions</small></h1>
            <p></p>.";
    }
}
