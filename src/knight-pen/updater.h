#pragma once

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QFile>
#include <QDir>

struct githubRelease {
    Q_GADGET
    Q_PROPERTY(int commitIndex MEMBER mTotalCommit)
    Q_PROPERTY(QString tagName MEMBER mTagName)
    Q_PROPERTY(QString dscription MEMBER mDscription)
    Q_PROPERTY(QString commitHash MEMBER mCommitHash)
    Q_PROPERTY(QString dateTime MEMBER mDateTime)
    Q_PROPERTY(QVariant assets MEMBER mAsset)
public:
    int mTotalCommit;
    QString mTagName;
    QString mDscription;
    QString mCommitHash;
    QString mDateTime;
    QVariant mAsset;
};
Q_DECLARE_METATYPE(githubRelease)

class updater : public QObject {
    Q_OBJECT
    Q_PROPERTY(QDir baseDir READ baseDir WRITE setBaseDir NOTIFY baseDirChanged)
    Q_PROPERTY(bool checking READ checking NOTIFY checkingChanged)
    Q_PROPERTY(bool downloading READ downloading NOTIFY downloadingChanged)

public:
    enum UpdateState { None, HasUpdate };
    Q_ENUM(UpdateState)

    updater(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void checkForUpdates(int releaseOffset = 1) {
        QString graphQLQuery = QString(
            R"(query { user(login: "smr76") { repository(name: "knight-pen") {
                   releases(last: %1) { edges { node {
                       tagCommit { oid committedDate history(before: "") { totalCount } }
                       id tagName description
                       releaseAssets(first: 20) { nodes { name downloadUrl } }
                   } }
                } } } } )").arg(releaseOffset);

        /// contain JSON string like:
        /// "query" : "query { user(login: \"smr76\") {...} }"
        QByteArray jsonString =
            QJsonDocument(QJsonObject {
                              {"query", QJsonValue(graphQLQuery)}
                          }).toJson();

        /// this url is a bridge to: https://api.github.com/graphql
        /// but no need to create authorization token.
        QNetworkRequest req(QUrl("https://s-m-r.ir/API/github/graphQLBridge.php"));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        checkBuffer = "";
        QNetworkReply *reply = netManager.post(req, jsonString);

        setChecking(true);

        QObject::connect(reply, &QNetworkReply::readyRead, this, &updater::checkForUpdateReadyRead);
        QObject::connect(reply, &QNetworkReply::finished, this, &updater::checkForUpdateFinished);
        QObject::connect(reply, &QNetworkReply::errorOccurred, this, &updater::downloadUpdateError);
    }

    Q_INVOKABLE void downloadUpdate(QUrl url) {
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/zip");
        req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

        downloadBuffer = "";
        QNetworkReply *reply = netManager.get(req);

        setDownloading(true);

        QObject::connect(reply, &QNetworkReply::readyRead, this, &updater::downloadUpdateReadyRead);
        QObject::connect(reply, &QNetworkReply::finished, this, &updater::downloadFileFinished);
        QObject::connect(reply, &QNetworkReply::errorOccurred, this, &updater::checkForUpdateError);
        QObject::connect(reply, &QNetworkReply::downloadProgress, this, &updater::fileDownloadProgress);
    }

    const QDir &baseDir() const { return mBaseDir; }
    void setBaseDir(const QDir &newBaseDir) {
        if(mBaseDir == newBaseDir)
            return;
        mBaseDir = newBaseDir;
        emit baseDirChanged();
    }

    bool checking() const { return mChecking; }
    void setChecking(bool checking) {
        mChecking = checking;
        emit checkingChanged();
    }

    bool downloading() const { return mDownloading; }
    void setDownloading(bool downloading) {
        mDownloading = downloading;
        emit downloadingChanged();
    }

private:
#define APP_VERSION "1.0"
    bool isNewVersion(QString otherVersion) {
        auto const taga = QString(APP_VERSION);
        auto const tagb = otherVersion;

        for(auto type : {"pre-alpha", "alpha", "beta"}) {
            if(taga.contains(type) != tagb.contains(type)) {
                return taga.contains(type) < tagb.contains(type);
            }
        }

        auto const vera = taga.split('-').first().mid(1).splitRef('.');
        auto const verb = tagb.split('-').first().mid(1).splitRef('.');

        for (int index = 0, end = std::max(vera.size(), verb.size()); index != end; ++index) {
            if (vera[index].toInt() < verb[index].toInt()) {
                return true;
            }
        }
        return false;
    }

    void checkForUpdateError(QNetworkReply::NetworkError error) {
        emit updateCheckFaild(QString("Network Error (Code: %1)").arg(error));
        setChecking(false);
    }

    void checkForUpdateReadyRead() {
        QNetworkReply *reply = dynamic_cast<QNetworkReply *>(QObject::sender());
        checkBuffer.append(reply->readAll());
    }

    void checkForUpdateFinished() {
        #if defined Q_OS_LINUX
            const char *os = "linux";
        #elif defined Q_OS_WINDOWS
            const char *os = "win";
        #elif defined Q_OS_MACOS
            const char *os = "macos";
        #else
            const char *os = "";
        #endif

        QJsonObject respons = QJsonDocument::fromJson(checkBuffer).object();

        QJsonValue message = respons["message"];
        QJsonValue error = respons["error"];
        QJsonValue data = respons["data"];

        // if both error and message are NULL.
        if(error.type() == QJsonValue::Null && message.type() == QJsonValue::Null) {
            QJsonValue lastNode = data["user"]["repository"]["releases"]["edges"][0]["node"];
            QJsonArray assets = lastNode["releaseAssets"]["nodes"].toArray();
            githubRelease release;

            release.mTagName = lastNode["tagName"].toString();
            release.mCommitHash = lastNode["tagCommit"]["oid"].toString();
            release.mDateTime = lastNode["tagCommit"]["committedDate"].toString();
            release.mTotalCommit = lastNode["tagCommit"]["history"]["totalCount"].toInt();
            release.mDscription = lastNode["description"].toString();

            for(const auto &asset: assets) {
                if(QJsonValue(asset)["name"].toString().contains(os)) { //> checks to see if the asset name contains the os name.
                    release.mAsset = asset.toVariant();
                    break;
                }
            }

            if(release.mTagName.isEmpty()) { //> invalid results.
                emit updateCheckFaild("No result from server.");
            } else if(isNewVersion(release.mTagName) == true && release.mAsset.isNull() == false) { //> check if tag is newer than current version
                emit updateAvaliable(release);
                emit updateCheckFinished(UpdateState::HasUpdate);
            } else {
                emit updateCheckFinished(UpdateState::None);
            }
        } else {
            emit updateCheckFaild(error.toString() + message.toString());
        }
        setChecking(false);
    }

    void downloadUpdateReadyRead() {
        QNetworkReply *reply = dynamic_cast<QNetworkReply *>(QObject::sender());
        downloadBuffer.append(reply->readAll());
    }

    void downloadUpdateError(QNetworkReply::NetworkError error) {
        emit updateCheckFaild(QString("Network Error (Code: %1)").arg(error));
        setDownloading(false);
    }

    void downloadFileFinished() {
        QFile downloadFile(mBaseDir.filePath("file.zip") );
        downloadFile.open(QFile::WriteOnly);
        downloadFile.write(downloadBuffer);
        downloadFile.close();
        emit fileDownloaded("");
        setDownloading(false);
    }

public slots:

signals:
    void updateAvaliable(githubRelease release);
    void updateCheckFaild(QString message);
    void updateCheckFinished(updater::UpdateState update);

    void fileDownloaded(QString basename);
    void fileDwonloadFaild(QString message);
    void fileDownloadProgress(qint64 recived, qint64 total);
    void baseDirChanged();
    void checkingChanged();
    void downloadingChanged();

private:
    QNetworkAccessManager netManager;
    QByteArray checkBuffer;
    QByteArray downloadBuffer;
    QDir mBaseDir;

    bool mDownloading = false;
    bool mChecking = false;
};

static void registerKPUpdaterType() {
    qRegisterMetaType<githubRelease>();
    qmlRegisterType<updater>("knight.pen.updater", 1, 0, "Updater");
}
Q_COREAPP_STARTUP_FUNCTION(registerKPUpdaterType)

