#ifndef JOB_H
#define JOB_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>

enum class JobStatus { Pending, Completed };
enum class JobPriority { Low, Medium, High };

class Job {
public:
    int id = 0;
    QString title;
    QString description;
    JobStatus status = JobStatus::Pending;
    JobPriority priority = JobPriority::Medium;
    QDateTime dueDate;
    QDateTime createdDate;
    QString notes;

    Job() = default;
    Job(int id, const QString& title, const QString& desc, JobStatus status, JobPriority priority, const QDateTime& due, const QDateTime& created, const QString& notes)
        : id(id), title(title), description(desc), status(status), priority(priority), dueDate(due), createdDate(created), notes(notes) {}

    QJsonObject toJson() const;
    static Job fromJson(const QJsonObject& obj);
};

#endif // JOB_H