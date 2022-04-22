import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import '../forms'
import '../controls'

BaseWindow {
    id: window

    property bool updateAvaliable: false
    property alias tabIndex: tabView

    TabView {
        id: tabView

        clip: true
        padding: 8
        anchors.fill: parent

        currentIndex: 1

        tabs: [
            { text:'Configuration', icon: '\ue058'},
            { text:'Updater', icon: '\ue052'}
        ]

        ConfigurationForm {
            width: parent.width
            height: parent.height
        }

        UpdaterForm {
            width: parent.width
            height: parent.height
        }
    }
}
