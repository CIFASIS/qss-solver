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
#  REQUIREMENTS: git, make, dpkg, fakeroot
#         NOTES: --- 
#        AUTHOR: Joaquin Fernandez, joaquin.f.fernandez@gmail.com
#       PROJECT: QSS Solver
#       VERSION: 4.5.3
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
readonly TMP_DEB_DIR="./tmp_deb"
readonly TMP_DIR="./tmp"

function setup_environment() {
  echo "Cleaning old packages..."
  rm -f qss-solver-*.deb

  echo "Retrieving latest from Git..."
  #git pull

  VER=$(cat "$DEPLOY_DIR/version")

  SYSTEM_VERSION=$(lsb_release -d)
  CONTROL_FILE="control.amd64"
  SBML_LIB="libsbml.so.5.18.0"
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
  cd "$SRC_DIR"
  make clean
  make
  cd - > /dev/null
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

  mkdir -p "$TMP_DEB_DIR/opt/qss-solver/bin/lib" \
           "$TMP_DEB_DIR/opt/qss-solver/src" \
           "$TMP_DEB_DIR/opt/qss-solver/build" \
           "$TMP_DEB_DIR/opt/qss-solver/output" \
           "$TMP_DEB_DIR/opt/qss-solver/lib"

  awk -v VERSION="$VER" '{ if(index($0,"Version:")>=1) print "Version: " VERSION ; else print $0;}' \
    "$TMP_DEB_DIR/DEBIAN/$CONTROL_FILE" > "$TMP_DEB_DIR/DEBIAN/control"
  rm "$TMP_DEB_DIR/DEBIAN/$CONTROL_FILE"

  cp "$DEPLOY_DIR/version" "$TMP_DEB_DIR/opt/qss-solver/"
  cp "$SRC_DIR/mmoc/usr/bin/mmoc" "$TMP_DEB_DIR/opt/qss-solver/bin/"
  cp "$SRC_DIR/gui/usr/bin/qss-solver" "$TMP_DEB_DIR/opt/qss-solver/bin/"
  cp "$SRC_DIR/interfaces/sbml/usr/bin/translate-sbml" "$TMP_DEB_DIR/opt/qss-solver/bin/"
  cp "$SRC_DIR/engine/3rd-party/partitioners/hmetis/khmetis" "$TMP_DEB_DIR/opt/qss-solver/bin/"
  cp "$DEPLOY_DIR/images/integrator.svg" "$TMP_DEB_DIR/opt/qss-solver/bin/"
  cp -r "$SRC_DIR/gui/3rd-party/qtermwidget-1-0.14.1/usr/config/"* "$TMP_DEB_DIR/opt/qss-solver/bin/"
  cp -r "$SRC_DIR/gui/3rd-party/qtermwidget-1-0.14.1/usr/lib/"* "$TMP_DEB_DIR/opt/qss-solver/bin/lib/"
  chmod 0755 $(find "$TMP_DEB_DIR/opt/qss-solver/bin" -type f)

  cp LICENSE INSTALL README.md CHANGELOG "$TMP_DEB_DIR/opt/qss-solver/"
  cp "$TMP_DIR/bin/"*.sh "$TMP_DEB_DIR/opt/qss-solver/bin/"
  cp -r "$TMP_DIR/doc" "$TMP_DEB_DIR/opt/qss-solver/"
  cp -r "$TMP_DIR/models" "$TMP_DEB_DIR/opt/qss-solver/"
  cp -r "$TMP_DIR/packages" "$TMP_DEB_DIR/opt/qss-solver/"
  cp -r "$TMP_DIR/src/engine" "$TMP_DEB_DIR/opt/qss-solver/src/"
  cp -r "$TMP_DIR/src/mmoc" "$TMP_DEB_DIR/opt/qss-solver/src/"
  cp -r "$TMP_DIR/src/gui" "$TMP_DEB_DIR/opt/qss-solver/src/"
  cp -r "$TMP_DIR/src/interfaces" "$TMP_DEB_DIR/opt/qss-solver/src/"
  cp -r "$TMP_DIR/src/usr" "$TMP_DEB_DIR/opt/qss-solver/src/"
  cp -r "$TMP_DIR/src/python" "$TMP_DEB_DIR/opt/qss-solver/src/"
  cp lib/*.a "$TMP_DEB_DIR/opt/qss-solver/lib"

  # Clean generated code.
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/engine/3rd-party"
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/engine/usr/obj"
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/mmoc/usr/obj"
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/mmoc/usr/bin"
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/mmoc/usr/share"
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/mmoc/usr/lib"
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/mmoc/usr/libexec"
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/usr/src/"*.o
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/usr/lib"
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/interfaces/sbml/usr"
  rm -rf "$TMP_DEB_DIR/opt/qss-solver/src/gui/usr"

  cp "/usr/lib/x86_64-linux-gnu/$SBML_LIB" "$TMP_DEB_DIR/opt/qss-solver/lib/libsbml.so.5"
  cp "$SRC_DIR/engine/3rd-party/partitioners/patoh/Linux-x86_64/libpatoh.a" "$TMP_DEB_DIR/opt/qss-solver/lib/libpatoh.a"
  cp "$SRC_DIR/engine/3rd-party/partitioners/metis/Linux-x86_64/libmetis.a" "$TMP_DEB_DIR/opt/qss-solver/lib/libmetis.a"
}

function set_permissions() {
  echo "Setting file permissions..."

  for ext in cpp c h ini png tex; do
    find "$TMP_DEB_DIR" -iname "*.$ext" -exec chmod 0644 {} +
  done

  find "$TMP_DEB_DIR/opt/qss-solver/doc" -type f -exec chmod 0644 {} +
  find "$TMP_DEB_DIR/opt/qss-solver/src" -type f -exec chmod 0644 {} +
  find "$TMP_DEB_DIR/opt/qss-solver/packages" -type f -exec chmod 0644 {} +
  find "$TMP_DEB_DIR/opt/qss-solver/src/usr" -type f -exec chmod 0644 {} +

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
