#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>
#include "login.h"
#include "mainwindow.h"
#include "sessionmanager.h"
#include "addeditjobdialog.h"
#include "noteseditor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("windows"));

    QString style = R"(
        QWidget { background-color: #f3f4f6; font-family: "Segoe UI", "Roboto", sans-serif; }
        QPushButton { background-color: #3b82f6; color: white; border-radius: 8px; padding: 8px 14px; }
        QPushButton:hover { background-color: #2563eb; }
        QPushButton:disabled { background-color: #94a3b8; }
        QLineEdit, QComboBox, QTextEdit, QDateEdit { background-color: white; border: 1px solid #d1d5db; border-radius: 6px; padding: 6px; }
        QLabel { color: #111827; }
        QTableWidget { gridline-color: #e5e7eb; background-color: white; }
        QHeaderView::section { background-color: #e5e7eb; color: #111827; padding: 6px; }
    )";
    a.setStyleSheet(style);

    SessionManager sm;
    QString username;
    bool valid = sm.isSessionValid(username);

    login* loginWidget = new login();
    MainWindow* mainWindow = nullptr;

    auto connectMainWindowSignals = [&]() {
        QObject::connect(mainWindow, &MainWindow::logoutRequested, [&]() {
            mainWindow->hide();
            delete mainWindow;
            mainWindow = nullptr;
            sm.clearSession();
            loginWidget->show();
        });

        QObject::connect(mainWindow, &MainWindow::addJobRequested, [&]() {
            AddEditJobDialog dialog(false);
            if (dialog.exec() == QDialog::Accepted) {
                Job job = dialog.getJob();
                int maxId = 0;
                for (const Job& j : mainWindow->getAllJobs()) maxId = qMax(maxId, j.id);
                job.id = maxId + 1;
                job.createdDate = QDateTime::currentDateTime();
                mainWindow->addJob(job);
                mainWindow->addHistoryEntry("create", job.id, "Trabajo creado");
            }
        });

        QObject::connect(mainWindow, &MainWindow::editJobRequested, [&](int id) {
            Job job = mainWindow->findJob(id);
            if (job.id == 0) return;
            AddEditJobDialog dialog(true, job);
            if (dialog.exec() == QDialog::Accepted) {
                Job updated = dialog.getJob();
                updated.id = id;
                updated.createdDate = job.createdDate;
                mainWindow->updateJob(updated);
                mainWindow->addHistoryEntry("edit", id, "Trabajo editado");
            }
        });

        QObject::connect(mainWindow, &MainWindow::deleteJobRequested, [&](int id) {
            if (QMessageBox::question(mainWindow, "Confirmar", "Eliminar trabajo?") == QMessageBox::Yes) {
                mainWindow->removeJob(id);
                mainWindow->addHistoryEntry("delete", id, "Trabajo eliminado");
            }
        });

        QObject::connect(mainWindow, &MainWindow::completeJobRequested, [&](int id) {
            Job job = mainWindow->findJob(id);
            job.status = JobStatus::Completed;
            mainWindow->updateJob(job);
            mainWindow->addHistoryEntry("complete", id, "Trabajo completado");
        });

        QObject::connect(mainWindow, &MainWindow::notesRequested, [&](int id) {
            Job job = mainWindow->findJob(id);
            NotesEditor editor(job.notes, mainWindow);
            if (editor.exec() == QDialog::Accepted) {
                job.notes = editor.getNotes();
                mainWindow->updateJob(job);
                mainWindow->addHistoryEntry("notes", id, "Notas actualizadas");
            }
        });
    };

    if (valid) {
        mainWindow = new MainWindow(username);
        mainWindow->show();
        connectMainWindowSignals();
    } else {
        loginWidget->show();
    }

    QObject::connect(loginWidget, &login::loginSuccessful, [&](const QString& user) {
        mainWindow = new MainWindow(user);
        mainWindow->show();
        loginWidget->hide();
        connectMainWindowSignals();
    });

    return a.exec();
}
