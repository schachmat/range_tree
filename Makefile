# See LICENSE file for copyright and license details.

include config.mk

SRC = range_tree.c range_list.c test.c
OBJ = ${SRC:.c=.o}

all: options test

options:
	@echo range_tree build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

test: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f ${OBJ}

.PHONY: all options clean
