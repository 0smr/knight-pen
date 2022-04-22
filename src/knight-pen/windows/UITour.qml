import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: window

    width: Screen.width
    height: Screen.height

    color: 'transparent'
    visibility: Window.Hidden
    flags: Qt.Window | Qt.WindowStaysOnTopHint | Qt.WindowTransparentForInput

    /**
     * Array of
     * {target: Item, text: "", align: Qt.AlignLeft}}
     */
    property var messages: []
    property int index: -1
    property int radius: 25

    function showNextMessage(startIndex = undefined, textMesssage = undefined) {
        index = startIndex || index + 1;

        if(index < messages.length) {
            const alignment = messages[index].align || Qt.AlignLeft;
            const message = textMesssage || messages[index].text;
            const targetItem = messages[index].target;
            const point = targetItem.mapToGlobal(14, 14);

            focusIn(point);
            showMessage(message, point, alignment);
        } else {
            window.hide();
            window.destroy();
        }
    }

    function showMessage(message, point, align) {
        text.text = message;
        text.y = point.y - text.implicitHeight/2;
        text.x = point.x;
        if(align === Qt.AlignLeft) {
            text.x -= window.radius + text.implicitWidth;
        } else {
            text.x += window.radius;
        }

        messageAnim.restart();
    }

    function focusIn(point, rad = 50) {
        shader.center = Qt.vector2d(point.x, point.y);
        focusAnim.restart();
    }

    Item { id: fake }

    Connections {
        enabled: messages[index] !== undefined
        target: (messages[index] || {}).target || fake
        ignoreUnknownSignals: true

        function onClicked() { window.showNextMessage() }
    }

    ParallelAnimation {
        id: focusAnim
        NumberAnimation {
            target: shader; properties: 'radius'
            from: 200; to: window.radius; duration: 300
            easing.type: Easing.InSine
        }
        NumberAnimation {
            target: shader; properties: 'opacity'
            from: 0.1; to: 0.3 ; duration: 300
        }
    }

    NumberAnimation {
        id: messageAnim
        target: text; properties: 'opacity'
        from: 0; to: 0.8; duration: 600
    }

    ShaderEffect {
        id: shader
        opacity: 0.3
        anchors.fill: parent
        property real radius
        property vector2d center
        readonly property real _radius: radius/width
        readonly property real _smooth: 0.5/width
        readonly property vector2d _center: Qt.vector2d(center.x/width, center.y/width)
        readonly property vector2d _ratio: Qt.vector2d(width / Math.max(width, height),
                                                       height/ Math.max(width, height));
        fragmentShader: "
            varying highp vec2 qt_TexCoord0;
            uniform highp float qt_Opacity;
            uniform highp vec2 _ratio;
            uniform highp vec2 _center;
            uniform highp float _radius;
            uniform highp float _smooth;

            void main() {
                highp vec2 coord = _ratio * qt_TexCoord0;
                highp float ring = smoothstep(0.0, _smooth, distance(coord, _center) - _radius);
                gl_FragColor = vec4(0,0,0,1) * ring * qt_Opacity;
            }"
    }

    Text {
        id: text
        rightPadding: 20
        color: 'white'
        textFormat: Text.RichText
        font {
            family: 'Calibri'
            pointSize: 16
            bold: true
        }
    }
}
