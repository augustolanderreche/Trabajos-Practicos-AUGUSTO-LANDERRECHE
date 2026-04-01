#include "serverhealthmonitor.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QDebug>
#include <QDateTime>
#include <cmath>

ServerHealthMonitor::ServerHealthMonitor(QObject *parent)
    : QObject(parent),
      m_networkManager(new QNetworkAccessManager(this)),
      m_checkTimer(new QTimer(this)),
      m_cpuThreshold(80.0),
      m_memoryThreshold(85.0),
      m_diskThreshold(90.0),
      m_checkIntervalMs(5000)  // 5 segundos por defecto
{
    // Inicializar datos
    m_currentData.status = "OFFLINE";
    m_currentData.cpuUsage = 0;
    m_currentData.memoryUsage = 0;
    m_currentData.diskUsage = 0;
    m_currentData.uptime = 0;
    m_currentData.responseTime = 0;
    m_currentData.serverResponseTime = 0;
    m_currentData.message = "Esperando conexión...";

    m_previousData = m_currentData;

    // Conectar timer
    connect(m_checkTimer, &QTimer::timeout, this, &ServerHealthMonitor::onCheckTimer);
}

ServerHealthMonitor::~ServerHealthMonitor()
{
    stopMonitoring();
}

void ServerHealthMonitor::setServerUrl(const QString &url)
{
    m_serverUrl = url;
    qDebug() << "URL del servidor establecida:" << m_serverUrl;
}

void ServerHealthMonitor::startMonitoring(int intervalMs)
{
    if (m_serverUrl.isEmpty()) {
        emit connectionError("URL del servidor no configurada");
        return;
    }

    m_checkIntervalMs = intervalMs;
    m_checkTimer->start(intervalMs);
    performManualCheck();  // Ejecutar chequeo inmediato
    qDebug() << "Monitoreo iniciado. Intervalo:" << intervalMs << "ms";
}

void ServerHealthMonitor::stopMonitoring()
{
    m_checkTimer->stop();
    qDebug() << "Monitoreo detenido";
}

void ServerHealthMonitor::performManualCheck()
{
    fetchHealthData();
}

void ServerHealthMonitor::setCheckInterval(int intervalMs)
{
    m_checkIntervalMs = intervalMs;
    if (m_checkTimer->isActive()) {
        m_checkTimer->setInterval(intervalMs);
    }
    qDebug() << "Intervalo de chequeo actualizado a:" << intervalMs << "ms";
}

void ServerHealthMonitor::setCpuThreshold(double threshold)
{
    m_cpuThreshold = threshold;
    qDebug() << "Umbral de CPU establecido a:" << threshold << "%";
}

void ServerHealthMonitor::setMemoryThreshold(double threshold)
{
    m_memoryThreshold = threshold;
    qDebug() << "Umbral de memoria establecido a:" << threshold << "%";
}

void ServerHealthMonitor::setDiskThreshold(double threshold)
{
    m_diskThreshold = threshold;
    qDebug() << "Umbral de disco establecido a:" << threshold << "%";
}

ServerHealthData ServerHealthMonitor::getCurrentData() const
{
    return m_currentData;
}

QVector<HealthEvent> ServerHealthMonitor::getEventHistory() const
{
    return m_eventHistory;
}

void ServerHealthMonitor::onCheckTimer()
{
    performManualCheck();
}

void ServerHealthMonitor::onNetworkReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        emit connectionError("Error: respuesta de red inválida");
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        m_currentData.status = "OFFLINE";
        m_currentData.message = "Error de conexión: " + reply->errorString();
        emit connectionError(reply->errorString());
        logEvent("ERROR", "Fallo al conectar con el servidor: " + reply->errorString(), "ERROR");
        emit healthDataUpdated(m_currentData);
        reply->deleteLater();
        return;
    }

    // Parsear respuesta JSON
    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (!jsonDoc.isObject()) {
        emit connectionError("Respuesta no es JSON válido");
        reply->deleteLater();
        return;
    }

    parseHealthData(jsonDoc.object());

    // Calcular RTT real (ida y vuelta)
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    m_currentData.responseTime = static_cast<int>(now - m_requestStartTime);

    emit healthDataUpdated(m_currentData);
    reply->deleteLater();
}

void ServerHealthMonitor::fetchHealthData()
{
    if (m_serverUrl.isEmpty()) {
        return;
    }

    m_requestStartTime = QDateTime::currentMSecsSinceEpoch();

    QNetworkRequest request{QUrl(m_serverUrl)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::UserAgentHeader, "Qt-ServerMonitor/1.0");

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &ServerHealthMonitor::onNetworkReplyFinished);
}

void ServerHealthMonitor::parseHealthData(const QJsonObject &json)
{
    m_previousData = m_currentData;

    // Parsear datos del JSON
    m_currentData.cpuUsage = json.value("cpu_usage").toDouble(0);
    m_currentData.memoryUsage = json.value("memory_usage").toDouble(0);
    m_currentData.diskUsage = json.value("disk_usage").toDouble(0);
    m_currentData.uptime = json.value("uptime").toInt(0);
    m_currentData.serverResponseTime = json.value("response_time").toInt(0);
    m_currentData.lastCheck = QDateTime::currentDateTime();

    // Evaluar la salud
    evaluateHealth();

    qDebug() << "Datos actualizados - CPU:" << m_currentData.cpuUsage
             << "% MEM:" << m_currentData.memoryUsage
             << "% DISCO:" << m_currentData.diskUsage
             << "% Estado:" << m_currentData.status;
}

void ServerHealthMonitor::evaluateHealth()
{
    QString newStatus = determineStatus();

    if (newStatus != m_previousData.status) {
        m_currentData.status = newStatus;
        emit statusChanged(newStatus);
        logEvent("STATUS_CHANGE", "Estado cambió a: " + newStatus, "INFO");
    } else {
        m_currentData.status = newStatus;
    }

    // Generar mensaje descriptivo
    if (m_currentData.cpuUsage > m_cpuThreshold) {
        logEvent("WARNING", "CPU alta: " + QString::number(m_currentData.cpuUsage, 'f', 1) + "%", "WARNING");
    }
    if (m_currentData.memoryUsage > m_memoryThreshold) {
        logEvent("WARNING", "Memoria alta: " + QString::number(m_currentData.memoryUsage, 'f', 1) + "%", "WARNING");
    }
    if (m_currentData.diskUsage > m_diskThreshold) {
        logEvent("WARNING", "Disco alto: " + QString::number(m_currentData.diskUsage, 'f', 1) + "%", "WARNING");
    }
}

void ServerHealthMonitor::logEvent(const QString &type, const QString &description, const QString &severity)
{
    // Limitar historial a últimos 50 eventos
    if (m_eventHistory.size() >= 50) {
        m_eventHistory.removeFirst();
    }

    HealthEvent event;
    event.timestamp = QDateTime::currentDateTime();
    event.eventType = type;
    event.description = description;
    event.severity = severity;

    m_eventHistory.append(event);
    emit eventLogged(event);
}

QString ServerHealthMonitor::determineStatus()
{
    // Si CPU, memoria o disco están en crítico
    if (m_currentData.cpuUsage > 95 ||
        m_currentData.memoryUsage > 95 ||
        m_currentData.diskUsage > 95) {
        m_currentData.message = "ESTADO CRÍTICO";
        return "CRITICAL";
    }

    // Si alguno está en aviso
    if (m_currentData.cpuUsage > m_cpuThreshold ||
        m_currentData.memoryUsage > m_memoryThreshold ||
        m_currentData.diskUsage > m_diskThreshold) {
        m_currentData.message = "ALERTA - Revisar métricas";
        return "WARNING";
    }

    // Todo bien
    m_currentData.message = "SISTEMA EN ÓPTIMAS CONDICIONES";
    return "OK";
}
