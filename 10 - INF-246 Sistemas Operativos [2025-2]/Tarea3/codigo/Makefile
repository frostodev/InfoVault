# Compiladores y Herramientas
CXX = g++
JCC = javac
JVM = java
PYTHON = python3
TIME_CMD = /usr/bin/time -v

# Flags (para C++) 
CXXFLAGS = -std=c++17 -O0 -Wall
LDFLAGS = -lstdc++fs

# Nombres de Archivos
CPP_EXEC = lab3
JAVA_CLASS = LAB3_Acosta_Carcamo
PLOT_SCRIPT = script_plots.py

# Reglas .PHONY (Acciones)
.PHONY: all run stats clean

# 'make' o 'make all' -> Solo compila
all: $(CPP_EXEC) $(JAVA_CLASS).class

# 'make run' -> Ejecuta los programas
run: all
	@echo "Ejecutando C++ (Forks)..."
	@./$(CPP_EXEC)
	@echo "Ejecutando Java (Threads)..."
	@$(JVM) $(JAVA_CLASS)

# 'make stats' -> Ejecuta con 'time' para medir RAM/CPU
stats: all
	@echo "Midiendo recursos de C++ (Forks)..."
	@$(TIME_CMD) -o ../codigo/data/measurements/stats_cpp.txt ./$(CPP_EXEC)
	@echo "Midiendo recursos de Java (Threads)..."
	@$(TIME_CMD) -o ../codigo/data/measurements/stats_java.txt $(JVM) $(JAVA_CLASS)
	@echo "Reportes de recursos generados en ../data/measurements/"
	@echo "Generando graficos..."
	@$(PYTHON) $(PLOT_SCRIPT)

# 'make clean' -> Limpia todo lo generado
clean:
	@echo "Limpiando archivos compilados..."
	@rm -f $(CPP_EXEC) *.class
	@echo "Limpiando archivos de salida..."
	@rm -f ../codigo/data/outputs/salidaFork.txt ../codigo/data/outputs/salidaThread.txt
	@rm -f ../codigo/data/measurements/datos_cpp.csv ../codigo/data/measurements/datos_java.csv
	@rm -f ../codigo/data/measurements/stats_cpp.txt ../codigo/data/measurements/stats_java.txt
	@rm -f ../codigo/data/plots/*.png

# Compilar C++
$(CPP_EXEC): LAB3_Acosta_Carcamo.cpp
	@echo "Compilando C++..."
	$(CXX) $(CXXFLAGS) LAB3_Acosta_Carcamo.cpp -o $(CPP_EXEC) $(LDFLAGS)

# Compilar Java
$(JAVA_CLASS).class: LAB3_Acosta_Carcamo.java
	@echo "Compilando Java..."
	$(JCC) LAB3_Acosta_Carcamo.java