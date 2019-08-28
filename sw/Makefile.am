################################################################################
bin_PROGRAMS = CoSimulation

# test0 application
CoSimulation_SOURCES = \
	src/gen_cmd.c \
	src/pipe.c 

CoSimulation_CFLAGS = ${USERAPP_CFLAGS} ${DBGFLAGS} \
	-Iinc

CoSimulation_LDFLAGS = ${USERAPP_LDFLAGS} ${DBGFLAGS}

CoSimulation_LDADD =-lm -lreadline -lncurses

