################################################################################
###  Author: Doctor Kino  ######################################################
################################################################################

AC_PREREQ(2.61)
AC_INIT([Test_PipeBus], [1.0], [doctorkinoo@gmail.com])
AC_CONFIG_SRCDIR([test_pipeBus.c])
AC_SUBST([SRCROOT],[`pwd`])
AC_CONFIG_AUX_DIR([./dist])
AM_INIT_AUTOMAKE([subdir-objects foreign -Wall -Werror])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])
AC_SUBST([OPTFLAGS],[" -O3 -fomit-frame-pointer -ffast-math -fstrength-reduce \
-fexpensive-optimizations -fschedule-insns -fschedule-insns2 \
-funroll-loops -funroll-all-loops"])
AC_SUBST([CFLAGS],[])
AC_SUBST([CPPFLAGS],[])
AC_SUBST([DBGFLAGS],[])
CXXFLAGS=""
CFLAGS=""

###  flags  ####################################################################

USERAPP_LDFLAGS=" -Wall -Wextra -rdynamic"
USERAPP_CFLAGS=" -Wall -Wextra -rdynamic -D__LINUX__"
AC_SUBST([USERAPP_LDFLAGS])
AC_SUBST([USERAPP_CFLAGS])

### defines ####################################################################

AC_DEFINE([__LINUX__], [], [Compilar o código para LINUX.])
AC_DEFINE_UNQUOTED([__SRCROOT__], ["${SRCROOT}/"], [Pasta raiz do projeto.])

################################################################################
# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT

################################################################################

#echo
#echo "USERAPP_CFLAGS = ${USERAPP_CFLAGS}"
#echo
#echo "USERAPP_LDFLAGS = ${USERAPP_LDFLAGS}"
#echo
#echo "CFLAGS = ${CFLAGS}"
#echo
#echo "CXXFLAGS = ${CXXFLAGS}"
#echo
#echo "CPPFLAGS = ${CPPFLAGS}"

################################################################################

