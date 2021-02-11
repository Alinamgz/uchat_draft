# ---------------- variables ----------------
CLIENT := uchat

# -- files and dirs --
LIB_DIR := libs
LIB_INC_DIR := $(LIB_DIR)/include

SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj

INC_FILES := $(wildcard $(INC_DIR)/*.h)
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:%.c=%.o)))

# ----- commands -----
RM := rm -rdf
MKDIR := mkdir -p

CC := clang

CFLAGS := -rdynamic `pkg-config --cflags --libs gtk+-3.0`\
          -std=c11 -Wall -Werror -Wextra -Wpedantic\
          -Wno-unused-command-line-argument -Wno-unused-variable \
          -Wno-unused-function -Wno-unused-parameter -g

# ------------------ body -------------------
all: $(CLIENT)

$(CLIENT): $(OBJ_FILES)
	@$(CC)  $(CFLAGS) $(OBJ_FILES) -L $(LIB_DIR) -lcjson -o $@

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	@$(CC) $(CFLAGS) -c $< -I $(INC_DIR) -I $(LIB_INC_DIR) -o $@

clean:
	@$(RM) $(OBJ_DIR)

uninstall: clean
	@$(RM) $(CLIENT) $(LIB_DIR)

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
