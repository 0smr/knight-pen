import QtQuick 2.15
import QtQuick.Controls 2.15
import Qt.labs.settings 1.1

import knight.pen.updater 1.0
import knight.pen.utils 1.0

import '../controls'

BaseForm {
    id: form

    property string assetUrl: "";
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

        Component.onCompleted: checkForUpdates();

        onFileDownloadProgress: {
            progressBar.value = recived/total;
        }

        onFileDownloaded: {
            logger.markdown = "### Download completed\nFile path: " + filePath;
            statusHeader.text = "Download completed";
            progressBar.value = 0;
            form.assetUrl = "";
            // After downloading file, open it in file path.
            Utils.openFileInExplorer(filePath);
        }

        onUpdateCheckFinished: {
            if(update == Updater.None) {
                statusHeader.text = "You're using latest version.";
            }
        }

        onUpdateCheckFaild: {
            logger.markdown = "### Update Failed:" + message;
        }

        onFileDwonloadFaild: {
            logger.markdown = "### Download Failed:" + message;
            progressBar.value = 0;
        }

        onUpdateAvaliable: {
            version.text = release.tagName;
            releaseDate.text = release.dateTime;
            statusHeader.text = "New update avaliable (%1).".arg(release.tagName);
            logger.markdown = release.dscription;
            appName.text = release.asset.name;
            form.assetUrl = release.asset.downloadUrl;
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
                font: KnightPen.boldFont
            }

            Grid {
                width: parent.width
                height: childrenRect.height
                flow: Grid.LeftToRight
                layoutDirection: Qt.LeftToRight
                rightPadding: 5

                Column {
                    width: parent.width - updateBtn.width
                    padding: 5
                    spacing: 5
                    AnimatedText {
                        id: statusHeader
                        width: parent.width
                        text: 'Updater'
                        font: KnightPen.monoFont
                        wrapMode: Label.WordWrap
                    }

                    ProgressBar {
                        id: progressBar
                        width: parent.width - 10
                        value: 0
                        visible: value > 0
                    }

                    Text {
                        width: implicitWidth
                        height: implicitHeight
                        visible: progressBar.visible
                        text: (progressBar.value * 100).toFixed() + "%"
                        opacity: 0.4
                        font.family: KnightPen.regularFont.family
                        font.pointSize: 7
                    }
                }

                Column {
                    width: lastUpdateCheckLabel.width + 10
                    height: childrenRect.height
                    Button {
                        id: updateBtn
                        width: Math.max(130, parent.width)
                        height: 40
                        text: form.assetUrl? "Download Update" : "Check for update"
                        iconx: BusyIndicator {}
                        enableIcon: updater.checking || updater.downloading
                        onPressed: {
                            if(updater.checking || updater.downloading) {
                                updater.abort()
                            } else {
                                if(form.assetUrl) {
                                    updater.downloadUpdate(form.assetUrl);
                                } else {
                                    settings.lastUpdateCheckTime = Date.now()
                                    updater.checkForUpdates();
                                }
                            }
                        }
                    }

                    Text {
                        id: lastUpdateCheckLabel
                        width: implicitWidth
                        height: implicitHeight
                        text: {
                            const milisecond = time.now - settings.lastUpdateCheckTime;
                            return `last update check ${Utils.timeToShortStr(milisecond)} ago`;
                        }
                        leftPadding: 5
                        opacity: 0.4
                        font: KnightPen.subscriptFont
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

        MDText {
            id: logger

            width: parent.width
            font: KnightPen.regularFont
            wrapMode: Text.WordWrap
            color: 'gray'
            markdown: ""
        }
    }
}
