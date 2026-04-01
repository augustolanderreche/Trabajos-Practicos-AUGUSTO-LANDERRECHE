#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>

class User {
public:
    QString username;
    QString passwordHash; // hashed password
    QString role = "user";

    User() = default;
    User(const QString& usr, const QString& hash, const QString& rol = "user")
        : username(usr), passwordHash(hash), role(rol) {}

    QJsonObject toJson() const;
    static User fromJson(const QJsonObject& obj);
};

#endif // USER_H