echo off
echo Build a [RELEASE] version and will make a new directory "release" in current directory.
make
mkdir release
copy *.* release
cd release
rm atapi.c
rm makefile
rm address.txt
rm DVD909.tsim
rm DVD909_tmp.tsim
rm DVD909_flash.tsim
copy makefile_rel makefile
copy address_rel.txt address.txt
mkdir BMP
mkdir OBJS
mkdir OSDString
mkdir osdgame
mkdir servo
mkdir vd
mkdir usb
cd osdgame
mkdir bmp
cd ...
copy BMP\. release\BMP\. /S
copy OSDString\. release\OSDString\. /S
copy osdgame\bmp\. release\osdgame\bmp\. /S
copy servo\SRVUDEIF.H release\servo\.
copy servo\makefile release\servo\.
copy vd\. release\vd\. /S
copy usb\. release\usb\. /S
del release\OSDString\unicode /S /X /Y
echo on
