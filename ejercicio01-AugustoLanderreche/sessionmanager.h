#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QString>
#include <QDir>

class SessionManager {
public:
    SessionManager();
    ~SessionManager() = default;

    bool isSessionValid(QString& username);
    void saveSession(const QString& username);
    void clearSession();

private:
    QDir dataDir;
    void ensureDataDir();
};

#endif // SESSIONMANAGER_H