# ARCHITECTURE.md - Arquitectura del Sistema de Monitoreo

## 📐 Diagrama de Componentes

```
┌─────────────────────────────────────────────────────────────────┐
│                      APLICACIÓN QT (GUI)                        │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  MonitoringDashboard (QMainWindow)                       │   │
│  │                                                          │   │
│  │  ┌────────────────────────────────────────────────────┐ │   │
│  │  │ Sección: Información del Servidor                 │ │   │
│  │  │ - QLineEdit: URL del servidor                     │ │   │
│  │  │ - QLabel: Estado, Mensaje, Último chequeo       │ │   │
│  │  │ - Indicador visual: Color según estado           │ │   │
│  │  └────────────────────────────────────────────────────┘ │   │
│  │                                                          │   │
│  │  ┌────────────────────────────────────────────────────┐ │   │
│  │  │ Sección: Métricas de Rendimiento                  │ │   │
│  │  │ - QProgressBar: CPU, Memoria, Disco              │ │   │
│  │  │ - QLabel: Valores en porcentaje                  │ │   │
│  │  │ - QLabel: Uptime, Response Time                  │ │   │
│  │  └────────────────────────────────────────────────────┘ │   │
│  │                                                          │   │
│  │  ┌────────────────────────────────────────────────────┐ │   │
│  │  │ Sección: Controles de Monitoreo                   │ │   │
│  │  │ - QPushButton: Refresco Manual, Iniciar/Parar   │ │   │
│  │  │ - QSpinBox: Intervalo (seg)                      │ │   │
│  │  │ - QSpinBox: Umbrales (CPU, Mem, Disco)          │ │   │
│  │  └────────────────────────────────────────────────────┘ │   │
│  │                                                          │   │
│  │  ┌────────────────────────────────────────────────────┐ │   │
│  │  │ Sección: Historial de Eventos                     │ │   │
│  │  │ - QListWidget: Eventos con timestamp             │ │   │
│  │  │ - Máximo 20 eventos (FIFO)                       │ │   │
│  │  └────────────────────────────────────────────────────┘ │   │
│  │                                                          │   │
│  │  Signals/Slots:                                         │   │
│  │  • onHealthDataUpdated() → Actualiza métricas       │   │
│  │  • onStatusChanged() → Cambia indicador             │   │
│  │  • onEventLogged() → Añade a historial             │   │
│  │  • onRefreshButtonClicked() → Chequeo manual       │   │
│  └──────────────────────────────────────────────────────────┘   │
│                          ▲                                       │
│                          │ Conecta con                           │
│                          │ signals/slots                         │
│                          ▼                                       │
│  ┌──────────────────────────────────────────────────────────┐   │
│  │  ServerHealthMonitor (Clase propia)                     │   │
│  │                                                          │   │
│  │  Responsabilidades:                                     │   │
│  │  • Conectar a endpoint HTTP                           │   │
│  │  • Ejecutar timer de chequeos                         │   │
│  │  • Parsear respuesta JSON                            │   │
│  │  • Evaluar salud según umbrales                      │   │
│  │  • Mantener historial de eventos                     │   │
│  │  • Emitir signals de cambios                         │   │
│  │                                                          │   │
│  │  Miembros:                                              │   │
│  │  - QNetworkAccessManager*: Para HTTP                  │   │
│  │  - QTimer*: Timer de chequeos                         │   │
│  │  - ServerHealthData: Datos actuales                   │   │
│  │  - QVector<HealthEvent>: Historial (50 Max)         │   │
│  │  - double: Umbrales (CPU, Mem, Disco)               │   │
│  │                                                          │   │
│  │  Métodos públicos:                                      │   │
│  │  • setServerUrl(const QString&)                       │   │
│  │  • startMonitoring(int intervalMs)                    │   │
│  │  • performManualCheck()                               │   │
│  │  • setCheckInterval(int)                              │   │
│  │  • setCpuThreshold(double)                            │   │
│  │  • getCurrentData() → ServerHealthData               │   │
│  │  • getEventHistory() → QVector<HealthEvent>         │   │
│  │                                                          │   │
│  │  Métodos privados:                                      │   │
│  │  • fetchHealthData()                                   │   │
│  │  • parseHealthData(const QJsonObject&)                │   │
│  │  • evaluateHealth()                                    │   │
│  │  • determineStatus() → QString                         │   │
│  │  • logEvent()                                          │   │
│  │                                                          │   │
│  │  Signals emitidos:                                      │   │
│  │  • healthDataUpdated(const ServerHealthData&)          │   │
│  │  • statusChanged(const QString&)                       │   │
│  │  • eventLogged(const HealthEvent&)                     │   │
│  │  • connectionError(const QString&)                     │   │
│  └──────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────┘
                          ▲
                          │ HTTP GET
                          │ /api/health
                          ▼
┌─────────────────────────────────────────────────────────────────┐
│                     SERVIDOR PYTHON (server.py)                 │
│                                                                  │
│  HTTPServer en puerto 8000                                      │
│  HealthHandler → do_GET()                                       │
│                                                                  │
│  Endpoint: GET http://localhost:8000/api/health                │
│                                                                  │
│  Respuesta JSON:                                                │
│  {                                                              │
│    "cpu_usage": 35.5,          // psutil.cpu_percent()        │
│    "memory_usage": 42.3,       // psutil.virtual_memory()     │
│    "disk_usage": 65.8,         // psutil.disk_usage()         │
│    "uptime": 3600,             // time.time() - boot_time()   │
│    "response_time": 45,        // Random 10-100 ms            │
│    "status": "OK",             // Metadata                     │
│    "timestamp": "ISO 8601"     // Timestamp de respuesta      │
│  }                                                              │
└─────────────────────────────────────────────────────────────────┘
```

---

## 🔄 Flujo de Execución

### Inicio de la Aplicación

```
main() 
  ↓
QApplication::exec()
  ↓
MonitoringDashboard constructor
  ├─ Crear ServerHealthMonitor
  ├─ setupUI()
  │  ├─ createServerInfoSection()
  │  ├─ createMetricsSection()
  │  ├─ createControlsSection()
  │  └─ createHistorySection()
  ├─ Conectar signals/slots del monitor
  ├─ Conectar cambios en spinbox
  └─ dashboard.show()
```

### Ciclo de Monitoreo

```
Usuario hace clic "Iniciar Monitoreo"
  ↓
onStartStopButtonClicked()
  ├─ Obtener URL del QLineEdit
  ├─ m_monitor->setServerUrl(url)
  ├─ m_monitor->startMonitoring(intervalo)
  │  └─ QTimer inicia con intervalo
  └─ Cambiar botón a "Detener"
  
  
Cada intervalo de tiempo:
  ↓
QTimer → timeout
  ↓
onCheckTimer()
  ↓
fetchHealthData()
  ├─ Crear QNetworkRequest con URL
  ├─ m_networkManager->get(request)
  └─ Conectar signal finished
  
  
Cuando respuesta llega:
  ↓
onNetworkReplyFinished()
  ├─ Verificar errores
  ├─ Leer QByteArray response
  ├─ QJsonDocument::fromJson()
  └─ parseHealthData(jsonObject)
  
  
parseHealthData():
  ├─ Extraer valores de JSON
  ├─ Guardar datos anteriores
  ├─ evaluateHealth()
  │  ├─ determineStatus()
  │  ├─ Comparar con umbrales
  │  ├─ logEvent() si cambios
  │  └─ Emitir statusChanged() si estado cambió
  ├─ Emitir healthDataUpdated()
  └─ logEvent() para cambios de métrica
  
  
Dashboard recibe signals:
  ├─ onHealthDataUpdated()
  │  ├─ Actualizar m_cpuProgressBar->setValue()
  │  ├─ Actualizar m_cpuLabel->setText()
  │  ├─ Actualizar m_memoryProgressBar->setValue()
  │  ├─ Actualizar m_memoryLabel->setText()
  │  ├─ Actualizar m_diskProgressBar->setValue()
  │  ├─ Actualizar m_diskLabel->setText()
  │  ├─ Actualizar m_uptimeLabel
  │  ├─ Actualizar m_responseTimeLabel
  │  └─ updateStatusIndicator()
  │
  ├─ onStatusChanged()
  │  ├─ Actualizar m_statusLabel
  │  └─ updateStatusIndicator() con nuevo color
  │
  └─ onEventLogged()
     ├─ Formatear evento con timestamp
     ├─ Añadir a m_eventHistoryList
     ├─ Limitar a 20 items (FIFO)
     └─ Scroll to bottom
```

---

## 📊 Estructuras de Datos

### ServerHealthData

```cpp
struct ServerHealthData {
    QString status;              // "OK", "WARNING", "CRITICAL", "OFFLINE"
    double cpuUsage;             // 0-100%
    double memoryUsage;          // 0-100%
    double diskUsage;            // 0-100%
    int uptime;                  // Segundos desde boot
    int responseTime;            // Milisegundos
    QDateTime lastCheck;         // Timestamp del último chequeo
    QString message;             // Mensaje descriptivo
};
```

### HealthEvent

```cpp
struct HealthEvent {
    QDateTime timestamp;         // Cuándo ocurrió
    QString eventType;           // "STATUS_CHANGE", "WARNING", "ERROR", "RECOVERY"
    QString description;         // Descripción del evento
    QString severity;            // "INFO", "WARNING", "ERROR"
};
```

---

## 🎨 Lógica de Color de Estado

| Estado | Color Hexadecimal | RGB | Uso |
|--------|------------------|-----|-----|
| OK | #4CAF50 | (76, 175, 80) | Verde - Sistema normal |
| WARNING | #FFC107 | (255, 193, 7) | Amarillo - Alerta media |
| CRITICAL | #FF5722 | (255, 87, 34) | Rojo - Problema grave |
| OFFLINE | #9E9E9E | (158, 158, 158) | Gris - Sin conexión |

---

## 🔐 Lógica de Evaluación de Salud

```cpp
determineStatus() {
    
    if (cpu > 95 || memory > 95 || disk > 95)
        return "CRITICAL"
    
    if (cpu > cpuThreshold || memory > memoryThreshold || disk > diskThreshold)
        return "WARNING"
    
    return "OK"
}
```

**Umbrales por defecto:**
- CPU: 80%
- Memoria: 85%
- Disco: 90%

---

## 🔗 Conexiones Signal/Slot

| Signal | Slot | Acción |
|--------|------|--------|
| ServerHealthMonitor::healthDataUpdated | MonitoringDashboard::onHealthDataUpdated | Actualizar métricas |
| ServerHealthMonitor::statusChanged | MonitoringDashboard::onStatusChanged | Cambiar estado |
| ServerHealthMonitor::eventLogged | MonitoringDashboard::onEventLogged | Mostrar evento |
| ServerHealthMonitor::connectionError | MonitoringDashboard::onConnectionError | Mostrar error |
| QPushButton::clicked | MonitoringDashboard::onRefreshButtonClicked | Refresco manual |
| QPushButton::clicked | MonitoringDashboard::onStartStopButtonClicked | Iniciar/Parar |
| QSpinBox::valueChanged | MonitoringDashboard::onIntervalChanged | Cambiar intervalo |
| QSpinBox::valueChanged | MonitoringDashboard::onCpuThresholdChanged | Cambiar umbral |
| QTimer::timeout | ServerHealthMonitor::onCheckTimer | Ejecutar chequeo |
| QNetworkReply::finished | ServerHealthMonitor::onNetworkReplyFinished | Procesar respuesta |

---

## 📦 Módulos Qt Utilizados

```cpp
QT += core gui network

Desglose:
- core: QObject, QTimer, QDateTime, etc.
- gui: QMainWindow, QWidget, QLabel, QPushButton, etc.
- network: QNetworkAccessManager, QNetworkRequest, QNetworkReply
```

---

## 🧪 Casos de Prueba

### Test 1: Inicio Normal
1. Iniciar servidor Python
2. Ejecutar aplicación Qt
3. Verificar URL preconfigurada
4. Clic "Iniciar Monitoreo"
5. **Esperado**: Métricas comienzan a actualizarse

### Test 2: Cambio de Intervalo
1. Con monitoreo activo
2. Cambiar QSpinBox a 10 segundos
3. **Esperado**: Actualizaciones cada 10 seg

### Test 3: Cambio de Umbral
1. Con monitoreo activo
2. Cambiar umbral CPU a 50%
3. Si CPU actual > 50%, estado cambia a WARNING
4. **Esperado**: Indicador cambia a amarillo

### Test 4: Refresco Manual
1. Clic "Refresco Manual"
2. **Esperado**: Datos se actualizan inmediatamente

### Test 5: Error de Conexión
1. Detener servidor Python
2. **Esperado**: Mostrar error, estado OFFLINE

---

## 🏆 Justificación de Decisiones Arquitectónicas

### ¿Por qué separar ServerHealthMonitor?
- **Separación de responsabilidades**: Lógica independiente de GUI
- **Reutilizable**: Podría usarse en otras interfaces
- **Testeable**: Lógica puede probarse sin GUI
- **Mantenible**: Cambios en lógica no afectan GUI

### ¿Por qué typedef struct en lugar de class?
- Datos simples sin métodos
- Más ligero en memoria
- Fácil de copiar y serializar
- Mejor para pass-by-value frecuentes

### ¿Por qué QNetworkAccessManager en lugar de curl?
- Integración nativa con Qt
- Compatible con signal/slot
- Cross-platform automático
- Manejo de eventos asincrónico

### ¿Por qué QTimer para polling?
- Integrado con event loop de Qt
- Precisión adecuada para monitoreo (ms)
- Pausable/reanudable fácilmente
- Mejor que threads para GUI

---

## 🔮 Posibles Extensiones Futuras

1. **Gráficos**: QChart para historiales visual
2. **Base de datos**: Guardar histórico en SQLite
3. **Alertas**: Notificaciones de escritorio (QSystemTrayIcon)
4. **Múltiples servidores**: Manager de múltiples conexiones
5. **Exportación**: Generar reportes PDF/CSV
6. **SSL/TLS**: Soporte HTTPS
7. **Autenticación**: Bearer tokens/API keys
8. **Caché**: Reducir llamadas HTTP

---

