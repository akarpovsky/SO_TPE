
# For using this script you should first config mtools.
# Edit the file /etc/mtools.conf and comment al the drives
# except drive B. Insert the correct path to the tpe.img

./compila
cd ./bin
mcopy kernel.bin b:boot/ -o


