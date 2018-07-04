#Dockerfile to build VLC 2.2.8 for Windows 64bits

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

# Create volume to work on VLC
VOLUME /home/vlc-2.2

# Add Prebuilt package
COPY files/vlc-contrib-x86_64-w64-mingw32-20160219.tar.bz2 .

# Add scripts
COPY files/init_vlc_repo.sh .
COPY files/build_vlc.sh .
COPY files/package_vlc.sh .
RUN chmod +x init_vlc_repo.sh && chmod +x build_vlc.sh && chmod +x package_vlc.sh

# Shell
CMD ["/bin/bash"]

# Build dockerfile :
# docker build <image_name> .
# Start container :
# docker run -v <local_dir>:/home/vlc-2.2 -it <image_name>
#only the first time : ./init_vlc_repo.sh
#build vlc : ./build_vlc.sh
#package vlc: ./package_vlc.sh





