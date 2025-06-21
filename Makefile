# Variables
CC = g++                       
CFLAGS = -Wall -g -Iincludes -Isrc
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/main

# Archivos fuente y objeto
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(SRC_DIR)/glad.c
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Regla principal
all: $(TARGET)

# Linkeo final
$(TARGET): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lglfw -ldl -lGL

# Compilación de .cpp a .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./$(TARGET)

rebuild: clean all

