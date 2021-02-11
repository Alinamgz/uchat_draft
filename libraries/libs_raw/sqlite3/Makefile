# ======== variables ========
NAME = sqlite3

# -- files and dirs --
DST_INC_DIR = ../../libs/include
DST_LIB_DIR = ../../libs

INC_FILE = $(NAME).h
SRC_FILE = $(NAME).c
OBJ_FILE = $(NAME).o

STATIC_LIB = $(DST_LIB_DIR)/libsqlite3.a

# -- commands --
RM = rm -rdf
MKDIR = mkdir -p

CC = clang -std=c89
# WHY do we use this flag???
SQL_FLAGS = -DSQLITE_ENABLE_UPDATE_DELETE_LIMIT=1

# ======== body ========

all: static

static: $(STATIC_LIB)

.c.o:
	@$(CC) -c $(SQL_FLAGS) $<
	@printf "$(notdir $@)\033[32;1m created\033[0m\n"

$(STATIC_LIB): $(OBJ_FILE)
	@$(MKDIR) $(DST_LIB_DIR) $(DST_INC_DIR)
	@cp $(INC_FILE) $(DST_INC_DIR)/$(INC_FILE)
	@$(AR) rcs $@ $<
	@printf "$(notdir $@)\033[32;1m created\033[0m\n"

$(OBJ_FILE): $(SRC_FILE) $(INC_FILE)

uninstall: clean
	@$(RM) $(STATIC_LIB)
	@printf "$(notdir $(STATIC_LIB))\033[31;1m deleted\033[0m\n"

clean:
	@$(RM) $(OBJ_FILE) $(DST_INC_DIR)/$(INC_FILE)

reinstall: uninstall all

.PHONY: all static clean uninstall reinstall
