Martín Ferrera Borquez -
Sergio Cárcamo Naranjo -

Se desarrolló usando XAMPP 3.3.0 en Windows 10.

PASOS PARA EJECUTAR:
1) Extraer el archivo. Se extraerán 3 carpetas: "php", "bd" e "imagenes".

2) Mover los archivos .php extraidos extraidos hacia la carpeta /htdocs de XAMPP, junto a la carpeta "imagenes". 
Se debe asegurar de borrar los contenidos previos de /htdocs, si es que estos existían.

3) Iniciar el servidor Apache y MySQL de XAMPP.

4) Abrir la página http://localhost/phpmyadmin desde un navegador

5) Crear una base de datos llamada "usteam"; eliminar en caso de existir.

6) Importar el archivo "basedatos.sql" ya sea mediante la pestaña Importar o copiando directamente los contenidos 
en la pestaña SQL, y ejecutar.

7) Esperar, y verificar que todo haya importado completamente.

8) La página debería poder accederse desde http://localhost/index.php

INFORMACIÓN:
- El archivo .sql contiene datos de muestra para la ejecución.

- Se puede listar todos los productos haciendo una búsqueda "vacía".

SUPUESTOS:
- El usuario tiene la cuenta bancaria vinculada con suficiente dinero para hacer la compra, de tal forma que no debe ingresar
  método de pago
  
- El usuario ingresará datos válidos a la página de administración y de búsqueda.