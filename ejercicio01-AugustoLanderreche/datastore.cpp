#include "datastore.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QCryptographicHash>
#include <QDebug>

DataStore::DataStore() {
    ensureDataDir();
}

void DataStore::ensureDataDir() {
    dataDir = QDir(QDir::currentPath() + "/data");
    if (!dataDir.exists()) {
        dataDir.mkpath(".");
    }
}

QString DataStore::hashPassword(const QString& password) {
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}

QList<Job> DataStore::loadJobs() {
    QList<Job> jobs;
    QFile file(dataDir.absoluteFilePath("jobs.json"));
    if (!file.open(QIODevice::ReadOnly)) {
        return jobs;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonArray array = doc.array();
    for (const QJsonValue& val : array) {
        jobs.append(Job::fromJson(val.toObject()));
    }
    return jobs;
}

void DataStore::saveJobs(const QList<Job>& jobs) {
    QJsonArray array;
    for (const Job& job : jobs) {
        array.append(job.toJson());
    }
    QJsonDocument doc(array);
    QFile file(dataDir.absoluteFilePath("jobs.json"));
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

QList<HistoryEntry> DataStore::loadHistory() {
    QList<HistoryEntry> history;
    QFile file(dataDir.absoluteFilePath("history.json"));
    if (!file.open(QIODevice::ReadOnly)) {
        return history;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonArray array = doc.array();
    for (const QJsonValue& val : array) {
        history.append(HistoryEntry::fromJson(val.toObject()));
    }
    return history;
}

void DataStore::saveHistory(const QList<HistoryEntry>& history) {
    QJsonArray array;
    for (const HistoryEntry& entry : history) {
        array.append(entry.toJson());
    }
    QJsonDocument doc(array);
    QFile file(dataDir.absoluteFilePath("history.json"));
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

QList<User> DataStore::loadUsers() {
    QList<User> users;
    QFile file(dataDir.absoluteFilePath("users.json"));
    if (!file.open(QIODevice::ReadOnly)) {
        // Create default user
        users.append(User("admin", hashPassword("admin")));
        saveUsers(users);
        return users;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QJsonArray array = doc.array();
    for (const QJsonValue& val : array) {
        users.append(User::fromJson(val.toObject()));
    }
    if (users.isEmpty()) {
        users.append(User("admin", hashPassword("admin")));
        saveUsers(users);
    }
    return users;
}

void DataStore::saveUsers(const QList<User>& users) {
    QJsonArray array;
    for (const User& user : users) {
        array.append(user.toJson());
    }
    QJsonDocument doc(array);
    QFile file(dataDir.absoluteFilePath("users.json"));
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}