#!/bin/bash

# Get sources
git clone https://github.com/videolan/vlc-2.2.git /home/vlc-2.2
cd /home/vlc-2.2
git checkout 2.2.8
# Build contrib
mkdir -p /home/vlc-2.2/contrib/win32
cd /home/vlc-2.2/contrib/win32
../bootstrap --host=x86_64-w64-mingw32
sed -i -- 's/latest/20160219/g' Makefile
mv /vlc-contrib-x86_64-w64-mingw32-20160219.tar.bz2 /home/vlc-2.2/contrib/win32/vlc-contrib-x86_64-w64-mingw32-20160219.tar.bz2
chmod +x vlc-contrib-x86_64-w64-mingw32-20160219.tar.bz2
make prebuilt
ln -sf 'x86_64-w64-mingw32' ../i686-w64-mingw32
# Bootstrap & configure
cd /home/vlc-2.2
./bootstrap
mkdir win32 && cd win32
export PKG_CONFIG_LIBDIR=/home/vlc-2.2/contrib/x86_64-w64-mingw32/lib/pkgconfig
../extras/package/win32/configure.sh --host=x86_64-w64-mingw32 --build=x86_64-pc-linux-gnu


