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
