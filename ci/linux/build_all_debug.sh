# Any commands which fail will cause the shell script to exit immediately
set -e

# Set PRODUCT_SOURCE_DIR root directory
if [ "$PRODUCT_SOURCE_DIR" = "" ]; then
  RESTORE_DIRECTORY="$PWD"
  cd "$(dirname "$0")"
  cd ../..
  export PRODUCT_SOURCE_DIR="$PWD"
  echo "PRODUCT_SOURCE_DIR set to '$PRODUCT_SOURCE_DIR'."
  cd "$RESTORE_DIRECTORY"
  unset RESTORE_DIRECTORY
fi

# Set debug or release build type
export PRODUCT_BUILD_TYPE=Debug
echo "PRODUCT_BUILD_TYPE set to '$PRODUCT_BUILD_TYPE'."

# Call all build scripts one by one.
cd "$PRODUCT_SOURCE_DIR/ci/linux" && ./install_googletest.sh;
cd "$PRODUCT_SOURCE_DIR/ci/linux" && ./install_rapidassist.sh;
cd "$PRODUCT_SOURCE_DIR/ci/linux" && ./install_bin2cpp.sh;
cd "$PRODUCT_SOURCE_DIR/ci/linux" && ./test_script.sh;
