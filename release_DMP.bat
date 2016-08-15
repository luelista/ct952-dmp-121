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
mkdir 950_Files
mkdir BMP
mkdir OBJS
mkdir OSDString
mkdir osdgame
mkdir servo
mkdir vd
cd osdgame
mkdir bmp
cd ...
copy 950_Files\. release\950_Files\. /S
copy BMP\. release\BMP\. /S
copy OSDString\. release\OSDString\. /S
copy osdgame\bmp\. release\osdgame\bmp\. /S
copy servo\SRVUDEIF.H release\servo\.
copy servo\makefile release\servo\.
copy vd\. release\vd\.
copy usb\. release\usb\. /S
echo on
