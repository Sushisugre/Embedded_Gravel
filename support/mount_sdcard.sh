sudo losetup /dev/loop0 ~/workspace/embedded/support/sdcard.img
sudo kpartx -a /dev/loop0
sudo mount /dev/mapper/loop0p1 /media/bootfs
