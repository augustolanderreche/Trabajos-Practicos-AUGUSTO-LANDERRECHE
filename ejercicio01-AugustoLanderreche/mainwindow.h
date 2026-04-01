#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QList>
#include "datastore.h"
#include "job.h"
#include "history.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(const QString& username, QWidget *parent = nullptr);
    ~MainWindow() override;

public:
    QList<Job> getAllJobs() const { return allJobs; }
    void addJob(const Job& job);
    void updateJob(const Job& job);
    void removeJob(int id);
    Job findJob(int id);
    void addHistoryEntry(const QString& action, int jobId, const QString& details);

signals:
    void logoutRequested();
    void addJobRequested();
    void editJobRequested(int jobId);
    void deleteJobRequested(int jobId);
    void completeJobRequested(int jobId);
    void notesRequested(int jobId);

private slots:
    void on_pushButtonApplyFilter_clicked();
    void on_pushButtonAddJob_clicked();
    void on_pushButtonRefresh_clicked();
    void on_pushButtonLogout_clicked();

private:
    Ui::MainWindow *ui;
    DataStore* dataStore;
    QString currentUser;
    QList<Job> allJobs;
    QList<Job> filteredJobs;
    QList<HistoryEntry> history;

    void loadData();
    void applyFilter();
    void updateJobTable();
    void updateHistoryTable();
    QString statusToString(JobStatus status);
    QString priorityToString(JobPriority priority);
};

#endif // MAINWINDOW_H