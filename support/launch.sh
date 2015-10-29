# Set SUPPORT_FOLDER in environment variable 
# SUPPORT_FOLDER=~/workspace/embedded/support
qemu-system-arm -nographic -M verdex -pflash $SUPPORT_FOLDER/flash.img -sd $SUPPORT_FOLDER/sdcard.img -s
