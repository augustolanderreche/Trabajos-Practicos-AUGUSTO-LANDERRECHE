#include "addeditjobdialog.h"
#include "ui_addeditjobdialog.h"
#include <QMessageBox>
#include <QDateTime>
#include <QTime>

AddEditJobDialog::AddEditJobDialog(bool isEdit, const Job& job, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddEditJobDialog)
    , job(job)
    , isEdit(isEdit)
{
    ui->setupUi(this);

    if (isEdit) {
        ui->lineEditTitle->setText(job.title);
        ui->textEditDescription->setPlainText(job.description);
        ui->comboBoxStatus->setCurrentText(statusToString(job.status));
        ui->comboBoxPriority->setCurrentText(priorityToString(job.priority));
        ui->dateEditDueDate->setDate(job.dueDate.date());
    } else {
        ui->dateEditDueDate->setDate(QDate::currentDate());
    }
}

AddEditJobDialog::~AddEditJobDialog()
{
    delete ui;
}

void AddEditJobDialog::on_buttonBox_accepted()
{
    QString title = ui->lineEditTitle->text().trimmed();
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Error", "El título es requerido.");
        return;
    }

    job.title = title;
    job.description = ui->textEditDescription->toPlainText();
    job.status = stringToStatus(ui->comboBoxStatus->currentText());
    job.priority = stringToPriority(ui->comboBoxPriority->currentText());
    job.dueDate = QDateTime(ui->dateEditDueDate->date(), QTime(0,0));

    accept();
}

// Helper functions
QString AddEditJobDialog::statusToString(JobStatus status) {
    switch (status) {
    case JobStatus::Pending: return "Pendiente";
    case JobStatus::Completed: return "Completado";
    default: return "Pendiente";
    }
}

QString AddEditJobDialog::priorityToString(JobPriority priority) {
    switch (priority) {
    case JobPriority::Low: return "Baja";
    case JobPriority::Medium: return "Media";
    case JobPriority::High: return "Alta";
    default: return "Media";
    }
}

JobStatus AddEditJobDialog::stringToStatus(const QString& str) {
    if (str == "Completado") return JobStatus::Completed;
    return JobStatus::Pending;
}

JobPriority AddEditJobDialog::stringToPriority(const QString& str) {
    if (str == "Baja") return JobPriority::Low;
    if (str == "Alta") return JobPriority::High;
    return JobPriority::Medium;
}