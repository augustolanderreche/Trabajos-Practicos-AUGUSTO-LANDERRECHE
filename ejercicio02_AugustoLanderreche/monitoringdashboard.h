#ifndef MONITORINGDASHBOARD_H
#define MONITORINGDASHBOARD_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
#include <QProgressBar>
#include <QListWidget>
#include <QTimer>
#include "serverhealthmonitor.h"

/**
 * @brief Clase MonitoringDashboard
 * Panel de control profesional para monitoreo de servidor.
 * Inspirado en Grafana/Netdata para presentación clara de métricas.
 *
 * Componentes justificados:
 * - QLineEdit: Para ingresar URL del servidor (necesario para configuración dinámica)
 * - QPushButton: Para refresco manual (control interactivo del usuario)
 * - QSpinBox: Para configurar intervalo de chequeo y umbrales (valores numéricos)
 * - QLabel: Para mostrar métricas, estado y mensajes (visualización de datos)
 * - QProgressBar: Para visualizar uso en porcentaje (representación intuitiva)
 * - QListWidget: Para historial de eventos (lista ordenada de eventos cronológicos)
 */
class MonitoringDashboard : public QMainWindow
{
    Q_OBJECT

public:
    MonitoringDashboard(QWidget *parent = nullptr);
    ~MonitoringDashboard();

private slots:
    void onRefreshButtonClicked();
    void onStartStopButtonClicked();
    void onHealthDataUpdated(const ServerHealthData &data);
    void onStatusChanged(const QString &status);
    void onEventLogged(const HealthEvent &event);
    void onConnectionError(const QString &error);
    void onIntervalChanged(int value);
    void onCpuThresholdChanged(int value);
    void onMemoryThresholdChanged(int value);
    void onDiskThresholdChanged(int value);
    void onServerUrlChanged();
    void updateUptimeDisplay();

private:
    void setupUI();
    void createCentralWidget();
    void createServerInfoSection();
    void createMetricsSection();
    void createControlsSection();
    void createHistorySection();
    void updateStatusIndicator(const QString &status);
    QString formatUptime(int seconds);
    QColor getStatusColor(const QString &status);

    // Miembros privados
    ServerHealthMonitor *m_monitor;
    QTimer *m_uptimeUpdateTimer;

    // Widgets del servidor y estado general
    QLineEdit *m_serverUrlInput;
    QLabel *m_statusIndicator;
    QLabel *m_statusLabel;
    QLabel *m_messageLabel;
    QLabel *m_lastCheckLabel;
    QLabel *m_uptimeLabel;
    QLabel *m_responseTimeLabel;

    // Widgets de métricas (CPU, Memoria, Disco)
    QProgressBar *m_cpuProgressBar;
    QLabel *m_cpuLabel;
    QProgressBar *m_memoryProgressBar;
    QLabel *m_memoryLabel;
    QProgressBar *m_diskProgressBar;
    QLabel *m_diskLabel;

    // Widgets de control
    QPushButton *m_refreshButton;
    QPushButton *m_startStopButton;
    QSpinBox *m_intervalSpinBox;
    QSpinBox *m_cpuThresholdSpinBox;
    QSpinBox *m_memoryThresholdSpinBox;
    QSpinBox *m_diskThresholdSpinBox;

    // Widgets de historial
    QListWidget *m_eventHistoryList;

    // Estados
    bool m_isMonitoring;
};

#endif // MONITORINGDASHBOARD_H
