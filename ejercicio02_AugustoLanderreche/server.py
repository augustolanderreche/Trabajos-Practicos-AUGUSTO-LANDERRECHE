#!/usr/bin/env python3
"""
Servidor simulador de salud del VPS
Proporciona un endpoint HTTP que devuelve datos de salud del servidor
para la aplicación de monitoreo Qt.

INSTALACIÓN:
    pip install psutil

EJECUTAR LOCALMENTE:
    python server.py root@194.163.180.102

CONECTAR DESDE QT:
    http://localhost:8000/api/health

DEPLOYAR EN VPS:
    1. Copiar este archivo a tu VPS
    2. Instalar: pip install psutil
    3. Ejecutar: python server.py
    4. En Qt cambiar URL a: http://TU_VPS_IP:8000/api/health
"""

import json
import time
import os
import platform
from http.server import HTTPServer, BaseHTTPRequestHandler
from datetime import datetime

# Importar psutil si está disponible
try:
    import psutil
    HAS_PSUTIL = True
except ImportError:
    HAS_PSUTIL = False
    print("⚠️  psutil no instalado. Usando datos simulados.")
    print("   Para datos reales: pip install psutil")

class HealthHandler(BaseHTTPRequestHandler):
    """Handler para las solicitudes HTTP del servidor de salud"""
    
    def do_GET(self):
        """Maneja solicitudes GET"""
        if self.path == '/api/health':
            self.send_response(200)
            self.send_header('Content-Type', 'application/json; charset=utf-8')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.send_header('Cache-Control', 'no-cache')
            self.end_headers()
            
            health_data = self.get_health_data()
            self.wfile.write(json.dumps(health_data, indent=2).encode())
            
        elif self.path == '/':
            # Página de bienvenida
            self.send_response(200)
            self.send_header('Content-Type', 'text/html; charset=utf-8')
            self.end_headers()
            html = """
            <html>
            <head><title>VPS Health Monitor Server</title></head>
            <body style="font-family: Arial; margin: 20px;">
                <h1>✓ Servidor de Monitoreo Activo</h1>
                <p><strong>Endpoint:</strong> <code>GET http://localhost:8000/api/health</code></p>
                <p><strong>Respuesta:</strong> JSON con métricas del sistema</p>
                <hr>
                <h3>Información del Sistema:</h3>
                <ul>
                    <li>OS: {os}</li>
                    <li>Procesadores: {cpus}</li>
                    <li>RAM Total: {ram}</li>
                </ul>
                <hr>
                <p>Conecta tu aplicación Qt a este endpoint para monitorear.</p>
            </body>
            </html>
            """.format(
                os=platform.platform(),
                cpus=os.cpu_count(),
                ram=self.format_bytes(psutil.virtual_memory().total if HAS_PSUTIL else 0)
            )
            self.wfile.write(html.encode())
            
        else:
            self.send_response(404)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            self.wfile.write(json.dumps({"error": "Endpoint no encontrado"}).encode())
    
    def do_OPTIONS(self):
        """Maneja solicitudes OPTIONS (CORS)"""
        self.send_response(200)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        self.end_headers()
    
    @staticmethod
    def get_health_data():
        """Obtiene datos de salud del sistema"""
        if HAS_PSUTIL:
            return HealthHandler.get_real_data()
        else:
            return HealthHandler.get_simulated_data()
    
    @staticmethod
    def get_real_data():
        """Obtiene datos reales usando psutil"""
        try:
            start = time.time()
            cpu_percent = psutil.cpu_percent(interval=0.1)
            memory_info = psutil.virtual_memory()
            disk_info = psutil.disk_usage('/')
            uptime_seconds = int(time.time() - psutil.boot_time())
            processing_ms = int((time.time() - start) * 1000)
            
            return {
                "cpu_usage": round(cpu_percent, 1),
                "memory_usage": round(memory_info.percent, 1),
                "disk_usage": round(disk_info.percent, 1),
                "uptime": uptime_seconds,
                "response_time": processing_ms,
                "status": "OK",
                "timestamp": datetime.now().isoformat(),
                "hostname": os.uname().nodename if hasattr(os, 'uname') else platform.node()
            }
        except Exception as e:
            return HealthHandler.get_simulated_data()
    
    @staticmethod
    def get_simulated_data():
        """Obtiene datos simulados (fallback)"""
        import random
        
        return {
            "cpu_usage": round(random.uniform(20, 60), 1),
            "memory_usage": round(random.uniform(30, 70), 1),
            "disk_usage": round(random.uniform(40, 75), 1),
            "uptime": random.randint(86400, 8640000),
            "response_time": random.randint(10, 50),
            "status": "OK",
            "timestamp": datetime.now().isoformat(),
            "hostname": "simulator-server"
        }
    
    @staticmethod
    def format_bytes(bytes_size):
        """Formatea bytes a MB/GB"""
        for unit in ['B', 'KB', 'MB', 'GB']:
            if bytes_size < 1024.0:
                return f"{bytes_size:.1f} {unit}"
            bytes_size /= 1024.0
        return f"{bytes_size:.1f} TB"
    
    def log_message(self, format, *args):
        """Override para logs personalizados"""
        client_ip = self.client_address[0]
        print(f"[{self.log_date_time_string()}] {client_ip} - {format % args}")


def run_server(host='', port=8000):
    """Ejecuta el servidor de salud"""
    server_address = (host, port)
    httpd = HTTPServer(server_address, HealthHandler)
    
    print("╔════════════════════════════════════════════╗")
    print("║  VPS Health Monitor Server                 ║")
    print("╚════════════════════════════════════════════╝")
    print(f"✓ Servidor iniciado en: http://localhost:{port}")
    print(f"✓ Endpoint disponible: http://localhost:{port}/api/health")
    print(f"✓ Página de prueba: http://localhost:{port}/")
    print(f"✓ Datos: {'REALES (psutil)' if HAS_PSUTIL else 'SIMULADOS'}")
    print()
    print("Configurar en Qt:")
    print(f"  URL: http://localhost:{port}/api/health")
    print()
    print("Para usar en VPS real:")
    print(f"  URL: http://TU_VPS_IP:{port}/api/health")
    print()
    print("Presiona Ctrl+C para detener el servidor\n")
    
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\n✓ Servidor detenido")
        httpd.server_close()


if __name__ == "__main__":
    # Puedes cambiar puerto o host según necesites
    # Para acceder desde otras máquinas: run_server(host='0.0.0.0', port=8000)
    run_server(host='0.0.0.0', port=8000)

