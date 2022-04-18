import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

import '../controls'
import '../forms'

BaseWindow {
    id: window

    TabView {
        id: tabView

        clip: true
        padding: 8
        anchors.fill: parent
        tabs: [
            { text:'About', icon: '\ue059'},
            { text:'Change log', icon: '\ue05a'},
            { text:'Help', icon: '\ue05e'},
            { text:'FAQ', icon: '\ue05d'}
        ]

        AboutForm {
            width: parent.width
            height: parent.height
        }

        ChangeLogForm {
            width: parent.width
            height: parent.height
        }

        HelpForm {}
        FAQForm {}
    }
}
