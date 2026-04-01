#ifndef SERVERHEALTHMONITOR_H
#define SERVERHEALTHMONITOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>
#include <QString>
#include <QJsonObject>
#include <QDateTime>

/**
 * @brief Estructura que contiene los datos de salud del servidor
 * Incluye todas las métricas principales y el estado general
 */
struct ServerHealthData {
    QString status;              // "OK", "WARNING", "CRITICAL", "OFFLINE"
    double cpuUsage;             // Porcentaje de uso de CPU (0-100)
    double memoryUsage;          // Porcentaje de uso de memoria (0-100)
    double diskUsage;            // Porcentaje de uso de disco (0-100)
    int uptime;                  // Uptime en segundos
    int responseTime;            // Tiempo total ida y vuelta (RTT) en ms
    int serverResponseTime;      // Tiempo de procesamiento del server API en ms
    QDateTime lastCheck;         // Momento del último chequeo
    QString message;             // Mensaje descriptivo del estado
};

/**
 * @brief Estructura para registrar eventos en el historial
 */
struct HealthEvent {
    QDateTime timestamp;
    QString eventType;           // "STATUS_CHANGE", "WARNING", "ERROR", "RECOVERY"
    QString description;
    QString severity;            // "INFO", "WARNING", "ERROR"
};

/**
 * @brief Clase ServerHealthMonitor
 * Responsable de monitorear la salud del servidor remoto.
 * Se conecta a un endpoint HTTP que devuelve datos en JSON.
 * Emite señales cuando los datos cambian y mantiene un historial de eventos.
 */
class ServerHealthMonitor : public QObject
{
    Q_OBJECT

public:
    explicit ServerHealthMonitor(QObject *parent = nullptr);
    ~ServerHealthMonitor();

    // Métodos públicos
    void setServerUrl(const QString &url);
    void startMonitoring(int intervalMs);
    void stopMonitoring();
    void performManualCheck();

    void setCheckInterval(int intervalMs);
    void setCpuThreshold(double threshold);
    void setMemoryThreshold(double threshold);
    void setDiskThreshold(double threshold);

    ServerHealthData getCurrentData() const;
    QVector<HealthEvent> getEventHistory() const;

signals:
    void healthDataUpdated(const ServerHealthData &data);
    void statusChanged(const QString &newStatus);
    void eventLogged(const HealthEvent &event);
    void connectionError(const QString &errorMessage);

private slots:
    void onCheckTimer();
    void onNetworkReplyFinished();

private:
    // Métodos privados
    void fetchHealthData();
    void parseHealthData(const QJsonObject &json);
    void evaluateHealth();
    void logEvent(const QString &type, const QString &description, const QString &severity);
    QString determineStatus();

    // Miembros privados
    QNetworkAccessManager *m_networkManager;
    QTimer *m_checkTimer;
    QString m_serverUrl;

    ServerHealthData m_currentData;
    ServerHealthData m_previousData;
    QVector<HealthEvent> m_eventHistory;

    qint64 m_requestStartTime;

    // Umbrales configurables
    double m_cpuThreshold;       // 80%
    double m_memoryThreshold;    // 85%
    double m_diskThreshold;      // 90%

    int m_checkIntervalMs;       // Intervalo de chequeo en ms
};

#endif // SERVERHEALTHMONITOR_H
