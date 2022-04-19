import QtQuick 2.15
import QtQuick.Controls 2.15

import knight.pen.utils 1.0

import '../controls'

ScrollView {
    id: control
    clip: true

    implicitWidth: 100
    implicitHeight: 100

    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

    RichText {
        text:
            `<style>
                a { color: #55f; text-decoration: none }
                p { margin: 2px }
            </style>
            <h1>Knight Pen <small> ${Utils.applicationVersion()} </small></h1>
            <p>A simple screen annotation software.</p>
            <p>Based on
            <a href='https://www.qt.io'>Qt</a> &
            <a href='https://github.com/QUItCoding/qnanopainter'>NanoPainter</a>.</p>
            <br>.<br>.<br>.<br>.<br>.<br>.<br>.<br>.<br>.<br>.
            <p>Created by: <a href='https://smr76.github.io'>SMR</a>.</p>
            <p>Related links:
            <a href='https://github.com/SMR76/knight-pen/issues'>Issues</a> -
            <a href='https://github.com/SMR76/knight-pen'>Repository</a> -
            <a href='https://github.com/SMR76/knight-pen/releases'>Releases</a>.</p>`;
    }
}
