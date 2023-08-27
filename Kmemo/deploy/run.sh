qemu-system-x86_64 \
  -s \
  -m 300M \
  -kernel ./bzImage \
  -initrd ./rootfs.cpio \
  -nographic \
  -monitor /dev/null \
  -no-reboot \
  -append "kaslr root=/dev/ram rw rdinit=/init console=ttyS0 pti=off nosmep nosmap loglevel=3 oops=panic panic=1"

