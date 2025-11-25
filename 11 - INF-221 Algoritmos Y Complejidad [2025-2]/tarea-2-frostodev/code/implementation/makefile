CXX = g++
PYTHON = python3
CXXFLAGS = -std=c++17 -Wall -Ialgorithms
TARGET = general

# Archivos fuente
SRCS =  general.cpp \
		brute-force.cpp \
		dynamic-programming.cpp \
		greedy1.cpp \
		greedy2.cpp
		
PLOT_SCRIPT = scripts/plot_generator.py

# Archivos objeto dentro de build/
OBJS = $(SRCS:.cpp=.o)
OBJS := $(addprefix build/, $(notdir $(OBJS)))

# Crear el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Regla para compilar cada .cpp en build/
build/%.o: %.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: algorithms/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
run:
	rm -f measurements/
	rm -f outputs/
	rm -f plots/
	./$(TARGET)
	$(PYTHON) $(PLOT_SCRIPT)

# Limpiar
clean:
	rm -f $(TARGET)
	rm -rf build/

.PHONY: clean
