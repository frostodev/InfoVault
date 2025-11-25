Martín Ferrera Bórquez -
Sergio Cárcamo Naranjo -

La tarea fue desarrollada y probada en Windows 10 con Docker Personal

PASOS PARA LA EJECUCIÓN:

1) Abrir Docker Desktop
2) Crear el contenedor Docker con deploy-202273552-K-202273512-0.yml, usando el comando: "docker-compose -f deploy-202273552-K-202273512-0.yml up -d" en una terminal.
3) Esperar 30 segundos para que funcione, y conectarse a la instancia mongo1 usando "docker exec -it mongo1 mongo --port 30001".
4) Verificar que la instancia actual es la primaria dentro del replica-set, en caso que no sea, modificar el comando del punto 2) para cambiar de instancia.
5) En la terminal de Docker, copiar el dataset a la instancia primaria usando "docker cp usteam-202273552-K-202273512-0.json mongoX:/tmp/games.json", donde X es el número de la instancia primaria.
6) Importar el dataset copiado desde la terminal de Docker, usando "docker exec -it mongoX mongoimport --host mongoX:3000X --port 3000X --db Steam --collection Games --file /tmp/games.json --jsonArray",
   donde X es el número de la instancia primaria.
7) Abrir el cuaderno Jupyter desde otra terminal usando "jupyter notebook"

SUPUESTOS Y CONSIDERACIONES:

- No se modificó el dataset original, solamente se le cambió el nombre de "games v2.json" a "games.json".
- En la consulta 6, se considera el atributo "user_score"
- El enlace del dataset es el siguiente: "https://drive.google.com/file/d/1RwgL2iMn4XFdFhRuH0jS8GDJuEZyz2YZ/view?usp=sharing"