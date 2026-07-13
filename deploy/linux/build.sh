#!/bin/bash
#===================================================================================
#
# 				 FILE: build.sh  
#
# 				USAGE: build.sh 
#
# 	DESCRIPTION: Build the Linux deb package including the QSS Solver engine, 
# 							 the QSS Solver GUI, the MicroModelica C Compiler with the 
# 						   corresponding user libraries and the SBML translator tool.
#
#    PARAMETERS: ---
#       OPTIONS: ---
#  REQUIREMENTS: ---
#         NOTES: --- 
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 6.1.1
#===================================================================================

set -euo pipefail

# Check required commands
for cmd in git make dpkg fakeroot; do
  if ! command -v "$cmd" &> /dev/null; then
    echo "Error: Required command '$cmd' not found. Please install it." >&2
    exit 1
  fi
done

readonly HOME_DIR="../../"
readonly DEPLOY_DIR="./deploy/linux"
readonly SRC_DIR="./src"
readonly BIN_DIR="./bin"
readonly BUILD_DIR="$SRC_DIR/build"
readonly TMP_DEB_DIR="./tmp_deb"
readonly TMP_DIR="./tmp"
readonly TMP_INSTALL_DIR="$TMP_DEB_DIR/opt/CIFASIS-CONICET/qss-solver"

function setup_environment() {
  echo "Cleaning old packages..."
  rm -f qss-solver-*.deb

  echo "Retrieving latest from Git..."
  git pull

  VER=$(cat "$DEPLOY_DIR/version")

  SYSTEM_VERSION=$(lsb_release -d)
  CONTROL_FILE="control.amd64"
  SBML_LIB="libsbml.so.5.20.2"
  PACKAGE_NAME="qss-solver-$VER"

  if [[ "$SYSTEM_VERSION" == *"22.04"* ]]; then
    CONTROL_FILE="control.amd64.u22"
    SBML_LIB="libsbml.so.5.19.0"
    PACKAGE_NAME="${PACKAGE_NAME}-u22"
  fi

  BRANCH=$(git rev-parse --abbrev-ref HEAD)
  if [[ "$BRANCH" != "qss-solver-release" ]]; then
    PACKAGE_NAME="${PACKAGE_NAME}-unstable"
  fi

  PACKAGE_NAME="${PACKAGE_NAME}.deb"

  echo "Building QSS Solver DEB package for $(uname -m) version $VER"

  # Export variables for use in other functions
  export VER PACKAGE_NAME CONTROL_FILE SBML_LIB BRANCH
}

function build_binaries() {
  echo "Building binaries..."
  mkdir -p "$BUILD_DIR"
  cd "$BUILD_DIR"
  cmake ..
  make clean
  make reset-targets
  make
  make install
  cd "$HOME_DIR"
}

function prepare_package() {
  echo "Preparing package directory structure..."

  rm -rf "$TMP_DEB_DIR" "$TMP_DIR"
  mkdir -p "$TMP_DEB_DIR" "$TMP_DIR"

  CHECKOUT_PATH="${MMOC_PATH}${TMP_DIR:1}/"
  mkdir -p "$CHECKOUT_PATH"
  git checkout-index -a -f --prefix="$CHECKOUT_PATH"

  cp -r "$TMP_DIR/deploy/linux/deb/"* "$TMP_DEB_DIR/"
  chmod 0755 "$TMP_DEB_DIR/DEBIAN/post"*

  mkdir -p "$TMP_INSTALL_DIR/bin/lib" \
           "$TMP_INSTALL_DIR/src" \
           "$TMP_INSTALL_DIR/build" \
           "$TMP_INSTALL_DIR/output" \
           "$TMP_INSTALL_DIR/lib"

  awk -v VERSION="$VER" '{ if(index($0,"Version:")>=1) print "Version: " VERSION ; else print $0;}' \
    "$TMP_DEB_DIR/DEBIAN/$CONTROL_FILE" > "$TMP_DEB_DIR/DEBIAN/control"
  rm "$TMP_DEB_DIR/DEBIAN/$CONTROL_FILE"

  cp "$DEPLOY_DIR/version" "$TMP_INSTALL_DIR/"
  cp "$BIN_DIR/mmoc" "$TMP_INSTALL_DIR/bin/"
  cp "$BIN_DIR/qss-solver" "$TMP_INSTALL_DIR/bin/"
  cp "$BIN_DIR/translate-sbml" "$TMP_INSTALL_DIR/bin/"
  cp "$SRC_DIR/engine/3rd-party/partitioners/hmetis/khmetis" "$TMP_INSTALL_DIR/bin/"
  cp "./deploy/images/integrator.svg" "$TMP_INSTALL_DIR/bin/"
  cp -r "$SRC_DIR/gui/3rd-party/qtermwidget-1-0.14.1/usr/config/"* "$TMP_INSTALL_DIR/bin/"
  cp -r "$SRC_DIR/gui/3rd-party/qtermwidget-1-0.14.1/usr/lib/"* "$TMP_INSTALL_DIR/bin/lib/"
  chmod 0755 $(find "$TMP_INSTALL_DIR/bin" -type f)

  cp LICENSE INSTALL README.md CHANGELOG "$TMP_INSTALL_DIR/"
  cp "$TMP_DIR/bin/"*.sh "$TMP_INSTALL_DIR/bin/"
  cp "$TMP_DIR/bin/"requirements.txt "$TMP_INSTALL_DIR/bin/"
  cp "$TMP_DIR/bin/"plot_data.py "$TMP_INSTALL_DIR/bin/"
  cp -r "$TMP_DIR/doc" "$TMP_INSTALL_DIR/"
  cp -r "$TMP_DIR/models" "$TMP_INSTALL_DIR/"
  cp -r "$TMP_DIR/packages" "$TMP_INSTALL_DIR/"
  cp -r "$TMP_DIR/src/engine" "$TMP_INSTALL_DIR/src/"
  cp -r "$TMP_DIR/src/mmoc" "$TMP_INSTALL_DIR/src/"
  cp -r "$TMP_DIR/src/gui" "$TMP_INSTALL_DIR/src/"
  cp -r "$TMP_DIR/src/interfaces" "$TMP_INSTALL_DIR/src/"
  cp -r "$TMP_DIR/src/usr" "$TMP_INSTALL_DIR/src/"
  cp -r "$TMP_DIR/src/python" "$TMP_INSTALL_DIR/src/"
  cp lib/*.a "$TMP_INSTALL_DIR/lib"

  cp "/usr/lib/x86_64-linux-gnu/$SBML_LIB" "$TMP_INSTALL_DIR/lib/libsbml.so.5"
  cp "$SRC_DIR/engine/3rd-party/partitioners/patoh/Linux-x86_64/libpatoh.a" "$TMP_INSTALL_DIR/lib/libpatoh.a"
  cp "$SRC_DIR/engine/3rd-party/partitioners/metis/Linux-x86_64/libmetis.a" "$TMP_INSTALL_DIR/lib/libmetis.a"
}

function set_permissions() {
  echo "Setting file permissions..."

  for ext in cpp c h ini png tex; do
    find "$TMP_DEB_DIR" -iname "*.$ext" -exec chmod 0644 {} +
  done

  find "$TMP_INSTALL_DIR/doc" -type f -exec chmod 0644 {} +
  find "$TMP_INSTALL_DIR/src" -type f -exec chmod 0644 {} +
  find "$TMP_INSTALL_DIR/packages" -type f -exec chmod 0644 {} +
  find "$TMP_INSTALL_DIR/src/usr" -type f -exec chmod 0644 {} +

  find "$TMP_DEB_DIR" -type d -exec chmod 0755 {} +
}

function build_package() {
  echo "Building deb package..."
  fakeroot dpkg -b "$TMP_DEB_DIR" qss-solver.deb
  mv qss-solver.deb "$DEPLOY_DIR/$PACKAGE_NAME"
}

function cleanup() {
  echo "Cleaning up temporary files..."
  rm -rf "$TMP_DEB_DIR" "$TMP_DIR"
}

function main() {
  cd "$HOME_DIR"
  setup_environment
  build_binaries
  prepare_package
  set_permissions
  build_package
  cleanup
  cd "$DEPLOY_DIR"
}

main
