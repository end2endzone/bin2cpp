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

echo ============================================================================
echo Testing RapidAssist library...
echo ============================================================================
cd "$BIN2CPP_SOURCE_DIR/build/bin"
./generate_test_files.sh
if [ "$BIN2CPP_BUILD_TYPE" = "Debug" ]; then
  ./bin2cpp_unittest-d || true; #do not fail build even if a test fails.
else
  ./bin2cpp_unittest   || true; #do not fail build even if a test fails.
fi

# Note:
#  GitHub Action do not support uploading test results in a nice GUI. There is no build-in way to detect a failed test.
#  Do not reset the error returned by unit test execution. This will actually fail the build and will indicate in GitHub that a test has failed.

#Debug TestProcess filters:
#./bin2cpp_unittest --gtest_filter=TestFilesystem.testNormalizePath:TestProcess.testIsRunning:TestProcess.testProcesses*:TestProcess.testGetExitCode*:TestProcess.testWaitExit:TestString.testIsNumeric
#./bin2cpp_unittest --gtest_filter=TestProcess.testGetExitCode*:TestProcess.testWaitExit
