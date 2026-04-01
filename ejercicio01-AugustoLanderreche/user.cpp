#include "user.h"

QJsonObject User::toJson() const {
    QJsonObject obj;
    obj["username"] = username;
    obj["passwordHash"] = passwordHash;
    obj["role"] = role;
    return obj;
}

User User::fromJson(const QJsonObject& obj) {
    User user;
    user.username = obj["username"].toString();
    user.passwordHash = obj["passwordHash"].toString();
    user.role = obj["role"].toString();
    return user;
}