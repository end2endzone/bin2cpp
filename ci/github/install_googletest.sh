# Any commands which fail will cause the shell script to exit immediately
set -e

# Validate GitHub CI environment
if [ "$GITHUB_WORKSPACE" = "" ]; then
  echo "Please define 'GITHUB_WORKSPACE' environment variable.";
  exit 1;
fi

# Call matching script for linux
this_filename=`basename "$0"`
$GITHUB_WORKSPACE/ci/linux/$this_filename
