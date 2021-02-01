# ---------------- variables ----------------
SERVER_NAME := uchat_server
CLIENT_NAME := uchat
CJSON_NAME := libcjson
SQLITE_NAME := libsqlite3

# -- files and dirs --
SERVER_DIR := server
CLIENT_DIR := client
CJSON_DIR := libs/libs_raw/cjson
SQLITE_DIR := libs/libs_raw/sqlite3
LIBS_DIR := libs/libs
LIBS_INC_DIR := libs/include

SERVER = $(SERVER_DIR)/$(SERVER_NAME)
CLIENT = $(CLIENT_DIR)/$(CLIENT_NAME)
CJSON = $(LIBS_DIR)/$(CJSON_NAME).a
SQLITE = $(LIBS_DIR)/$(SQLITE_NAME).a

# ----- commands -----
RM := rm -rdf
MKDIR = mkdir -p

# ------------------ body -------------------
all: $(CJSON_NAME) $(SQLITE_NAME) $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER)
$(CLIENT_NAME): $(CLIENT)
$(CJSON_NAME): $(CJSON)
$(SQLITE_NAME): $(SQLITE)

$(SERVER): $(SERVER_DIR)
$(CLIENT): $(CLIENT_DIR)
$(CJSON): $(CJSON_DIR)
$(SQLITE): $(SQLITE_DIR)

$(SERVER) $(CLIENT):
	@$(MAKE) -sC $<
	@cp $@ .

$(CJSON) $(SQLITE):| $(LIBS_DIR) $(LIBS_INC_DIR)
	@$(MAKE) -sC $<

$(LIBS_DIR) $(LIBS_INC_DIR):
	@$(MKDIR) $(LIBS_DIR) $(LIBS_INC_DIR)

clean:
	@$(MAKE) clean -sC $(SERVER_DIR)
	@$(MAKE) clean -sC $(CLIENT_DIR)
	@$(MAKE) clean -sC $(CJSON_DIR)
	@$(MAKE) clean -sC $(SQLITE_DIR)

uninstall: clean
	@$(MAKE) uninstall -sC $(SERVER_DIR)
	@$(MAKE) uninstall -sC $(CLIENT_DIR)
	@$(MAKE) uninstall -sC $(CJSON_DIR)
	@$(MAKE) uninstall -sC $(SQLITE_DIR)
	@$(RM) $(SERVER_NAME)
	@$(RM) $(CLIENT_NAME)
	@$(RM) $(LIBS_DIR)
	@$(RM) $(LIBS_INC_DIR)

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
