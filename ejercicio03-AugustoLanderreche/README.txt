README: PAINT COLABORATIVO EN TIEMPO REAL
=======================================

Este proyecto implementa un lienzo de dibujo colaborativo en tiempo real
usando Qt/C++ para el cliente y un servidor Python/Flask en VPS.

ARCHIVOS DEL PROYECTO:
----------------------
- ejercicio03_AugustoLanderreche.pro: Archivo de proyecto Qt
- main.cpp, mainwindow.*: Ventana principal con toolbar
- canvasmodel.*: Modelo de datos (trazos, serializacion JSON)
- canvasview.*: Vista del lienzo (dibujo, eventos)
- syncmanager.*: Gestion de sincronizacion con servidor

TUTORIALES DISPONIBLES:
-----------------------
1. tutorial_servidor.txt: Configuracion del backend en VPS
2. tutorial_cliente.txt: Configuracion del cliente Qt
3. tutorial_colaboracion.txt: Pruebas de colaboracion
4. tutorial_troubleshooting.txt: Resolucion de problemas

INSTRUCCIONES RAPIDAS:
----------------------
1. Configura servidor: Lee tutorial_servidor.txt
2. Configura cliente: Lee tutorial_cliente.txt
3. Prueba colaboracion: Lee tutorial_colaboracion.txt

FUNCIONALIDADES:
---------------
- Dibujo libre con suavizado
- Colores fijos (1-9): rojo, azul, negro, violeta, naranja, verde, amarillo, rosa, gris
- Grosor variable con scroll (independiente para lapiz/goma)
- Click izquierdo: dibujar
- Click derecho: borrar
- Sincronizacion automatica cada 2 segundos
- Guardado manual con boton "Guardar"
- Fusion de trazos sin perder informacion local

REQUISITOS:
----------
- Qt 5.15+ con modulos Core, GUI, Widgets, Network
- Python 3.8+ con Flask
- VPS con Linux (Ubuntu/Debian recomendado)

COMPILACION:
-----------
qmake && make
# O en Qt Creator: Build > Build Project

EJECUCION:
---------
./ejercicio03_AugustoLanderreche

CONFIGURACION:
-------------
- URL del servidor: mainwindow.cpp linea ~25
- Intervalo sync: mainwindow.cpp linea ~30
- Colores: canvasview.cpp colorForKey()

NOTAS TECNICAS:
--------------
- Arquitectura MVC: Model-View-Controller
- Serializacion JSON para persistencia
- Polling HTTP para sincronizacion (no WebSockets)
- Merge incremental para colaboracion
- IDs unicos (QUuid) para evitar conflictos

LIMITACIONES:
------------
- Sincronizacion no instantanea (2s delay)
- Sin resolucion avanzada de conflictos
- Almacenamiento en archivo (no BD)
- Sin autenticacion/seguridad

VERSION: 1.0
AUTOR: Augusto Landerreche
FECHA: Abril 2026