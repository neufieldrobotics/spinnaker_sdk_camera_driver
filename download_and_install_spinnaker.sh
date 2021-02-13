#!/usr/bin/env bash
set -x

CWD=`pwd`

export DEBIAN_FRONTEND=noninteractive

# install basic packages
apt-get update
apt-get install -q -y --no-install-recommends \
    build-essential tree wget dirmngr gnupg2 vim nano git debconf-utils libunwind-dev iputils-ping dialog apt-utils

wget https://coe.northeastern.edu/fieldrobotics/spinnaker_sdk_archive/spinnaker-$SPINNAKER_VERSION-$SPINNAKER_LINUX_ARCH-pkg.tar.gz -nv

tar -zxvf spinnaker-$SPINNAKER_VERSION-$SPINNAKER_LINUX_ARCH-pkg.tar.gz
cd spinnaker-$SPINNAKER_VERSION-$SPINNAKER_LINUX_ARCH

# fix issue with 'logname' command on docker, required by spinnaker 2.2.0.48
mv /usr/bin/logname /usr/bin/logname_old
echo "echo root" > /usr/bin/logname
chmod +x /usr/bin/logname

# auto accept spinnaker license agreements
echo libspinnaker libspinnaker/present-flir-eula note '' | debconf-set-selections
echo libspinnaker libspinnaker/accepted-flir-eula boolean true | debconf-set-selections

dpkg -i   *.deb

cd ..

