# Variables
COMMON_DIR = common
MAIN_PROGRAM_DIR = main_program
PLUGIN1_DIR = plugins/Plugin1
PLUGIN2_DIR = plugins/Plugin2

# Règles
all: common main_program plugin1 plugin2

# Règle pour compiler les fichiers sources du répertoire common/src
common:
	$(MAKE) -C $(COMMON_DIR)

main_program: common
	$(MAKE) -C $(MAIN_PROGRAM_DIR)

plugin1: common
	$(MAKE) -C $(PLUGIN1_DIR)

plugin2: common
	$(MAKE) -C $(PLUGIN2_DIR)

clean:
	$(MAKE) -C $(COMMON_DIR) clean
	$(MAKE) -C $(MAIN_PROGRAM_DIR) clean
	$(MAKE) -C $(PLUGIN1_DIR) clean
	$(MAKE) -C $(PLUGIN2_DIR) clean

.PHONY: all common main_program plugin1 plugin2 clean
