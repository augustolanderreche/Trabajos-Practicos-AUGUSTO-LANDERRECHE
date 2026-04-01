#include "sessionmanager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDateTime>

SessionManager::SessionManager() {
    ensureDataDir();
}

void SessionManager::ensureDataDir() {
    dataDir = QDir(QDir::currentPath() + "/data");
    if (!dataDir.exists()) {
        dataDir.mkpath(".");
    }
}

bool SessionManager::isSessionValid(QString& username) {
    QFile file(dataDir.absoluteFilePath("session.json"));
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonObject obj = doc.object();
    username = obj["username"].toString();
    QDateTime loginTime = QDateTime::fromString(obj["loginTime"].toString(), Qt::ISODate);
    QDateTime now = QDateTime::currentDateTime();
    if (loginTime.isValid() && loginTime.secsTo(now) < 300) { // 5 minutes = 300 seconds
        return true;
    }
    return false;
}

void SessionManager::saveSession(const QString& username) {
    QJsonObject obj;
    obj["username"] = username;
    obj["loginTime"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    QJsonDocument doc(obj);
    QFile file(dataDir.absoluteFilePath("session.json"));
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void SessionManager::clearSession() {
    QFile file(dataDir.absoluteFilePath("session.json"));
    if (file.exists()) {
        file.remove();
    }
}