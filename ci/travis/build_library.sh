# Any commands which fail will cause the shell script to exit immediately
set -e

# Validate Travis CI environment
if [ "$TRAVIS_BUILD_DIR" = "" ]; then
  echo "Please define 'TRAVIS_BUILD_DIR' environment variable.";
  exit 1;
fi

# Call matching script for linux
this_filename=`basename "$0"`
$TRAVIS_BUILD_DIR/ci/linux/$this_filename
