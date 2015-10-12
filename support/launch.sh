SUPPORT_FOLDER=~/workspace/embedded/support
qemu-system-arm -nographic -M verdex -pflash $SUPPORT_FOLDER/flash.img -sd $SUPPORT_FOLDER/sdcard.img
