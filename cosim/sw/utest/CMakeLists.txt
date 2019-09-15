#    Filename : CmakeLists.txt
# Description : Definition of all source and header files of one package.
#               To add this package to a project, include this file in the corresponding CMakeLists.txt file.
#      Author :
#     Release :
#   Copyright : 
#    Revision : 
# ------------------------------------------------------------------------------------------------
#     History : 
# ================================================================================================
cmake_minimum_required (VERSION 2.6)

set(this_package      TestPipeBus)                    		 # package prefix
set(this_package_file ${CMAKE_CURRENT_LIST_FILE})    # package cmk file
set(this_package_dir  ${CMAKE_CURRENT_LIST_DIR}/../../../)  # package root directory
# this_target_dir     (set in CMakeLists.txt)        # (sub-)project root directory
# this_target_shortname (set in common.cmake)        # last part of current directory (e.g. 'legacy')

################################################################################
# List here all used files of the package:

# only static files
add_executable(TestPipeBus
	${this_package_dir}/cosim/sw/utest/test_pipeBus.c
	${this_package_dir}/cosim/sw/src/pipeBus.c

	${this_package_dir}/sw/inc/sanity.h
	${this_package_dir}/sw/src/sanity.c
	${this_package_dir}/sw/src/log.c
)

include_directories(
	${this_package_dir}/cosim/sw/inc/
	${this_package_dir}/sw/inc/
)

################################################################################
