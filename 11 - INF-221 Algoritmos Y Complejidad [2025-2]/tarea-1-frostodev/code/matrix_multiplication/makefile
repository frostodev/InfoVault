CXX = g++
CXXFLAGS = -std=c++20 -Wall -Ialgorithms
TARGET = matrix_multiplication

# Archivos fuente
SRCS = matrix_multiplication.cpp \
       algorithms/naive.cpp \
       algorithms/strassen.cpp

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

# Limpiar
clean:
	rm -f $(TARGET)
	rm -rf build/

.PHONY: clean
