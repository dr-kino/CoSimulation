################################################################################
bin_PROGRAMS = Test_PipeBus

# Test_PipeBus application
Test_PipeBus_SOURCES = \
	test_pipeBus.c \
	../src/pipeBus.c \
	../../../sw/src/sanity.c \
 	../../../sw/src/log.c

Test_PipeBus_CFLAGS = ${USERAPP_CFLAGS} ${DBGFLAGS} \
	-I../inc \
	-I../../../sw/inc

Test_PipeBus_LDFLAGS = ${USERAPP_LDFLAGS} ${DBGFLAGS}

Test_PipeBus_LDADD =-lm -lreadline -lncurses
