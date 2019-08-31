#!/bin/bash

autoreconf -fi
./configure

while [[ ! -e Makefile ]] ; do
    sleep 1
    echo "Creating Makefile.."
done

make clean all
