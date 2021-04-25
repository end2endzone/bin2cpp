# Any commands which fail will cause the shell script to exit immediately
set -e

# Set BIN2CPP_SOURCE_DIR root directory
if [ "$BIN2CPP_SOURCE_DIR" = "" ]; then
  RESTORE_DIRECTORY="$PWD"
  cd "$(dirname "$0")"
  cd ../..
  export BIN2CPP_SOURCE_DIR="$PWD"
  echo "BIN2CPP_SOURCE_DIR set to '$BIN2CPP_SOURCE_DIR'."
  cd "$RESTORE_DIRECTORY"
  unset RESTORE_DIRECTORY
fi

# Set debug or release build type
export BIN2CPP_BUILD_TYPE=Release
echo "BIN2CPP_BUILD_TYPE set to '$BIN2CPP_BUILD_TYPE'."

# Call all build scripts one by one.
cd "$BIN2CPP_SOURCE_DIR/ci/linux" && ./install_googletest.sh;
cd "$BIN2CPP_SOURCE_DIR/ci/linux" && ./install_rapidassist.sh;
cd "$BIN2CPP_SOURCE_DIR/ci/linux" && ./install_bin2cpp.sh;
cd "$BIN2CPP_SOURCE_DIR/ci/linux" && ./test_script.sh;
