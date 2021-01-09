# ---------------- variables ----------------
SERVER := uchat_server
CLIENT := uchat

# -- files and dirs --
SERVER_DIR := server
CLIENT_DIR := client

# ----- commands -----
RM := rm -rdf

# ------------------ body -------------------
all: $(SERVER) $(CLIENT)

$(SERVER):
	@$(MAKE) -sC $(SERVER_DIR)
	@mv $(SERVER_DIR)/$(SERVER) .

$(CLIENT):
	@$(MAKE) -sC $(CLIENT_DIR)
	@mv $(CLIENT_DIR)/$(CLIENT) .

clean:
	@$(MAKE) clean -C $(SERVER_DIR)
	@$(MAKE) clean -C $(CLIENT_DIR)

uninstall: clean
	@$(RM) $(SERVER)
	@$(RM) $(CLIENT)

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
