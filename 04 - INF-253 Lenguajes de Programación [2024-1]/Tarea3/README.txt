Nombre: Sergio Cárcamo Naranjo.
Rol: -

Se probó en Arch Linux y Ubuntu WSL2; con javac 11.0.2 y Make 4.3
Para ejecutar el programa, se debe compilar desde el directorio que contiene la makefile de la siguiente forma:

make
make run

NOTA: El makefile incluye el parámetro -Xlint:all para mostrar todos los Warning (los cuales este código no tiene)
      Sin embargo, Visual Studio Code muestra algunos Warnings que javac no, por lo que no deben ser considerados.