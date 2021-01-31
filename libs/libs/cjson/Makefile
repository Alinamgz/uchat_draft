CJSON_OBJ = cJSON.o
CJSON_LIBNAME = libcjson

CC = clang -std=c89

PIC_FLAGS = -fPIC
R_CFLAGS = $(PIC_FLAGS) -pedantic -Wall -Werror -Wstrict-prototypes -Wwrite-strings -Wshadow -Winit-self -Wcast-align -Wformat=2 -Wmissing-prototypes -Wstrict-overflow=2 -Wcast-qual -Wc++-compat -Wundef -Wswitch-default -Wconversion

#library file extensions
STATIC = a

CJSON_STATIC = $(CJSON_LIBNAME).$(STATIC)

.PHONY: all static clean uninstall reinstall

all: static

static: $(CJSON_STATIC)

.c.o:
	@$(CC) -c $(R_CFLAGS) $<
	@printf "$(notdir $@)\033[32;1m created\033[0m\n"

$(CJSON_STATIC): $(CJSON_OBJ)
	@$(AR) rcs $@ $<
	@printf "$(notdir $@)\033[32;1m created\033[0m\n"


$(CJSON_OBJ): cJSON.c cJSON.h

uninstall: clean
	@rm -rf $(CJSON_STATIC)
	@printf "$(notdir $(CJSON_STATIC))\033[31;1m deleted\033[0m\n"

clean:
	@rm -rf $(CJSON_OBJ)
	@printf "$(notdir $(CJSON_OBJ))\033[31;1m deleted\033[0m\n"

reinstall: uninstall all
