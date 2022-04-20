import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.settings 1.1

import knight.pen.updater 1.0
import knight.pen.utils 1.0

import '../controls/interactive icons'
import '../controls'

BaseForm {
    id: form

    property bool updateAvaliable: false;
    property alias updater: updater

    onVisibleChanged: time.now = Date.now()

    QtObject {
        id: time
        property real now: 0
    }

    Settings {
        id: settings
        category: "Updater"
        fileName: "config.ini"

        property bool checked: false
        property real lastUpdateCheckTime: 1
        property string lastUpdateCheckHash: ''
    }

    Updater {
        id: updater

        property var assets: []
//        Component.onCompleted: checkForUpdates();

        onFileDownloadProgress: {
            progressBar.value = recived/total;
            print(recived/total);
        }

        onFileDownloaded: {
            print([basename].join('/'));
            progressBar.visible = 0;
            progressBar.value = 0;
        }

        onUpdateCheckFinished: {
            if(update == Updater.None) {
                statusHeader.text = "You're using latest version.";
            }
        }

        onUpdateCheckFaild: {
            logger.text = "### Update Failed:\n\n" + message;
        }

        onFileDwonloadFaild: {
            logger.text = "### Download Failed:\n\n" + message;
            downloadUpdate.enabled = true;
        }

        onUpdateAvaliable: {
            version.text = release.tagName;
            releaseDate.text = release.dateTime;
            statusHeader.text = "New update avaliable (%1).".arg(release.tagName);
            logger.text = release.dscription;
            appName.text = release.assets.name;
            form.updateAvaliable = true;
        }
    }

    Grid {
        anchors.fill: parent
        spacing: 5
        layoutDirection: Qt.LeftToRight

        Column {
            width: parent.width
            spacing: 10

            Label {
                text: 'Updater'
                font.family: 'Calibri'
                font.bold: true
                font.pointSize: 9
            }

            Grid {
                width: parent.width
                height: childrenRect.height
                flow: Grid.LeftToRight
                layoutDirection: Qt.LeftToRight
                padding: 5

                AnimatedText {
                    id: statusHeader
                    width: parent.width - updateBtn.width - 5
                    text: ''
                    font.pointSize: 8
                    font.family: 'Consolas'
                    wrapMode: Label.WordWrap
                }

                Column {
                    width: childrenRect.width
                    height: childrenRect.height
                    Button {
                        id: updateBtn
                        width: 140
                        height: 40
                        text: "Check for update"
                        iconx: BusyIndicator {}
                        enableIcon: updater.checking || updater.downloading
                        onPressed: {
                            settings.lastUpdateCheckTime = Date.now()
                            updater.checkForUpdates();
                        }
                    }

                    Text {
                        text: {
                            const milisecond = time.now - settings.lastUpdateCheckTime;
                            return `last update check ${Utils.timeToShortStr(milisecond)} ago`;
                        }
                        leftPadding: 5
                        opacity: 0.4
                        font {
                            pointSize: 7
                            family: 'Calibri'
                        }
                    }
                }
            }

            Column {
                padding: 5
                width: parent.width
                AnimatedText { id: appName; opacity: 0.8 }

                WideRow {
                    height: 25
                    width: parent.width - 10

                    AnimatedText { id: version; opacity: 0.8 }
                    AnimatedText { id: releaseDate; opacity: 0.8; text: 'YYYY-MM-DD HH:MM:SS'; }
                }
            }
        }

        GridSeprator { length: parent.width; fill: 1 }

        RichText {
            id: logger

            width: parent.width
            font.family: 'Consolas'
            textFormat: Text.MarkdownText
            wrapMode: Text.WordWrap
            color: 'gray'
            text: ""
        }
    }
}
