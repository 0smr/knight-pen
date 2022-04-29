#pragma once

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QObject>
#include <QtCore>

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

#include "utils.h"

struct githubRelease {
    Q_GADGET
    Q_PROPERTY(int commitIndex MEMBER mTotalCommit)
    Q_PROPERTY(QString tagName MEMBER mTagName)
    Q_PROPERTY(QString dscription MEMBER mDscription)
    Q_PROPERTY(QString commitHash MEMBER mCommitHash)
    Q_PROPERTY(QString dateTime MEMBER mDateTime)
    Q_PROPERTY(QVariant asset MEMBER mAsset)
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

    updater(QObject* parent = nullptr)
        : QObject(parent), mDownloadReply(nullptr), mCheckReply(nullptr), mBaseDir("updates"){
        mNetworkManager.setAutoDeleteReplies(true);
    }

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

        /// Without the need to obtain an authorization token,
        /// this link creates a bridge to: https://api.github.com/graphql.
        QNetworkRequest req(QUrl("https://s-m-r.ir/API/github/graphQLBridge.php"));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        mCheckBuffer = "";
        mCheckReply = mNetworkManager.post(req, jsonString);

        setChecking(true);

        QObject::connect(mCheckReply, &QNetworkReply::readyRead, this, &updater::checkForUpdateReadyRead);
        QObject::connect(mCheckReply, &QNetworkReply::finished, this, &updater::checkForUpdateFinished);
        QObject::connect(mCheckReply, &QNetworkReply::errorOccurred, this, &updater::checkForUpdateError);
    }

    Q_INVOKABLE void downloadUpdate(QUrl url) {
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/zip");
        req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

        mDownloadBuffer = "";
        mDownloadReply = mNetworkManager.get(req);

        setDownloading(true);

        QObject::connect(mDownloadReply, &QNetworkReply::readyRead, this, &updater::downloadUpdateReadyRead);
        QObject::connect(mDownloadReply, &QNetworkReply::finished, this, &updater::downloadFileFinished);
        QObject::connect(mDownloadReply, &QNetworkReply::errorOccurred, this, &updater::downloadUpdateError);
        QObject::connect(mDownloadReply, &QNetworkReply::downloadProgress, this, &updater::fileDownloadProgress);
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
        mCheckBuffer.append(mCheckReply->readAll());
    }

    void checkForUpdateFinished() {
        QString platformOS = "";
        switch(utils::PLATFORM) {
            case utils::Windows: platformOS = "win"; break;
            case utils::Linux: platformOS = "linux"; break;
            case utils::Mac: platformOS = "mac"; break;
            case utils::Other: return;
            default: break;
        }

        QJsonObject response = QJsonDocument::fromJson(mCheckBuffer).object();

        QJsonValue message = response["message"];
        QJsonValue error = response["error"];
        QJsonValue data = response["data"];

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
                if(QJsonValue(asset)["name"].toString().contains(platformOS)) { //> checks to see if the asset name contains the os name.
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
        mDownloadBuffer.append(reply->readAll());
    }

    void downloadUpdateError(QNetworkReply::NetworkError error) {
        emit updateCheckFaild(QString("Network Error (Code: %1)").arg(error));
        setDownloading(false);
    }

    void downloadFileFinished() {
        QString rawHeader = mDownloadReply->rawHeader("Content-Disposition");
        QString fileName = rawHeader.mid(rawHeader.indexOf("filename=") + 9);
        QFile downloadFile(mBaseDir.filePath(fileName));

        mBaseDir.mkpath(".");
        if(mDownloadReply->error() == QNetworkReply::NoError && downloadFile.open(QFile::WriteOnly) == true) {
            downloadFile.write(mDownloadBuffer);
            downloadFile.close();
            emit fileDownloaded(mBaseDir.filePath(fileName)); //> Emit path to file as "filePath"
        }
        setDownloading(false);
    }

public slots:
    void abort() {
        if(mDownloading && mDownloadReply) mDownloadReply->abort();
        if(mChecking && mCheckReply) mCheckReply->abort();
    }

signals:
    void updateAvaliable(githubRelease release);
    void updateCheckFaild(QString message);
    void updateCheckFinished(updater::UpdateState update);

    void fileDownloaded(QString filePath);
    void fileDwonloadFaild(QString message);
    void fileDownloadProgress(qint64 recived, qint64 total);
    void baseDirChanged();
    void checkingChanged();
    void downloadingChanged();

private:
    QNetworkAccessManager mNetworkManager;
    QNetworkReply *mDownloadReply;
    QNetworkReply *mCheckReply;
    QByteArray mDownloadBuffer;
    QByteArray mCheckBuffer;
    QDir mBaseDir;

    bool mDownloading = false;
    bool mChecking = false;
};

static void registerKPUpdaterType() {
    qRegisterMetaType<githubRelease>();
    qmlRegisterType<updater>("knight.pen.updater", 1, 0, "Updater");
}
Q_COREAPP_STARTUP_FUNCTION(registerKPUpdaterType)
