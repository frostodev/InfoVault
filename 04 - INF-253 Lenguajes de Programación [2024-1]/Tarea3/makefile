# Compilación de los archivos de Java
%.class:%.java
	@javac $<

# Regla para compilar todos los archivos de Java
all: 
	@javac -Xlint:all LP/*.java

# Regla para ejecutar el archivo principal (UltraJava)
run: all
	java LP/Ultrajava

# Limpieza de archivos compilados
clean:
	rm -f LP/*.class