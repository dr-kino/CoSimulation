# Makefile
SHELL := /bin/bash

all: synthesize
	@echo "Dummy make entry..."

synthesize:
	source /usr/share/Xilinx/14.5/ISE_DS/settings64.sh
	@make -C hw synthesize
