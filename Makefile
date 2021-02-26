# ---------------- variables ----------------
SERVER_NAME := uchat_server
CLIENT_NAME := uchat
CJSON_NAME := libcjson
SQLITE_NAME := libsqlite3
DB_NAME := uchat.db

# -- files and dirs --
SERVER_DIR := server
CLIENT_DIR := client
CJSON_DIR := libraries/libs_raw/cjson
SQLITE_DIR := libraries/libs_raw/sqlite3
LIBRARIES_DIR := libraries
LIBS_DIR := libs
LIBS_INC_DIR := libs/include

SERVER = $(SERVER_DIR)/$(SERVER_NAME)
CLIENT = $(CLIENT_DIR)/$(CLIENT_NAME)
CJSON = $(LIBRARIES_DIR)/$(LIBS_DIR)/$(CJSON_NAME).a
SQLITE = $(LIBRARIES_DIR)/$(LIBS_DIR)/$(SQLITE_NAME).a

# ----- commands -----
RM := rm -rdf
MKDIR = mkdir -p

# ------------------ body -------------------
all: $(CJSON_NAME) $(SQLITE_NAME) $(SERVER_NAME) $(CLIENT_NAME)

$(SERVER_NAME): $(SERVER)
$(CLIENT_NAME): $(CLIENT)
$(CJSON_NAME): $(CJSON)
$(SQLITE_NAME): $(SQLITE)

$(SERVER): $(SERVER_DIR) | $(SERVER_DIR)/$(LIBS_DIR)
$(CLIENT): $(CLIENT_DIR) | $(CLIENT_DIR)/$(LIBS_DIR)
$(CJSON): $(CJSON_DIR)
$(SQLITE): $(SQLITE_DIR)

$(SERVER):
	@cp $(CJSON) $(SERVER_DIR)/$(LIBS_DIR)
	@cp $(SQLITE) $(SERVER_DIR)/$(LIBS_DIR)
	@cp $(LIBRARIES_DIR)/$(LIBS_INC_DIR)/cJSON.h $(SERVER_DIR)/$(LIBS_INC_DIR)
	@cp $(LIBRARIES_DIR)/$(LIBS_INC_DIR)/sqlite3.h $(SERVER_DIR)/$(LIBS_INC_DIR)
	@$(MAKE) -sC $<
	@cp $@ .

 $(CLIENT):
	@cp $(CJSON) $(CLIENT_DIR)/$(LIBS_DIR)
	@cp $(LIBRARIES_DIR)/$(LIBS_INC_DIR)/cJSON.h $(CLIENT_DIR)/$(LIBS_INC_DIR)
	@$(MAKE) -sC $<
	@cp $@ .

$(SERVER_DIR)/$(LIBS_DIR) $(CLIENT_DIR)/$(LIBS_DIR):
	@$(MKDIR) $(SERVER_DIR)/$(LIBS_DIR) $(SERVER_DIR)/$(LIBS_INC_DIR)
	@$(MKDIR) $(CLIENT_DIR)/$(LIBS_DIR) $(CLIENT_DIR)/$(LIBS_INC_DIR)

$(CJSON) $(SQLITE):| $(LIBRARIES_DIR)/$(LIBS_DIR) $(LIBRARIES_DIR)/$(LIBS_INC_DIR)
	@$(MAKE) -sC $<

$(LIBRARIES_DIR)/$(LIBS_DIR) $(LIBRARIES_DIR)/$(LIBS_INC_DIR):
	@$(MKDIR) $(LIBRARIES_DIR)/$(LIBS_DIR) $(LIBRARIES_DIR)/$(LIBS_INC_DIR)

clean:
	@$(MAKE) clean -sC $(SERVER_DIR)
	@$(MAKE) clean -sC $(CLIENT_DIR)
	@$(MAKE) clean -sC $(CJSON_DIR)
	@$(MAKE) clean -sC $(SQLITE_DIR)
	@$(RM) $(LIBRARIES_DIR)/$(LIBS_DIR)

uninstall: clean
	@$(MAKE) uninstall -sC $(SERVER_DIR)
	@$(MAKE) uninstall -sC $(CLIENT_DIR)
	@$(MAKE) uninstall -sC $(CJSON_DIR)
	@$(MAKE) uninstall -sC $(SQLITE_DIR)
	@$(RM) $(SERVER_NAME)
	@$(RM) $(CLIENT_NAME)
	@$(RM) $(DB_NAME)

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
