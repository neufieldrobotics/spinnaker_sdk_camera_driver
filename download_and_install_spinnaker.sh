#!/usr/bin/env bash
set -x

CWD=`pwd`

export DEBIAN_FRONTEND=noninteractive
export ROS_DISTRO kinetic
export SPINNAKER_VERSION=2.0.0.147
export SPINNAKER_LINUX_ARCH=amd64

# install basic packages
apt-get update
apt-get install -q -y --no-install-recommends \
    build-essential tree wget dirmngr gnupg2 vim nano git debconf-utils libunwind-dev

wget https://www.dl.dropboxusercontent.com/s/xq5f6r15i4rea4w/spinnaker-$SPINNAKER_VERSION-amd64-pkg.tar.gz

tar -zxvf spinnaker-$SPINNAKER_VERSION-$SPINNAKER_LINUX_ARCH-pkg.tar.gz
#rm spinnaker-$SPINNAKER_VERSION-$SPINNAKER_LINUX_ARCH-pkg.tar.gz
cd spinnaker-$SPINNAKER_VERSION-$SPINNAKER_LINUX_ARCH

echo libspinnaker libspinnaker/present-flir-eula note '' | debconf-set-selections
echo libspinnaker libspinnaker/accepted-flir-eula boolean true | debconf-set-selections

dpkg -i   libspinnaker_*.deb
dpkg -i   libspinnaker-dev_*.deb
dpkg -i   libspinnaker-c_*.deb
dpkg -i   libspinnaker-c-dev_*.deb
dpkg -i   libspinvideo_*.deb
dpkg -i   libspinvideo-dev_*.de
dpkg -i   libspinvideo-c_*.deb
dpkg -i   libspinvideo-c-dev_*.deb
dpkg -i   spinview-qt_*.deb
dpkg -i   spinview-qt-dev_*.deb
dpkg -i   spinupdate_*.deb
dpkg -i   spinupdate-dev_*.deb
dpkg -i   spinnaker_*.deb
dpkg -i   spinnaker-doc_*.deb

cd ..
#rm -r spinnaker-$SPINNAKER_VERSION-$SPINNAKER_LINUX_ARCH/

