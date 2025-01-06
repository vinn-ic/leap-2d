# Nome do executável
TARGET = main.exe

# Código fonte
SRC = main.cpp

# Caminhos e flags
INCLUDE = -I"C:/raylib-master/src"
LIBS = -L"C:/raylib-master/build/raylib" -lraylib -lopengl32 -lgdi32 -lwinmm

# Compilador
CXX = g++

# Regras
$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(INCLUDE) $(LIBS)

# Limpeza (opcional)
clean:
	rm -f $(TARGET)
