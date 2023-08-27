#!/bin/bash

make clean && \
make && \
mv kmemo.ko ./rootfs && \
cd rootfs && \
./gen_cpio.sh
