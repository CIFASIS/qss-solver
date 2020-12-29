#!/bin/bash

sudo apt-get update

echo "Installing QSS Solver Dependencies"
sudo apt install -y gnuplot-x11
sudo apt install -y build-essential
sudo apt install -y libc6
sudo apt install -y qt5-default
sudo apt install -y libqt5svg5
sudo apt install -y zenity
sudo apt install -y libhdf5-dev

# Needed to avoid linking error, see: https://github.com/YosysHQ/nextpnr/issues/444#issuecomment-631558250
sudo strip --remove-section=.note.ABI-tag /usr/lib/x86_64-linux-gnu/libQt5Core.so.5

echo "Installing QSS Solver"
sudo apt install -y ./scripts/qsssolver.deb

# Packages needed to open user folder browser.
sudo apt install -y konqueror
sudo apt install -y nautilus

sudo useradd -m -s /bin/bash -G sudo,dialout -p $(openssl passwd qsssolver) qsssolver
