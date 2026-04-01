#include "monitoringdashboard.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFont>
#include <QDateTime>
#include <QDebug>
#include <QScrollArea>

MonitoringDashboard::MonitoringDashboard(QWidget *parent)
    : QMainWindow(parent),
      m_monitor(new ServerHealthMonitor(this)),
      m_uptimeUpdateTimer(new QTimer(this)),
      m_isMonitoring(false)
{
    setWindowTitle("Sistema de Monitoreo de Servidor - VPS Monitor");
    setWindowIcon(QIcon());
    setGeometry(100, 100, 1200, 900);

    // Configurar estilo
    setStyleSheet(
        "QMainWindow { background-color: #f5f5f5; }"
        "QGroupBox { border: 1px solid #ddd; border-radius: 5px; margin-top: 10px; padding-top: 10px; font-weight: bold; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }"
        "QLabel { color: #333; }"
        "QPushButton { background-color: #0066cc; color: white; border: none; padding: 8px 15px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #0052a3; }"
        "QPushButton:pressed { background-color: #003d7a; }"
        "QLineEdit { border: 1px solid #ccc; padding: 5px; border-radius: 4px; }"
        "QSpinBox { border: 1px solid #ccc; padding: 5px; border-radius: 4px; }"
        "QProgressBar { border: 1px solid #ddd; border-radius: 4px; text-align: center; }"
        "QListWidget { border: 1px solid #ddd; border-radius: 4px; }"
    );

    setupUI();

    // Timer para actualizar uptime cada segundo
    connect(m_uptimeUpdateTimer, &QTimer::timeout, this, &MonitoringDashboard::updateUptimeDisplay);

    // Conectar señales del monitor
    connect(m_monitor, &ServerHealthMonitor::healthDataUpdated, this, &MonitoringDashboard::onHealthDataUpdated);
    connect(m_monitor, &ServerHealthMonitor::statusChanged, this, &MonitoringDashboard::onStatusChanged);
    connect(m_monitor, &ServerHealthMonitor::eventLogged, this, &MonitoringDashboard::onEventLogged);
    connect(m_monitor, &ServerHealthMonitor::connectionError, this, &MonitoringDashboard::onConnectionError);

    // Conectar cambios en spinbox
    connect(m_intervalSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MonitoringDashboard::onIntervalChanged);
    connect(m_cpuThresholdSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MonitoringDashboard::onCpuThresholdChanged);
    connect(m_memoryThresholdSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MonitoringDashboard::onMemoryThresholdChanged);
    connect(m_diskThresholdSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MonitoringDashboard::onDiskThresholdChanged);
}

MonitoringDashboard::~MonitoringDashboard()
{
    if (m_isMonitoring) {
        m_monitor->stopMonitoring();
    }
}

void MonitoringDashboard::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    createServerInfoSection();
    createMetricsSection();
    createControlsSection();
    createHistorySection();

    mainLayout->addStretch();
    centralWidget->setLayout(mainLayout);
}

void MonitoringDashboard::createServerInfoSection()
{
    QGroupBox *serverInfoGroup = new QGroupBox("Información del Servidor", this);
    QHBoxLayout *infoLayout = new QHBoxLayout();

    // URL del servidor
    QLabel *urlLabel = new QLabel("URL del Servidor:", this);
    m_serverUrlInput = new QLineEdit(this);
    m_serverUrlInput->setPlaceholderText("http://localhost:8000/api/health");
    m_serverUrlInput->setText("http://localhost:8000/api/health");
    connect(m_serverUrlInput, &QLineEdit::editingFinished, this, &MonitoringDashboard::onServerUrlChanged);

    infoLayout->addWidget(urlLabel);
    infoLayout->addWidget(m_serverUrlInput, 1);

    // Estado general
    m_statusIndicator = new QLabel(this);
    m_statusIndicator->setStyleSheet(
        "QLabel { background-color: #ff6b6b; border-radius: 25px; min-width: 50px; "
        "min-height: 50px; border: 2px solid #cc0000; }"
    );
    m_statusIndicator->setAlignment(Qt::AlignCenter);

    m_statusLabel = new QLabel("Estado: OFFLINE", this);
    m_statusLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 14px; }");

    m_messageLabel = new QLabel("Esperando conexión...", this);

    QVBoxLayout *statusVLayout = new QVBoxLayout();
    statusVLayout->addWidget(m_statusIndicator, 0, Qt::AlignCenter);
    statusVLayout->addWidget(m_statusLabel);
    statusVLayout->addWidget(m_messageLabel);

    infoLayout->addLayout(statusVLayout);

    serverInfoGroup->setLayout(infoLayout);

    // Agregar a la ventana principal
    QMainWindow::centralWidget()->layout()->addWidget(serverInfoGroup);
}

void MonitoringDashboard::createMetricsSection()
{
    QGroupBox *metricsGroup = new QGroupBox("Métricas de Rendimiento", this);
    QGridLayout *metricsLayout = new QGridLayout();

    // CPU
    QLabel *cpuTitleLabel = new QLabel("CPU:", this);
    cpuTitleLabel->setStyleSheet("QLabel { font-weight: bold; }");
    m_cpuLabel = new QLabel("0%", this);
    m_cpuLabel->setStyleSheet("QLabel { font-size: 14px; }");
    m_cpuProgressBar = new QProgressBar(this);
    m_cpuProgressBar->setMaximum(100);

    metricsLayout->addWidget(cpuTitleLabel, 0, 0);
    metricsLayout->addWidget(m_cpuLabel, 0, 1);
    metricsLayout->addWidget(m_cpuProgressBar, 0, 2);

    // Memoria
    QLabel *memoryTitleLabel = new QLabel("Memoria:", this);
    memoryTitleLabel->setStyleSheet("QLabel { font-weight: bold; }");
    m_memoryLabel = new QLabel("0%", this);
    m_memoryLabel->setStyleSheet("QLabel { font-size: 14px; }");
    m_memoryProgressBar = new QProgressBar(this);
    m_memoryProgressBar->setMaximum(100);

    metricsLayout->addWidget(memoryTitleLabel, 1, 0);
    metricsLayout->addWidget(m_memoryLabel, 1, 1);
    metricsLayout->addWidget(m_memoryProgressBar, 1, 2);

    // Disco
    QLabel *diskTitleLabel = new QLabel("Disco:", this);
    diskTitleLabel->setStyleSheet("QLabel { font-weight: bold; }");
    m_diskLabel = new QLabel("0%", this);
    m_diskLabel->setStyleSheet("QLabel { font-size: 14px; }");
    m_diskProgressBar = new QProgressBar(this);
    m_diskProgressBar->setMaximum(100);

    metricsLayout->addWidget(diskTitleLabel, 2, 0);
    metricsLayout->addWidget(m_diskLabel, 2, 1);
    metricsLayout->addWidget(m_diskProgressBar, 2, 2);

    // Uptime y response time
    m_uptimeLabel = new QLabel("Uptime: --:--:--", this);
    m_responseTimeLabel = new QLabel("Tiempo de respuesta: 0 ms", this);
    m_lastCheckLabel = new QLabel("Último chequeo: Nunca", this);

    metricsLayout->addWidget(m_uptimeLabel, 3, 0, 1, 3);
    metricsLayout->addWidget(m_responseTimeLabel, 4, 0, 1, 3);
    metricsLayout->addWidget(m_lastCheckLabel, 5, 0, 1, 3);

    metricsGroup->setLayout(metricsLayout);

    // Agregar a la ventana principal
    QMainWindow::centralWidget()->layout()->addWidget(metricsGroup);
}

void MonitoringDashboard::createControlsSection()
{
    QGroupBox *controlsGroup = new QGroupBox("Controles de Monitoreo", this);
    QGridLayout *controlsLayout = new QGridLayout();

    // Botón de refresco manual
    m_refreshButton = new QPushButton("🔄 Refresco Manual", this);
    connect(m_refreshButton, &QPushButton::clicked, this, &MonitoringDashboard::onRefreshButtonClicked);

    // Botón de inicio/parada
    m_startStopButton = new QPushButton("▶ Iniciar Monitoreo", this);
    connect(m_startStopButton, &QPushButton::clicked, this, &MonitoringDashboard::onStartStopButtonClicked);

    controlsLayout->addWidget(m_refreshButton, 0, 0);
    controlsLayout->addWidget(m_startStopButton, 0, 1);

    // Intervalo de chequeo
    QLabel *intervalLabel = new QLabel("Intervalo de chequeo (segundos):", this);
    m_intervalSpinBox = new QSpinBox(this);
    m_intervalSpinBox->setMinimum(1);
    m_intervalSpinBox->setMaximum(300);
    m_intervalSpinBox->setValue(5);
    m_intervalSpinBox->setSuffix(" seg");

    controlsLayout->addWidget(intervalLabel, 1, 0);
    controlsLayout->addWidget(m_intervalSpinBox, 1, 1);

    // Umbrales configurables
    QLabel *cpuThresholdLabel = new QLabel("Umbral CPU (%):", this);
    m_cpuThresholdSpinBox = new QSpinBox(this);
    m_cpuThresholdSpinBox->setMinimum(0);
    m_cpuThresholdSpinBox->setMaximum(100);
    m_cpuThresholdSpinBox->setValue(80);
    m_cpuThresholdSpinBox->setSuffix("%");

    controlsLayout->addWidget(cpuThresholdLabel, 2, 0);
    controlsLayout->addWidget(m_cpuThresholdSpinBox, 2, 1);

    QLabel *memoryThresholdLabel = new QLabel("Umbral Memoria (%):", this);
    m_memoryThresholdSpinBox = new QSpinBox(this);
    m_memoryThresholdSpinBox->setMinimum(0);
    m_memoryThresholdSpinBox->setMaximum(100);
    m_memoryThresholdSpinBox->setValue(85);
    m_memoryThresholdSpinBox->setSuffix("%");

    controlsLayout->addWidget(memoryThresholdLabel, 3, 0);
    controlsLayout->addWidget(m_memoryThresholdSpinBox, 3, 1);

    QLabel *diskThresholdLabel = new QLabel("Umbral Disco (%):", this);
    m_diskThresholdSpinBox = new QSpinBox(this);
    m_diskThresholdSpinBox->setMinimum(0);
    m_diskThresholdSpinBox->setMaximum(100);
    m_diskThresholdSpinBox->setValue(90);
    m_diskThresholdSpinBox->setSuffix("%");

    controlsLayout->addWidget(diskThresholdLabel, 4, 0);
    controlsLayout->addWidget(m_diskThresholdSpinBox, 4, 1);

    controlsGroup->setLayout(controlsLayout);

    // Agregar a la ventana principal
    QMainWindow::centralWidget()->layout()->addWidget(controlsGroup);
}

void MonitoringDashboard::createHistorySection()
{
    QGroupBox *historyGroup = new QGroupBox("Historial de Eventos (Últimos 20)", this);
    QVBoxLayout *historyLayout = new QVBoxLayout();

    m_eventHistoryList = new QListWidget(this);
    m_eventHistoryList->setMaximumHeight(250);

    historyLayout->addWidget(m_eventHistoryList);

    historyGroup->setLayout(historyLayout);

    // Agregar a la ventana principal
    QMainWindow::centralWidget()->layout()->addWidget(historyGroup);
}

void MonitoringDashboard::onRefreshButtonClicked()
{
    m_monitor->performManualCheck();
    qDebug() << "Refresco manual ejecutado";
}

void MonitoringDashboard::onStartStopButtonClicked()
{
    if (!m_isMonitoring) {
        QString url = m_serverUrlInput->text();
        if (url.isEmpty()) {
            m_messageLabel->setText("Error: Ingrese la URL del servidor");
            return;
        }

        m_monitor->setServerUrl(url);
        m_monitor->startMonitoring(m_intervalSpinBox->value() * 1000);
        m_isMonitoring = true;
        m_startStopButton->setText("⏹ Detener Monitoreo");
        m_uptimeUpdateTimer->start(1000);
        m_serverUrlInput->setReadOnly(true);
    } else {
        m_monitor->stopMonitoring();
        m_isMonitoring = false;
        m_startStopButton->setText("▶ Iniciar Monitoreo");
        m_uptimeUpdateTimer->stop();
        m_serverUrlInput->setReadOnly(false);
    }
}

void MonitoringDashboard::onHealthDataUpdated(const ServerHealthData &data)
{
    qDebug() << "Dashboard actualizado con datos de salud";

    // Actualizar métricas
    m_cpuProgressBar->setValue(static_cast<int>(data.cpuUsage));
    m_cpuLabel->setText(QString::number(data.cpuUsage, 'f', 1) + "%");

    m_memoryProgressBar->setValue(static_cast<int>(data.memoryUsage));
    m_memoryLabel->setText(QString::number(data.memoryUsage, 'f', 1) + "%");

    m_diskProgressBar->setValue(static_cast<int>(data.diskUsage));
    m_diskLabel->setText(QString::number(data.diskUsage, 'f', 1) + "%");

    // Actualizar tiempo de respuesta (RTT total) y tiempo de procesamiento en server
    m_responseTimeLabel->setText("Tiempo de respuesta: " + QString::number(data.responseTime) + " ms (server: " + QString::number(data.serverResponseTime) + " ms)");

    // Actualizar último chequeo
    m_lastCheckLabel->setText("Último chequeo: " + data.lastCheck.toString("hh:mm:ss"));

    // Actualizar estado
    updateStatusIndicator(data.status);
    m_messageLabel->setText(data.message);
}

void MonitoringDashboard::onStatusChanged(const QString &status)
{
    m_statusLabel->setText("Estado: " + status);
    updateStatusIndicator(status);
}

void MonitoringDashboard::onEventLogged(const HealthEvent &event)
{
    QString eventText = event.timestamp.toString("hh:mm:ss") + " - [" + event.severity + "] " + event.description;

    // Limitar a últimos 20 eventos
    if (m_eventHistoryList->count() >= 20) {
        delete m_eventHistoryList->takeItem(0);
    }

    m_eventHistoryList->addItem(eventText);
    m_eventHistoryList->scrollToBottom();
}

void MonitoringDashboard::onConnectionError(const QString &error)
{
    m_messageLabel->setText("Error: " + error);
    m_statusLabel->setText("Estado: OFFLINE");
    updateStatusIndicator("OFFLINE");
    qDebug() << "Error de conexión:" << error;
}

void MonitoringDashboard::onIntervalChanged(int value)
{
    if (m_isMonitoring) {
        m_monitor->setCheckInterval(value * 1000);
    }
}

void MonitoringDashboard::onCpuThresholdChanged(int value)
{
    m_monitor->setCpuThreshold(value);
}

void MonitoringDashboard::onMemoryThresholdChanged(int value)
{
    m_monitor->setMemoryThreshold(value);
}

void MonitoringDashboard::onDiskThresholdChanged(int value)
{
    m_monitor->setDiskThreshold(value);
}

void MonitoringDashboard::onServerUrlChanged()
{
    qDebug() << "URL del servidor cambiada a:" << m_serverUrlInput->text();
}

void MonitoringDashboard::updateStatusIndicator(const QString &status)
{
    QColor color = getStatusColor(status);
    m_statusIndicator->setStyleSheet(
        QString("QLabel { background-color: %1; border-radius: 25px; min-width: 50px; "
                "min-height: 50px; border: 2px solid %2; }")
            .arg(color.name())
            .arg(color.darker(200).name())
    );
}

void MonitoringDashboard::updateUptimeDisplay()
{
    ServerHealthData data = m_monitor->getCurrentData();
    m_uptimeLabel->setText("Uptime: " + formatUptime(data.uptime));
}

QString MonitoringDashboard::formatUptime(int seconds)
{
    int days = seconds / 86400;
    int hours = (seconds % 86400) / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;

    return QString("%1d %2:%3:%4")
        .arg(days)
        .arg(hours, 2, 10, QLatin1Char('0'))
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0'));
}

QColor MonitoringDashboard::getStatusColor(const QString &status)
{
    if (status == "OK") {
        return QColor("#4CAF50");  // Verde
    } else if (status == "WARNING") {
        return QColor("#FFC107");  // Amarillo
    } else if (status == "CRITICAL") {
        return QColor("#FF5722");  // Rojo oscuro
    } else {
        return QColor("#9E9E9E");  // Gris
    }
}
