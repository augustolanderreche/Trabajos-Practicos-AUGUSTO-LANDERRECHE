#include "job.h"

QJsonObject Job::toJson() const {
    QJsonObject obj;
    obj["id"] = id;
    obj["title"] = title;
    obj["description"] = description;
    obj["status"] = static_cast<int>(status);
    obj["priority"] = static_cast<int>(priority);
    obj["dueDate"] = dueDate.toString(Qt::ISODate);
    obj["createdDate"] = createdDate.toString(Qt::ISODate);
    obj["notes"] = notes;
    return obj;
}

Job Job::fromJson(const QJsonObject& obj) {
    Job job;
    job.id = obj["id"].toInt();
    job.title = obj["title"].toString();
    job.description = obj["description"].toString();
    job.status = static_cast<JobStatus>(obj["status"].toInt());
    job.priority = static_cast<JobPriority>(obj["priority"].toInt());
    job.dueDate = QDateTime::fromString(obj["dueDate"].toString(), Qt::ISODate);
    job.createdDate = QDateTime::fromString(obj["createdDate"].toString(), Qt::ISODate);
    job.notes = obj["notes"].toString();
    return job;
}