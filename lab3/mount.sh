#! /bin/sh
echo "making"
make
echo "mounting mmc1"
# mount /mnt/mmc1
# cp kernel/kernel.bin /mnt/mmc1
# cp tasks/bin/*.bin /mnt/mmc1
# umount /mnt/mmc1

MOUNT_FOLDER = /media/bootfs

sudo losetup /dev/loop1 $SUPPORT_FOLDER/sdcard.img
sudo kpartx -a /dev/loop1
sudo mount /dev/mapper/loop1p1 $MOUNT_FOLDER

if [ -z "$var" ];
then
    sudo cp kernel/kernel.bin $MOUNT_FOLDER
    sudo cp tasks/bin/*.bin $MOUNT_FOLDER
else
    for var
    do
     sudo cp $var $MOUNT_FOLDER
    done
fi
sync;sync;sync
sudo umount /media/bootfs
sudo kpartx -d /dev/loop1
sudo losetup -d /dev/loop1

echo "done mounting"

