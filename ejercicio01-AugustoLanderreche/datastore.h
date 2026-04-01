#ifndef DATASTORE_H
#define DATASTORE_H

#include <QList>
#include <QDir>
#include "job.h"
#include "history.h"
#include "user.h"

class DataStore {
public:
    DataStore();
    ~DataStore() = default;

    QList<Job> loadJobs();
    void saveJobs(const QList<Job>& jobs);

    QList<HistoryEntry> loadHistory();
    void saveHistory(const QList<HistoryEntry>& history);

    QList<User> loadUsers();
    void saveUsers(const QList<User>& users);

private:
    QDir dataDir;
    QString hashPassword(const QString& password);
    void ensureDataDir();
};

#endif // DATASTORE_H