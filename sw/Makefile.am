################################################################################
bin_PROGRAMS = CoSimulation

# test0 application
CoSimulation_SOURCES = \
	src/gen_cmd.c \
	src/pipe.c \
	src/log.c \
	src/sanity.c \
	../cosim/sw/src/pipeBus.c

CoSimulation_CFLAGS = ${USERAPP_CFLAGS} ${DBGFLAGS} \
	-Iinc \
	-I../cosim/sw/inc

CoSimulation_LDFLAGS = ${USERAPP_LDFLAGS} ${DBGFLAGS}

CoSimulation_LDADD =-lm -lreadline -lncurses

