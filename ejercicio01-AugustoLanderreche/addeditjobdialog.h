#ifndef ADDEDITJOBDIALOG_H
#define ADDEDITJOBDIALOG_H

#include <QDialog>
#include "job.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class AddEditJobDialog;
}
QT_END_NAMESPACE

class AddEditJobDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditJobDialog(bool isEdit, const Job& job = Job(), QWidget *parent = nullptr);
    ~AddEditJobDialog() override;

    Job getJob() const { return this->job; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddEditJobDialog *ui;
    Job job;
    bool isEdit;

    QString statusToString(JobStatus status);
    QString priorityToString(JobPriority priority);
    JobStatus stringToStatus(const QString& str);
    JobPriority stringToPriority(const QString& str);
};

#endif // ADDEDITJOBDIALOG_H