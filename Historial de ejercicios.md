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
