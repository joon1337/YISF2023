#!/bin/bash

gcc -o solve solve.c -static -no-pie && \
mv solve ./rootfs && \
make clean && \
make && \
mv sample.ko ./rootfs && \
cd rootfs && \
./gen_cpio.sh
