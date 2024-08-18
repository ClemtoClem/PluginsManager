# Variables
BUILD_DIR = build
SRC_DIR = src

# Compiler
CXX = g++
CXXFLAGS = -std=c++20 -fPIC

# Sources and Objects
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Cible par défaut : construire les fichiers objets
all: $(OBJS)

# Règle de compilation générique : convertir .cpp en .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Créer le répertoire build/ s'il n'existe pas
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Cible de nettoyage : supprimer les fichiers objets
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
