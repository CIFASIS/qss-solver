#!/bin/bash

sudo apt-get update

sudo apt-get install gnuplot-x11

echo "Installing QSS Solver Dependencies"

# TODO: install dependencies.
# sudo apt-get install 

echo "Installing QSS Solver"
sudo dpkg -i -y qss-solver.den

# Packages needed to open user folder browser.
sudo apt-get install -y konqueror
sudo apt-get install -y nautilus

sudo useradd -m -s /bin/bash -G sudo,dialout -p $(openssl passwd qsssolver) qsssolver

