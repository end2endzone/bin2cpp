# Any commands which fail will cause the shell script to exit immediately
set -e

# Validate mandatory environment variables
if [ "$PRODUCT_BUILD_TYPE" = "" ]; then
  echo "Please define 'PRODUCT_BUILD_TYPE' environment variable.";
  exit 1;
fi

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

# Prepare CMAKE parameters
export CMAKE_INSTALL_PREFIX="$PRODUCT_SOURCE_DIR/third_parties/googletest/install"
unset CMAKE_PREFIX_PATH
export CMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH;"

echo ============================================================================
echo Cloning googletest into $PRODUCT_SOURCE_DIR/third_parties/googletest
echo ============================================================================
mkdir -p "$PRODUCT_SOURCE_DIR/third_parties"
cd "$PRODUCT_SOURCE_DIR/third_parties"
git clone "https://github.com/google/googletest.git"
cd googletest
echo

echo Checking out version 1.8.0...
git -c advice.detachedHead=false checkout release-1.8.0
echo

echo ============================================================================
echo Generating googletest...
echo ============================================================================
mkdir -p build
cd build
cmake -Wno-dev -DCMAKE_BUILD_TYPE=$PRODUCT_BUILD_TYPE -DBUILD_SHARED_LIBS=OFF -DBUILD_GMOCK=OFF -DBUILD_GTEST=ON -DCMAKE_INSTALL_PREFIX="$CMAKE_INSTALL_PREFIX" -DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH" ..

echo ============================================================================
echo Compiling googletest...
echo ============================================================================
cmake --build . -- -j4
echo

echo ============================================================================
echo Installing googletest into $PRODUCT_SOURCE_DIR/third_parties/googletest/install
echo ============================================================================
make install
echo
