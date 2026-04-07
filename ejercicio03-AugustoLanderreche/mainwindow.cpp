#include "mainwindow.h"
#include "canvasmodel.h"
#include "canvasview.h"
#include "syncmanager.h"

#include <QToolBar>
#include <QPushButton>
#include <QStatusBar>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

MainWindow::MainWindow(const QString &userName, QWidget *parent)
    : QMainWindow(parent),
    m_userName(userName),
      m_model(new CanvasModel(this)),
    m_view(new CanvasView(m_model, m_userName, this)),
      m_sync(new SyncManager(m_model, this))
{
    resize(1024, 768);
    setWindowTitle("Ejercicio 03 - Lienzo colaborativo en tiempo real");

    QToolBar *toolbar = addToolBar("Toolbar");
    toolbar->setMovable(false);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);

    QPushButton *saveButton = new QPushButton("Guardar", this);
    saveButton->setStyleSheet("QPushButton { background-color: #0078D7; color: white; border: none; padding: 8px 16px; border-radius: 4px; } QPushButton:hover { background-color: #005A9E; }");
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    toolbar->addWidget(saveButton);

    QLabel *userLabel = new QLabel(QString("Usuario: %1").arg(m_userName), this);
    toolbar->addWidget(userLabel);

    QLabel *infoLabel = new QLabel("Botones 1-9: color / Rueda: grosor / botón izquierdo: dibujar / botón derecho: goma", this);
    toolbar->addWidget(infoLabel);

    setCentralWidget(m_view);

    m_sync->setServerUrl(QUrl("http://194.163.180.102:5000"));
    connect(m_sync, &SyncManager::syncError, this, &MainWindow::onSyncError);

    m_sync->loadFromServer();
    m_sync->startAutoSync(2000);
}

MainWindow::~MainWindow() {}

void MainWindow::onSaveClicked() {
    m_sync->saveToServer();
    statusBar()->showMessage("Guardando en servidor...", 2000);
}

void MainWindow::onSyncError(const QString &message) {
    statusBar()->showMessage("Sync error: " + message, 5000);
}
