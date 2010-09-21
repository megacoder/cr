# vim: ts=8 sw=8

PREFIX	:=/opt/cr
BINDIR	=${PREFIX}/bin

CC	=ccache gcc -march=i686
CFLAGS	=-Os -Wall -Werror -pedantic -pipe -g
LDFLAGS	=-g

all::	cr

clean::
	${RM} *.o a.out core.* lint tags

distclean clobber:: clean
	${RM} cr

install:: cr
	install -d ${BINDIR}
	install -c -s cr ${BINDIR}/

uninstall::
	${RM} ${BINDIR}/cr

check::	cr
	./cr ${ARGS} Makefile
