#Dockerfile to build VLC 2.8.0 for Windows 64bits

FROM ubuntu:16.04

MAINTAINER Magnarox <thevinc@gmx.fr>

# Update
RUN dpkg --add-architecture i386
RUN apt-get update

# Install all building stuff
RUN apt-get install -y git
RUN apt-get install -y gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 mingw-w64-tools
RUN apt-get install -y libtool automake autoconf autopoint make gettext pkg-config
RUN apt-get install -y lua5.2 
RUN apt-get install -y lua5.2:i386 
RUN apt-get install -y qt4-dev-tools qt5-default
RUN apt-get install -y wine64-development-tools libwine-development-dev

# Get VLC source
RUN git clone https://github.com/videolan/vlc-2.2.git /home/vlc-2.2
RUN chmod -R 777 /home/vlc-2.2
WORKDIR "/home/vlc-2.2"
RUN git checkout 2.2.8

# Build contrib
RUN mkdir -p /home/vlc-2.2/contrib/win32
WORKDIR "/home/vlc-2.2/contrib/win32"
RUN ../bootstrap --host=x86_64-w64-mingw32
RUN sed -i -- 's/latest/20160219/g' Makefile
COPY files/vlc-contrib-x86_64-w64-mingw32-20160219.tar.bz2 .
RUN chmod 777 vlc-contrib-x86_64-w64-mingw32-20160219.tar.bz2
RUN make prebuilt
RUN ln -sf 'x86_64-w64-mingw32' ../i686-w64-mingw32

# Build vlc
WORKDIR "/home/vlc-2.2"
RUN ./bootstrap
RUN mkdir win32 && cd win32 && export PKG_CONFIG_LIBDIR=/home/vlc-2.2/contrib/x86_64-w64-mingw32/lib/pkgconfig && ../extras/package/win32/configure.sh --host=x86_64-w64-mingw32 --build=x86_64-pc-linux-gnu
WORKDIR "/home/vlc-2.2/win32"
RUN make

# Create volume to work on VLC
VOLUME /home/vlc-2.2/win32

CMD ["make", "package-win-common"]

