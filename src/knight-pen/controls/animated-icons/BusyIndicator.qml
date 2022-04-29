import QtQuick 2.15

Item {
    id: spinner
    width: 12
    height: width
    anchors.centerIn: parent

    ShaderEffect {
        anchors.fill: parent
        property real strokeWidth: 0.06
        property real sweepAngle: .5
        fragmentShader: "
            varying highp vec2 qt_TexCoord0;
            uniform highp float qt_Opacity;
            uniform highp float sweepAngle;
            uniform highp float strokeWidth;
            uniform highp float width;

            void main() {
                highp vec2 coord = qt_TexCoord0 - vec2(0.5);
                highp float ring = smoothstep(0.0, 0.5/width, -abs(length(coord) - 0.5 + strokeWidth) + strokeWidth);
                gl_FragColor = vec4(0,0,0,1) * ring;
                gl_FragColor *= smoothstep(0.0, 0.5/width, -atan(coord.x, coord.y) / 6.2831 - 0.48 + sweepAngle);
            }"

        SequentialAnimation on sweepAngle {
            running: spinner.enabled
            loops: -1
            NumberAnimation { duration: 1400; from: 0.2; to: 0.7 }
            NumberAnimation { duration: 2400; from: 0.7; to: 0.2 }
        }
    }

    NumberAnimation on rotation {
        running: spinner.enabled && spinner.visible
        duration: 1000
        loops: -1
        from: 0; to: 360
    }
}
