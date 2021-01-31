# SQLITE_OBJ = sqlite3.o
# SQLITE_LIBNAME = libsqlite3

# CC = clang -std=c89
# # TODO: Do we need this flag???
# # SQL_FLAGS = -DSQLITE_ENABLE_UPDATE_DELETE_LIMIT=1

# STATIC = a

# SQLITE_STATIC = ../../$(SQLITE_LIBNAME).$(STATIC)

# .PHONY: all static clean uninstall reinstall

# all: static

# static: $(SQLITE_STATIC)

# .c.o:
# 	@$(CC) -c $<
# 	@printf "$(notdir $@)\033[32;1m created\033[0m\n"

# $(SQLITE_STATIC): $(SQLITE_OBJ)
# 	@cp sqlite3.h ../../include/
# 	@$(AR) rcs $@ $<
# 	@printf "$(notdir $@)\033[32;1m created\033[0m\n"


# $(SQLITE_OBJ): sqlite3.c sqlite3.h

# uninstall: clean
# 	@rm -rf $(SQLITE_STATIC)
# 	@printf "$(notdir $(SQLITE_STATIC))\033[31;1m deleted\033[0m\n"

# clean:
# 	@rm -rf $(SQLITE_OBJ)
# 	@printf "$(notdir $(SQLITE_OBJ))\033[31;1m deleted\033[0m\n"

# reinstall: uninstall all

SQLITE_OBJ = sqlite3.o
SQLITE_LIBNAME = libsqlite3

CC = gcc -std=c89

#library file extensions
STATIC = a

SQLITE_STATIC = ../../libs/$(SQLITE_LIBNAME).$(STATIC)

.PHONY: all static clean uninstall reinstall

all: static

static: $(SQLITE_STATIC)

$(SQLITE_STATIC): $(SQLITE_OBJ)
	@cp sqlite3.h ../../include/
	@$(AR) rcs $@ $<
	@printf "\033[32;1m$(notdir $@) created\033[0m\n"

$(SQLITE_OBJ): sqlite3.c sqlite3.h
	@$(CC) -DSQLITE_ENABLE_UPDATE_DELETE_LIMIT=1 -c $<

uninstall: clean
	@rm -rf $(SQLITE_STATIC)

clean:
	@rm -rf $(SQLITE_OBJ)
	@rm -rf ../../include/sqlite3.h
reinstall: uninstall all
