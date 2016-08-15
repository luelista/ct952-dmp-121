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
mkdir STB
cd osdgame
mkdir bmp
cd ..
cd STB
mkdir bmp
mkdir ct_objs
mkdir objs
mkdir OSDString
mkdir Overlap_Files
mkdir pobjs
mkdir database
mkdir device
mkdir include
mkdir nim
mkdir plug_in
cd plug_in
mkdir bmp
mkdir lunar
cd..
mkdir usrif
cd usrif
mkdir config
mkdir font0
mkdir logo
cd....
copy 950_Files\. release\950_Files\. /S
copy BMP\. release\BMP\. /S
copy OSDString\. release\OSDString\. /S
copy osdgame\bmp\. release\osdgame\bmp\. /S
copy servo\SRVUDEIF.H release\servo\.
copy servo\makefile release\servo\.
copy vd\. release\vd\.
copy usb\. release\usb\. /S
copy STB\. release\STB\.
copy STB\bmp\. release\STB\bmp\.
copy STB\database\. release\STB\database\.
copy STB\device\. release\STB\device\.
copy STB\include\. release\STB\include\.
copy STB\pobjs\. release\STB\pobjs\.
copy STB\OSDString\. release\STB\OSDString\. /S
copy STB\Overlap_Files\. release\STB\Overlap_Files\.
copy STB\usrif\. release\STB\usrif\.
copy STB\usrif\config\. release\STB\usrif\config\.
copy STB\usrif\font0\. release\STB\usrif\font0\.
copy STB\usrif\logo\. release\STB\usrif\logo\.
del release\STB\database\*.o
del release\STB\device\*.o
del release\STB\usrif\*.o
echo on
