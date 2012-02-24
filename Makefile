# vim: ts=8 sw=8

PREFIX	:=/opt
BINDIR	=${PREFIX}/bin

INSTALL	=install

CC	=ccache gcc -mtune=native -std=gnu99
CFLAGS	=-Os -Wall -Werror -pedantic -pipe -g
LDFLAGS	=-g

all::	cr

clean::
	${RM} *.o a.out core.* lint tags

distclean clobber:: clean
	${RM} cr

install:: cr
	${INSTALL} -D -s cr ${BINDIR}/cr

uninstall::
	${RM} ${BINDIR}/cr

check::	cr
	./cr ${ARGS} Makefile
