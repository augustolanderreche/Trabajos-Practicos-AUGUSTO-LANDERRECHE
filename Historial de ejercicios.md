ejercicio 1:
En este trabajo se realizó el entregable de la clase 5

En el Código se le pidió a Copilot que realice el Código con la estructura adecuada y pensado para ejecutarse en QT con un diseño moderno
El Código es de un gestor de trabajos prácticos. En él se pueden crear los trabajos prácticos con 2 estados, completado o pendiente y con 3 prioridades, baja, media, alta. Se pueden poner fechas límites y luego de crearlos se pueden modificar, eliminar, completar y agregar una nota. Todos los cambios o agregados de trabajos se registran en un historial que se encuentra en la parte inferior, dentro de ese historial se podrá ver la fecha y hora de las modificaciones, el ID del trabajo, el usuario que realizo las acciones, las acciones realizadas y un breve detalle.

PARA INICIAR SECION: usuario: admin -- contraseña: admin

PARA REGISTRARSE: si desea registrarse, lo único que tiene que hacer es rellenar la parte del usuario con el nombre deseado y luego rellenar la parte de la contraseña, luego de eso, apretar el botón de registrarse, con eso echo se registrará el usuario nuevo y podrá iniciar sesión con dicho usuario.

BOTON RECORDAR SECION: Luego de poner su usuario y contraseña, puede mantener su sesión guardad por un máximo de 5 minutos, lo que quiere decir es que puede cerrar el programa por completo, y a la hora de ejecutarlo de nuevo, si es que no pasaron 5 minutos, su sesión anterior estará abierta.


ejercicio 2:
En el Código se le pidió a Copilot que realice el Código con la estructura adecuada y pensado para ejecutarse en QT con un diseño moderno
El codigo es de monitoreo de VPS. En el se pueden ver metricas reales y a tiempo real del servidor. Toda la guia de como usarlo esta subido dentro de la carpeta.

ejercicio3:
El codigo cumple los requerimientos principales de la consigna y presenta una
implementacion funcional de un lienzo colaborativo en tiempo real con Qt,
integrando dibujo, goma, guardado en VPS, carga remota y control basico por
usuario.
La colaboracion se resuelve mediante un modelo incremental de trazos.

Cada cliente trabaja localmente sobre su copia del lienzo y puede persistir el
estado en el servidor. El sistema mantiene informacion suficiente para:
- preservar el dibujo al cerrar y volver a abrir
- permitir trabajo compartido entre varios integrantes
- limitar el borrado a los trazos del usuario actual

Ejercicio 4:
Primero se comenzó abriendo el servidor mediante ssh desde la terminal local, luego se creo el directorio "Ejercicio-04" en el vps con el comando de Linux MKDIR. 
Posteriormente, se subieron las carpetas mediante el comando SCP, copiando así el directorio desde el local hacia el servidor.
Una vez subido se ejecuta el comando LS para verificar que se creo el directorio, luego se utilizo el comando "cd Ejercicio04" para volver a verificar si se subió el directorio que copiamos desde local se ejecuto "ls -la" y luego de ver que estaba todo correcto lo que sigue es poner los contenedores en up.
Para eso se utiliza el comando "Docker compose up -d" se esperan 30 segundos y luego mediante el comando "Docker compose ps" verificamos que ambos contenedores se encuentren en estado up, si no fuera así lo que se procede a hace es reiniciarlos.
Para eso, se bajan los contenedores con "Docker compose down" y luego se vuelve a subir con la misma metodología especificada antes.
Una vez que los estados son UP, se procede a probar en la terminal la conexión con la url para ver que la autenticación funcione. Utilizamos el comando "curl -i -u admin:[contraseña] http://ip_host:port/health".
Si todo esta ok se procede a probar el código ejecutándolo en QT Creator. 
La URL que se utiliza para el inicio de sesión es: http://192.163.180.102:8000
El usuario es: admin.
La contraseña es: 1234.

Ejercicio 5:
Se desarrollo un video que muestra como se hace un login sin QTDesing
Video del desarrollo del código subido a YouTube sin Listar.
Link:
https://youtu.be/4_-Q4PaeF3U

Ejercicio 6:
Se desarrollo un video que muestra como se hace un login con QTDesing
Video del desarrollo del login subido a YouTube sin Listar.
Link:
https://youtu.be/RZX0vMQpIkw?si=3QHWGAA2DfdU-DCv

ejercicio 7:
Este proyecto fue desarrollado en C++ con Qt y utiliza una pantalla de Login creada con Qt Designer. La aplicacion permite iniciar sesion, consultar clima real desde API, trabajar con proxy configurable y mostrar una ventana principal en pantalla completa con tarjeta de perfil.

PARA INICIAR SESION: ejecutar la app desde Qt Creator y, en la pantalla de Login, ingresar usuario admin y contrasena 1234.

PARA USAR LA APP EN DEMO: al abrir Login se mostraran temperatura, hora local y estado de conectividad. Los estados posibles son Consultando..., Online (API via proxy), Online (API directa) y Offline (datos simulados).

PARA VOLVER AL LOGIN DESDE LA VENTANA PRINCIPAL: una vez iniciada la sesion, presionar ESC en pantalla completa. La app vuelve al Login y no se cierra.

PARA CAMBIAR FOTO DE PERFIL DESDE LA CARPETA ASSETS: copiar la foto nueva dentro de assets (por ejemplo assets/foto_nueva.jpg), luego editar config.ini en la seccion [ui] y actualizar profile_photo_path=assets/foto_nueva.jpg. Al volver a ejecutar la app, se mostrara la nueva foto.

PARA CAMBIAR API KEY O CIUDAD: editar config.ini en la seccion [api] y configurar weather_url, key, city y units.

Ejemplo:
weather_url=https://api.openweathermap.org/data/2.5/weather
key=TU_API_KEY
city=Cordoba,AR
units=metric

Si la API key no es valida, la ciudad es incorrecta o falla la conectividad, la app mostrara mensajes amigables y pasara automaticamente a datos simulados para mantener el funcionamiento.

PARA ACTIVAR PROXY: editar config.ini en la seccion [proxy] y usar enabled=true, completando host, port, user y password.

Ejemplo:
enabled=true
host=IP_DEL_PROXY
port=8888
user=USUARIO
password=CLAVE

PARA DESACTIVAR PROXY: colocar enabled=false.

PARA VERIFICAR SI REALMENTE SE ESTA USANDO PROXY: en la interfaz debe aparecer Online (API via proxy) cuando lleguen datos reales por proxy. Adicionalmente, revisar eventos.log en build/Desktop_Qt_6_11_0_MinGW_64_bit-Debug/debug/logs/eventos.log y buscar lineas como Proxy habilitado para fondo y Proxy de clima habilitado.

PARA PRESENTACION CON EL PROFESOR: mostrar config.ini, ejecutar la app, iniciar sesion, volver con ESC al Login, cambiar foto o ciudad en config.ini y volver a ejecutar. Finalmente mostrar eventos.log para dejar evidencia tecnica del flujo y del uso de API/proxy.

ERRORES COMUNES:
- Proxy denied connection: la IP del cliente no esta permitida en tinyproxy.
- Proxy connection refused: servicio de proxy caido o puerto incorrecto.
- Proxy auth required: usuario o clave del proxy incorrectos.
- Sin datos reales de clima: revisar API key, city, conectividad y/o configuracion de proxy.

  Ejercicio 08:

En este trabajo se realizó el entregable de la clase 13 (Programación Orientada a Objetos).
El código fue desarrollado en C++ con Qt Widgets (qmake) enfocándose en arquitectura OO, polimorfismo obligatorio, redefinición de eventos y validación de sintaxis en tiempo real.
La aplicación es un editor de código multilenguaje (C++, Python, Java) con login seguro, sistema de bloqueo temporal tras 3 intentos fallidos, validación de sintaxis por línea al abandonar, resaltado de errores en rojo con mensajes diagnósticos amigables, exportación a JPG, registro de eventos persistente con fecha/hora, panel lateral estilo LinkedIn con perfil profesional y soporte completamente offline.

ESTRUCTURA ARQUITECTONICA:
- Jerarquía 1: Clase base abstracta Pantalla (interfaz común) con derivadas LoginScreen, ModoBloqueado, EditorPrincipal
- Jerarquía 2: Clase base abstracta ValidadorSintaxis (validación) con derivadas ValidadorCpp, ValidadorPython, ValidadorJava
- Flujo polimorfico: AppController maneja transiciones usando punteros a clase base (Pantalla*)
- Redefinición de 6 eventos en cada pantalla: keyPressEvent, mousePressEvent, resizeEvent, closeEvent, focusInEvent, focusOutEvent
- Signals/Slots para comunicación desacoplada entre componentes

PARA INICIAR SESION:
usuario: admin
contraseña: 1234

PARA USAR LA APLICACION:
1. PANTALLA DE LOGIN:
   - Ingresar usuario "admin"
   - Ingresar contraseña "1234"
   - Presionar "Ingresar" o Enter
   - Si falla 3 veces: se activa modo bloqueado con cuenta regresiva de 15 segundos (configurable en config.ini)
   - Tras espera: vuelve a login para reintentar

2. EDITOR PRINCIPAL (fullscreen tras login exitoso):
   - Selector de lenguaje: dropdown con C++, Python, Java
   - Al cambiar lenguaje: editor se vacía y carga template inicial del lenguaje seleccionado
   - Escribir código en el lenguaje seleccionado
   - Al abandonar línea (Enter, Tab, click otra línea, perder focus): se valida automáticamente
   - Línea válida: sin marca de color
   - Línea inválida: resaltado en ROJO + mensaje diagnóstico específico del error
   
3. VALIDACION DE SINTAXIS POR LENGUAJE:
   C++:
   ✓ Comentarios //, palabras clave (if, for, class, void, int, etc.), terminación con ; o bloques {}
   ✗ Rechaza: print(), def, class x:, System.out (palabras de Python/Java)
   Python:
   ✓ Comentarios #, indentación múltiplos de 4, palabras clave (if:, for:, def, class, etc.), bloques con :
   ✗ Rechaza: cout, void, System.out, {}, ; al final (palabras de C++/Java)
   Java:
   ✓ Comentarios //, palabras clave (public, class, if, for, etc.), terminación con ; o bloques {}
   ✗ Rechaza: print(), def, import, : al final (palabras de Python/C++)

4. ATAJOS DE TECLADO:
   Enter → Intentar login (en pantalla de login)
   Ctrl+S → Exportar código a JPG (en editor)
   ESC → Cerrar editor y volver a login

5. EXPORTACION A JPG:
   - Botón "Exportar JPG (Ctrl+S)" en toolbar
   - Se renderiza todo el código a imagen JPEG legible
   - Se guarda en: salidas/codigo.jpg (configurable en config.ini)
   - Se muestra diálogo con ruta final de guardado
   - Si editor está vacío: muestra advertencia y no exporta

6. REGISTRO DE EVENTOS:
   - Todos los eventos se registran en: logs/eventos.log
   - Formato: [YYYY-MM-DD HH:MM:SS] | descripción
   - Incluye: logins, intentos fallidos, cambios de lenguaje, validaciones, exportaciones, etc.

ARCHIVOS Y CARPETAS GENERADOS EN EJECUCION:
- config.ini: configuración de usuario, credenciales, tiempos de bloqueo, rutas (auto-creado si no existe)
- logs/eventos.log: registro de eventos con timestamp
- salidas/codigo.jpg: exportación del código escrito

CONFIGURACION (config.ini):
[auth]
usuario=admin
clave=1234
tiempoBloqueoSegundos=15

[editor]
lenguajePorDefecto=C++
rutaExportacion=salidas/codigo.jpg

[app]
rutaLog=logs/eventos.log

>>> Modificar valores según necesidad para cambiar credenciales, tiempo de bloqueo o rutas <<<

CARACTERISTICAS TECNICAS DESTACADAS:

1. Polimorfismo en 2 jerarquías:
   - AppController usa punteros a Pantalla (base) que apuntan a LoginScreen, ModoBloqueado o EditorPrincipal en runtime
   - EditorPrincipal valida código con puntero a ValidadorSintaxis (base) que apunta a Cpp, Python o Java según selección

2. Validadores estrictos por lenguaje:
   - Cada validador rechaza palabras clave de otros lenguajes inmediatamente
   - Imposible mezclar código de diferentes lenguajes sin que dispare error

3. Signals/Slots desacoplados:
   - LoginScreen → AppController: loginExitoso(), bloqueoSolicitado()
   - ModoBloqueado → AppController: bloqueoFinalizado()
   - CodeEditor → EditorPrincipal: lineaAbandonada(), editorPerdioFoco(), editorRecibioFoco()
   - UI → métodos privados: onLenguajeCambiado(), exportarAJpg(), limpiarCodigo()

4. Redefinición consciente de eventos:
   - LoginScreen: Enter intenta login, confirmación en closeEvent
   - ModoBloqueado: QTimer de 15 segundos, cuenta regresiva visual
   - EditorPrincipal: Ctrl+S exporta, ESC cierra, validación en focusOut y lineChange

5. Persistencia completa:
   - config.ini auto-creada con valores por defecto
   - Directorios logs/ y salidas/ auto-creados en startup
   - Log persistente con formato estándar
   - Exportación JPG guardada permanentemente

6. UI responsiva:
   - Panel CV estilo LinkedIn con avatar circular, descripción, habilidades, contacto
   - Toolbar con selector de lenguaje y botones de acción
   - Diagnostico textual de errores bajo el editor
   - Resaltado de líneas inválidas en ROJO con mensaje específico

LIMITACIONES Y CONSIDERACIONES:
- Validación por línea no analiza contexto multi-línea (bucles incompletos, etc.)
- Exportación JPG es blanco y negro sin coloreado sintáctico
- Credenciales en config.ini (en producción usar BD con hashing)
- Sistema offline: no sincroniza a la nube
- Validación es heurística/didáctica, no reemplaza compiladores reales

TROUBLESHOOTING:

Problema: No compila - "header not found"
Solución: Verificar que Qt 6.x esté instalado y kit seleccionado correctamente

Problema: config.ini no se crea
Solución: Verificar permisos de escritura en directorio de ejecución

Problema: Validación muy estricta
Solución: Es por diseño. Editar validadores.cpp si se desean cambiar reglas

Problema: JPG no se guarda
Solución: Verificar permisos en carpeta salidas/ y espacio disponible en disco

Problema: Log vacío
Solución: Verificar permisos en carpeta logs/ y que exista

Ejercicio09:
"Se desarrollo una app en Qt Widgets con login y registro de usuarios
sobre SQLite. El login se disenia con Qt Designer y el lienzo
se programa en una clase QWidget propia. Cada trazo se guarda
como puntos (x, y, color, grosor) para reconstruir el dibujo
cuando se vuelve a abrir la aplicacion. Tambien guardo logs
de acceso exitoso y fallido, y las contrasenias van hasheadas
con SHA-256 en la base de datos." 

Como se guarda el dibujo en DB:
Idea clave: un dibujo se guarda en 2 niveles.
1) Tabla trazos:
- Cada vez que el usuario termina un trazo (mouse release),
  se crea un registro nuevo en trazos y se obtiene su id.
2) Tabla puntos:
- Cada punto del trazo guarda:
  - trazo_id
  - x, y
  - r, g, b
  - grosor

Reconstruccion:
- Al iniciar, se leen los trazos ordenados por id.
- Para cada trazo, se leen sus puntos ordenados por id.
- Se repintan en el mismo orden y aparece el dibujo previo.

 El deshacer:
- Se maneja por trazos completos, no por pixel.
- Ctrl+Z elimina el ultimo trazo de memoria.
- Tambien borra ese trazo de la base (trazos + puntos).
- Se limita a 10 acciones para cumplir consigna.

Seguridad de contraseñas:
- No se guarda clave en texto plano en registros nuevos.
- Se usa hash SHA-256 (QCryptographicHash).
- En login se compara hash(clave_ingresada) vs clave_guardada.
- Se incluyo migracion automatica para claves viejas en texto plano.

limitaciones:
- SHA-256 sin salt no es lo mas fuerte para produccion.
- Para sistema real conviene Argon2/PBKDF2/bcrypt + salt.
- Para trabajo practico de la materia, esta correcto y mejora
  claramente respecto a texto plano.
