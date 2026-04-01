#include "history.h"

QJsonObject HistoryEntry::toJson() const {
    QJsonObject obj;
    obj["timestamp"] = timestamp.toString(Qt::ISODate);
    obj["action"] = action;
    obj["jobId"] = jobId;
    obj["user"] = user;
    obj["details"] = details;
    return obj;
}

HistoryEntry HistoryEntry::fromJson(const QJsonObject& obj) {
    HistoryEntry entry;
    entry.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
    entry.action = obj["action"].toString();
    entry.jobId = obj["jobId"].toInt();
    entry.user = obj["user"].toString();
    entry.details = obj["details"].toString();
    return entry;
}