#ifndef HISTORY_H
#define HISTORY_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>

class HistoryEntry {
public:
    QDateTime timestamp;
    QString action; // e.g., "create", "edit", "delete", "complete"
    int jobId = 0;
    QString user;
    QString details;

    HistoryEntry() = default;
    HistoryEntry(const QDateTime& ts, const QString& act, int jid, const QString& usr, const QString& det)
        : timestamp(ts), action(act), jobId(jid), user(usr), details(det) {}

    QJsonObject toJson() const;
    static HistoryEntry fromJson(const QJsonObject& obj);
};

#endif // HISTORY_H