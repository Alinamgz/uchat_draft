# ---------------- variables ----------------
SERVER := uchat_server

# -- files and dirs --
LIB_DIR := libs
LIB_INC_DIR := $(LIB_DIR)/include

SRC_DIR := src
INC_DIR := inc
OBJ_DIR := obj

INC_FILES := $(wildcard $(INC_DIR)/*.h)
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:%.c=%.o)))
DB_FILES := uchat.db test.db

# ----- commands -----
RM := rm -rdf
MKDIR := mkdir -p

CC := clang
CFLAGS := -std=c11 -Wall -Werror -Wextra -Wpedantic

# ------------------ body -------------------

all: $(SERVER)

$(SERVER): $(OBJ_FILES)
	@$(CC) $(CFLAGS) -L $(LIB_DIR) -lcjson -lsqlite3 $(OBJ_FILES) -o $@

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	@$(CC) $(CFLAGS) -c $< -I $(INC_DIR)  -I $(LIB_INC_DIR) -o $@

clean:
	@$(RM) $(OBJ_DIR)

uninstall: clean
	@$(RM) $(SERVER) $(LIB_DIR) $(DB_FILES)

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
