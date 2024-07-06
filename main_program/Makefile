# Variables
BUILD_DIR = build
SRC_DIR = src
COMMON_DIR = ../common
COMMON_OBJS_DIR = $(COMMON_DIR)/build

# Compiler
CXX = g++
CXXFLAGS = -std=c++20
COMMON_OBJS = $(wildcard $(COMMON_OBJS_DIR)/*.o)

# Sources and Objects
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Exécutable
EXEC = ../test

# Cible par défaut : construire l'exécutable
all: $(EXEC)

# Règle de construction de l'exécutable
$(EXEC): $(OBJS) $(COMMON_OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Règle de compilation générique : convertir .cpp en .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Créer le répertoire build/ s'il n'existe pas
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Cible de nettoyage : supprimer les fichiers objets et l'exécutable
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
