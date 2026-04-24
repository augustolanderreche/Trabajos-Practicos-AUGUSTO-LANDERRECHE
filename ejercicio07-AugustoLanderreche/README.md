# Ejercicio 07 - Login con clima, proxy y ventana principal fullscreen (Qt/C++)

## 1. Descripcion general
Este proyecto implementa una aplicacion de escritorio en **Qt (C++)** con:

- Pantalla de login con validacion de credenciales (`admin` / `1234`)
- Interfaz de Login construida en **Qt Designer** (archivo [login.ui](login.ui))
- Consulta de clima real por API (OpenWeather)
- Soporte de proxy HTTP configurable
- Fallback a datos simulados cuando falla red/API
- Descarga y cache local de imagen de fondo
- Ventana principal en fullscreen con tarjeta tipo CV
- Logging de eventos para diagnostico y evidencia

La aplicacion fue disenada para cumplir la consigna academica de uso de clases, herencia, abstraccion y consumo de API de terceros usando `QNetworkAccessManager`.

---

## 2. Objetivos de la entrega
- Implementar autenticacion simple en interfaz grafica.
- Consumir una API REST externa sin SDK.
- Manejar conectividad real, proxy y errores de red.
- Mostrar informacion amigable al usuario (online, proxy, datos simulados).
- Mantener arquitectura limpia y separacion de responsabilidades.

---

## 3. Tecnologias y herramientas
- **Lenguaje:** C++
- **Framework:** Qt 6 (Widgets + Network)
- **Build system:** qmake (`.pro`)
- **IDE recomendada:** Qt Creator
- **API externa:** OpenWeather
- **Proxy opcional:** Tinyproxy en VPS

---

## 4. Estructura del proyecto

### Archivos principales
- [main.cpp](main.cpp): punto de entrada (`QApplication`, muestra `Login`).
- [pantalla.h](pantalla.h), [pantalla.cpp](pantalla.cpp): clase abstracta base de pantallas.
- [login.h](login.h), [login.cpp](login.cpp), [login.ui](login.ui): login, estado de red y fondo.
- [ventana.h](ventana.h), [ventana.cpp](ventana.cpp): ventana principal fullscreen y perfil.
- [datosbase.h](datosbase.h): clase abstracta base para datos.
- [climaservicio.h](climaservicio.h), [climaservicio.cpp](climaservicio.cpp): consumo de API clima.
- [appconfig.h](appconfig.h), [appconfig.cpp](appconfig.cpp): carga de `config.ini`.
- [logger.h](logger.h), [logger.cpp](logger.cpp): logger a archivo con timestamp.
- [config.ini](config.ini): configuracion runtime (API, UI, proxy).
- [ejercicio06-AugustoLanderreche.pro](ejercicio06-AugustoLanderreche.pro): configuracion de proyecto Qt.

### Documentacion incluida
- [SETUP_PROXY_VPS_TINYPROXY.txt](SETUP_PROXY_VPS_TINYPROXY.txt): guia de setup de proxy en VPS.

---

## 5. Arquitectura y diseno

### 5.1 Jerarquia de clases
- `Pantalla` (abstracta)
  - `Login`
  - `Ventana`

- `DatosBase` (abstracta)
  - `ClimaServicio`

### 5.2 Responsabilidades
- **Login:** autenticacion, bloqueo por intentos, estados de conectividad, arranque de servicios.
- **ClimaServicio:** request HTTP, parseo JSON, reintentos, errores amigables, fallback simulado.
- **Ventana:** vista principal, render de fondo y tarjeta de perfil, retorno a login con ESC.
- **AppConfig:** centraliza lectura de configuracion y proxy.
- **Logger:** registro de eventos tecnicos/funcionales para soporte y defensa.

---

## 6. Flujo funcional
1. Se inicia la app y se muestra Login.
2. Se carga `config.ini` (priorizando la raiz del proyecto).
3. Se configura proxy (si `enabled=true`).
4. Se consulta clima:
   - si hay respuesta valida: datos reales
   - si falla: datos simulados + mensaje amigable
5. Se prepara fondo (cache local o descarga).
6. Usuario valida login (`admin` / `1234`).
7. Si credenciales correctas y fondo listo: se abre `Ventana` en fullscreen.
8. En fullscreen, al presionar `ESC`, vuelve al login (no se cierra la app).

---

## 7. Funcionalidades implementadas

### 7.1 Login y seguridad basica
- Formulario de Login desarrollado con **Qt Designer** sobre [login.ui](login.ui)
- Usuario y clave validos: `admin` / `1234`
- 3 intentos fallidos consecutivos -> bloqueo de 30 segundos
- Soporte de Enter en usuario/password para ejecutar login

### 7.2 Clima y conectividad
- Consulta a OpenWeather por URL + query params
- Parseo robusto de JSON
- Deteccion de errores HTTP (401, 404, 429, 5xx)
- Modo offline con datos simulados
- Reintentos automaticos con backoff (5s, 10s, 15s)

### 7.3 Mensajeria de estado (UI)
La pantalla muestra estados claros sobre el origen de datos:
- **Online (API via proxy):** datos reales pasando por proxy
- **Online (API directa):** datos reales sin proxy
- **Offline (datos simulados):** fallback por error de red/API
- **Consultando...:** request en curso

### 7.4 Fondo e imagenes
- Fondo descargado por URL publica
- Cache local para evitar descargas repetidas
- Fallback a fondo generado localmente si falla la descarga
- Foto de perfil local prioritaria; URL como respaldo

### 7.5 Ventana principal
- Apertura en fullscreen
- Escalado de fondo sin deformacion (`KeepAspectRatioByExpanding`)
- Cierre por `ESC` con retorno al login

### 7.6 Logging
- Archivo: `build/.../debug/logs/eventos.log`
- Registra: carga de config, requests, errores, login, apertura/cierre de ventana, proxy

---

## 8. Configuracion (`config.ini`)
Ejemplo minimo:

```ini
[api]
weather_url=https://api.openweathermap.org/data/2.5/weather
key=TU_API_KEY
city=Cordoba,AR
units=metric

[ui]
background_url=https://picsum.photos/1920/1080
profile_photo_path=assets/fotoPerfil.jpg
profile_photo_url=https://randomuser.me/api/portraits/men/32.jpg

[proxy]
enabled=false
host=
port=8888
user=
password=
```

### Notas de proxy
- Si no usas proxy: `enabled=false`
- Si usas proxy: `enabled=true` y completar host/port/user/password
- Ver guias: [SETUP_PROXY_VPS_TINYPROXY.txt](SETUP_PROXY_VPS_TINYPROXY.txt)

---

## 9. Compilacion y ejecucion

### Opcion A - Qt Creator (recomendada)
1. Abrir [ejercicio06-AugustoLanderreche.pro](ejercicio06-AugustoLanderreche.pro).
2. Seleccionar kit Qt 6 + MinGW.
3. Build.
4. Run.

### Opcion B - Consola (qmake)
Desde la carpeta del proyecto:

```bash
qmake ejercicio06-AugustoLanderreche.pro
mingw32-make
./debug/ejercicio06-AugustoLanderreche.exe
```

(En Windows, ejecutar desde entorno con Qt/MinGW en PATH o usar Qt Creator)

---

## 10. Validacion funcional recomendada
Para demostrar cobertura completa en defensa:

1. **Caso online sin proxy**
- `proxy/enabled=false`
- Ver estado: `Online (API directa)`

2. **Caso online con proxy**
- `proxy/enabled=true`
- Ver estado: `Online (API via proxy)`

3. **Caso error de API key (401)**
- API key invalida
- Debe mostrar fallback simulado y mensaje amigable

4. **Caso ciudad invalida (404)**
- city incorrecta
- Debe mostrar mensaje amigable y fallback

5. **Caso sin conectividad / proxy caido**
- Simular corte o puerto incorrecto
- Debe activar reintentos y luego datos simulados

6. **Caso bloqueo login**
- 3 credenciales incorrectas
- Debe bloquear 30 segundos

7. **Caso retorno por ESC**
- Iniciar sesion
- En fullscreen presionar ESC
- Debe volver al login

---

## 11. Troubleshooting rapido
- **No conecta con proxy:** revisar `Allow` en tinyproxy + reglas UFW + puerto.
- **Proxy denied connection:** IP cliente no permitida en tinyproxy.
- **Proxy auth required:** usuario/clave de proxy incorrectos.
- **Timeout:** proxy no disponible o ruta bloqueada.
- **No toma config esperada:** revisar log `Configuracion cargada desde: ...`.

---

## 12. Estado de cumplimiento
Implementado y operativo:
- UI login en Qt Designer
- Herencia/abstraccion (`Pantalla`, `DatosBase`)
- API de terceros con `QNetworkAccessManager`
- Manejo de proxy configurable
- Manejo de errores y mensajes amigables
- Modo offline simulado
- Ventana fullscreen con retorno a login por ESC
- Logging completo para diagnostico y defensa

---

## 13. Autor
- Augusto Landerreche

