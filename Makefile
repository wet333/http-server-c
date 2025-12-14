# --- Variables de Directorios ---
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# --- Configuración del Compilador ---
CC = gcc
CFLAGS = -Iinclude -Wall -g

# --- Archivos y Rutas ---
# Nombre final del ejecutable
TARGET_NAME = server
TARGET = $(BIN_DIR)/$(TARGET_NAME)

# Encontramos los sources (puedes listarlos a mano si prefieres)
SRCS = $(shell find $(SRC_DIR) -type f -name "*.c")

# Convertimos 'src/archivo.c' en 'build/archivo.o'
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# --- Regla Principal (Por defecto) ---
all: create_dirs $(TARGET)

# --- Fase de Linkeo (Crear el ejecutable) ---
$(TARGET): $(OBJS)
	@echo "Enlazando el ejecutable: $@"
	$(CC) $(OBJS) -o $@

# --- Fase de Compilación (Crear los objetos) ---
# Esta regla dice: Para hacer build/%.o, busca src/%.c
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compilando: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# --- Crear directorios si no existen ---
create_dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# --- Limpieza ---
clean:
	@echo "Limpiando archivos generados..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# .PHONY evita conflictos si existieran archivos llamados 'clean' o 'all'
.PHONY: all clean create_dirs