#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>

MainWindow::MainWindow(const QString& username, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
    , dataStore(new DataStore())
    , currentUser(username)
{
    ui->setupUi(this);
    connect(ui->pushButtonApplyFilter, &QPushButton::clicked, this, &MainWindow::on_pushButtonApplyFilter_clicked);
    connect(ui->pushButtonAddJob, &QPushButton::clicked, this, &MainWindow::on_pushButtonAddJob_clicked);
    connect(ui->pushButtonRefresh, &QPushButton::clicked, this, &MainWindow::on_pushButtonRefresh_clicked);
    connect(ui->pushButtonLogout, &QPushButton::clicked, this, &MainWindow::on_pushButtonLogout_clicked);

    // Diseño moderno, estilo limpio bien espaciado
    QString style = R"(
        QWidget { background-color: #f4f6fb; color: #0f172a; }
        QLabel#labelTitle { font-size: 20px; font-weight: bold; color: #0f172a; }
        QLabel { color: #334155; }
        QLineEdit, QComboBox, QDateEdit { background: #ffffff; color: #0f172a; border: 1px solid #cbd5e1; border-radius: 8px; min-height: 30px; padding: 6px; }
        QComboBox { min-width: 120px; }
        QPushButton { font-weight: bold; color: #ffffff; border-radius: 10px; background-color: #0f172a; min-height: 36px; padding: 8px 14px; }
        QPushButton:hover { background-color: #1e293b; }
        QTableWidget { background-color: #ffffff; border: 1px solid #cbd5e1; gridline-color: #e2e8f0; alternate-background-color: #f8fafc; color: #0f172a; }
        QTableWidget::item { color: #0f172a; }
        QTableView::item:selected { background: #93c5fd; color: #0f172a; }
        QHeaderView::section { background: #e2e8f0; color: #0f172a; padding: 4px; border: 1px solid #cbd5e1; }
        QTableCornerButton::section { background: #e2e8f0; }
        QPushButton#actionButton { min-width: 70px; color: #ffffff; background-color: #1d4ed8; }
        QPushButton#actionButton:hover { background-color: #1e40af; }
    )";
    this->setStyleSheet(style);

    ui->tableWidgetJobs->setAlternatingRowColors(true);
    ui->tableWidgetHistory->setAlternatingRowColors(true);
    ui->tableWidgetJobs->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetJobs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetHistory->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->comboBoxStatus->setEditable(false);
    ui->comboBoxPriority->setEditable(false);
    ui->comboBoxStatus->setCurrentIndex(0);
    ui->comboBoxPriority->setCurrentIndex(0);

    loadData();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dataStore;
}

void MainWindow::loadData()
{
    allJobs = dataStore->loadJobs();
    history = dataStore->loadHistory();
    applyFilter();
    updateHistoryTable();
}

void MainWindow::applyFilter()
{
    filteredJobs.clear();
    JobStatus filterStatus = JobStatus::Pending; // default
    QString statusText = ui->comboBoxStatus->currentText();
    if (statusText == "Todos") {
        // all
    } else if (statusText == "Pendiente") {
        filterStatus = JobStatus::Pending;
    } else if (statusText == "Completado") {
        filterStatus = JobStatus::Completed;
    }

    JobPriority filterPriority = JobPriority::Low; // default
    QString priorityText = ui->comboBoxPriority->currentText();
    if (priorityText == "Todas") {
        // all
    } else if (priorityText == "Baja") {
        filterPriority = JobPriority::Low;
    } else if (priorityText == "Media") {
        filterPriority = JobPriority::Medium;
    } else if (priorityText == "Alta") {
        filterPriority = JobPriority::High;
    }

    for (const Job& job : allJobs) {
        bool statusMatch = (statusText == "Todos") || (job.status == filterStatus);
        bool priorityMatch = (priorityText == "Todas") || (job.priority == filterPriority);
        if (statusMatch && priorityMatch) {
            filteredJobs.append(job);
        }
    }
    updateJobTable();
}

void MainWindow::updateJobTable()
{
    ui->tableWidgetJobs->setRowCount(filteredJobs.size());
    ui->tableWidgetJobs->setColumnCount(5);
    ui->tableWidgetJobs->setHorizontalHeaderLabels({"Título", "Estado", "Prioridad", "Fecha Límite", "Acciones"});

    for (int row = 0; row < filteredJobs.size(); ++row) {
        const Job& job = filteredJobs[row];
        ui->tableWidgetJobs->setItem(row, 0, new QTableWidgetItem(job.title));
        ui->tableWidgetJobs->setItem(row, 1, new QTableWidgetItem(statusToString(job.status)));
        ui->tableWidgetJobs->setItem(row, 2, new QTableWidgetItem(priorityToString(job.priority)));
        ui->tableWidgetJobs->setItem(row, 3, new QTableWidgetItem(job.dueDate.toString("dd/MM/yyyy")));

        QWidget* actionsWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout(actionsWidget);
        layout->setContentsMargins(0, 0, 0, 0);

        QPushButton* editBtn = new QPushButton("Editar");
        QPushButton* deleteBtn = new QPushButton("Eliminar");
        QPushButton* completeBtn = new QPushButton("Completar");
        QPushButton* notesBtn = new QPushButton("Notas");

        editBtn->setObjectName("actionButton");
        deleteBtn->setObjectName("actionButton");
        completeBtn->setObjectName("actionButton");
        notesBtn->setObjectName("actionButton");

        editBtn->setStyleSheet("min-width: 70px; font-weight:600; padding: 3px; margin: 1px;");
        deleteBtn->setStyleSheet("min-width: 70px; font-weight:600; padding: 3px; margin: 1px;");
        completeBtn->setStyleSheet("min-width: 70px; font-weight:600; padding: 3px; margin: 1px;");
        notesBtn->setStyleSheet("min-width: 70px; font-weight:600; padding: 3px; margin: 1px;");

        layout->addWidget(editBtn);
        layout->addWidget(deleteBtn);
        layout->addWidget(completeBtn);
        layout->addWidget(notesBtn);

        ui->tableWidgetJobs->setCellWidget(row, 4, actionsWidget);

        connect(editBtn, &QPushButton::clicked, this, [this, row]() {
            emit editJobRequested(filteredJobs[row].id);
        });
        connect(deleteBtn, &QPushButton::clicked, this, [this, row]() {
            emit deleteJobRequested(filteredJobs[row].id);
        });
        connect(completeBtn, &QPushButton::clicked, this, [this, row]() {
            emit completeJobRequested(filteredJobs[row].id);
        });
        connect(notesBtn, &QPushButton::clicked, this, [this, row]() {
            emit notesRequested(filteredJobs[row].id);
        });
    }
}

void MainWindow::updateHistoryTable()
{
    ui->tableWidgetHistory->setRowCount(history.size());
    ui->tableWidgetHistory->setColumnCount(5);
    ui->tableWidgetHistory->setHorizontalHeaderLabels({"Fecha/Hora", "Acción", "Trabajo ID", "Usuario", "Detalles"});

    for (int row = 0; row < history.size(); ++row) {
        const HistoryEntry& entry = history[row];
        ui->tableWidgetHistory->setItem(row, 0, new QTableWidgetItem(entry.timestamp.toString("dd/MM/yyyy hh:mm")));
        ui->tableWidgetHistory->setItem(row, 1, new QTableWidgetItem(entry.action));
        ui->tableWidgetHistory->setItem(row, 2, new QTableWidgetItem(QString::number(entry.jobId)));
        ui->tableWidgetHistory->setItem(row, 3, new QTableWidgetItem(entry.user));
        ui->tableWidgetHistory->setItem(row, 4, new QTableWidgetItem(entry.details));
    }
}

void MainWindow::addHistoryEntry(const QString& action, int jobId, const QString& details)
{
    HistoryEntry entry(QDateTime::currentDateTime(), action, jobId, currentUser, details);
    history.append(entry);
    dataStore->saveHistory(history);
    updateHistoryTable();
}

QString MainWindow::statusToString(JobStatus status)
{
    switch (status) {
    case JobStatus::Pending: return "Pendiente";
    case JobStatus::Completed: return "Completado";
    default: return "Desconocido";
    }
}

QString MainWindow::priorityToString(JobPriority priority)
{
    switch (priority) {
    case JobPriority::Low: return "Baja";
    case JobPriority::Medium: return "Media";
    case JobPriority::High: return "Alta";
    default: return "Desconocida";
    }
}

void MainWindow::on_pushButtonApplyFilter_clicked()
{
    applyFilter();
}

void MainWindow::on_pushButtonAddJob_clicked()
{
    emit addJobRequested();
}

void MainWindow::on_pushButtonRefresh_clicked()
{
    loadData();
}

void MainWindow::on_pushButtonLogout_clicked()
{
    emit logoutRequested();
}

void MainWindow::addJob(const Job& job)
{
    allJobs.append(job);
    dataStore->saveJobs(allJobs);
    applyFilter();
}

void MainWindow::updateJob(const Job& job)
{
    for (Job& j : allJobs) {
        if (j.id == job.id) {
            j = job;
            break;
        }
    }
    dataStore->saveJobs(allJobs);
    applyFilter();
}

void MainWindow::removeJob(int id)
{
    for (int i = 0; i < allJobs.size(); ++i) {
        if (allJobs[i].id == id) {
            allJobs.removeAt(i);
            break;
        }
    }
    dataStore->saveJobs(allJobs);
    applyFilter();
}

Job MainWindow::findJob(int id)
{
    for (const Job& j : allJobs) {
        if (j.id == id) return j;
    }
    return Job();
}