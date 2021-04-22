# Any commands which fail will cause the shell script to exit immediately
set -e

# Validate mandatory environment variables
if [ "$BIN2CPP_BUILD_TYPE" = "" ]; then
  echo "Please define 'BIN2CPP_BUILD_TYPE' environment variable.";
  exit 1;
fi

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

# Prepare CMAKE parameters
export CMAKE_INSTALL_PREFIX="$BIN2CPP_SOURCE_DIR/install"
unset CMAKE_PREFIX_PATH
export CMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH;$BIN2CPP_SOURCE_DIR/third_parties/googletest/install"
export CMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH;$BIN2CPP_SOURCE_DIR/third_parties/RapidAssist/install"

echo ============================================================================
echo Generating bin2cpp library...
echo ============================================================================
cd "$BIN2CPP_SOURCE_DIR"
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=$BIN2CPP_BUILD_TYPE -DBIN2CPP_BUILD_TEST=ON -DBIN2CPP_BUILD_SAMPLES=ON -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_PREFIX" -DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH" ..

echo ============================================================================
echo Compiling bin2cpp library...
echo ============================================================================
cmake --build . -- -j4
echo

echo ============================================================================
echo Installing bin2cpp library into $BIN2CPP_SOURCE_DIR/install
echo ============================================================================
make install
echo
