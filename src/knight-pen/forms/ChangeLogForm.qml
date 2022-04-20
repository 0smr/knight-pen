import QtQuick 2.15
import QtQuick.Controls 2.15

import '../controls'

ScrollView {
    id: control
    clip: true

    implicitWidth: 100
    implicitHeight: 100

    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

    RichText {
        text:"
            <style>
                a { color: #55f; text-decoration: none }
                p { margin: 2px }
            </style>
            <h1>Change Log</h1>
            <h4> v0.2.0: </h4>
            <ul>
                <li>.</li>
            </ul>
            <h4> v0.1.2: </h4>
            <ul>
                <li>Fix black screen issue when updating canvas on mouse mode.</li>
                <li>Add Tooltip text to all buttons.</li>
                <li>Some changes on UI.</li>
                <li>Other minor changes.</li>
            </ul>
            <h4> v0.1.1: </h4>
            <ul>
                <li>Bug Fixes.</li>
                <li>Other minor improvement.</li>
            </ul>
            <h4> v0.1.0: </h4>
            <ul>
                <li>Draw lines, circles, rectangles, and paths.</li>
                <li>Clear all canvas.</li>
                <li>Use color selectors and eye-dropper to pick colors.</li>
                <li>Switch between normal and canvas modes.</li>
                <li>Minimize to tray.</li>
            </ul>";
    }
}
