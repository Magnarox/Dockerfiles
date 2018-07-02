# Dockerfile vlc-2.2 for Windows 64bits
1. Build docker image
2. Run the new image with -v (local):/home/vlc-2.2/win32 and -it options
3. In the shell launch :
./bootstrap

cd win32 && export PKG_CONFIG_LIBDIR=/home/vlc-2.2/contrib/x86_64-w64-mingw32/lib/pkgconfig && ../extras/package/win32/configure.sh --host=x86_64-w64-mingw32 --build=x86_64-pc-linux-gnu

make

make package-win-common

4. VLC 2.2.8 for Wondows 64bits in (local)/vlc-2.2.8
