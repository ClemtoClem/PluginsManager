# Variables
BUILD_DIR = build
SRC_DIR = src
COMMON_DIR = ../../common
COMMON_OBJS_DIR = $(COMMON_DIR)/build

# Compiler
CXX = g++
CXXFLAGS = -std=c++20 -fPIC
COMMON_OBJS = $(wildcard $(COMMON_OBJS_DIR)/*.o)

# Sources and Objects
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Plugin partagé
PLUGIN_SO = ../Plugin2.so

# Cible par défaut : construire le plugin
all: $(PLUGIN_SO)

# Règle de construction du plugin
$(PLUGIN_SO): $(OBJS) $(COMMON_OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -shared $^ -o $@

# Règle de compilation générique : convertir .cpp en .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

# Créer le répertoire build/ s'il n'existe pas
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Cible de nettoyage : supprimer les fichiers objets et le plugin
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(PLUGIN_SO)

.PHONY: all clean
