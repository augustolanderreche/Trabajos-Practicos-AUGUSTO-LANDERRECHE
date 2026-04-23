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