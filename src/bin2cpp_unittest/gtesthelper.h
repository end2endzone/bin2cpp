#ifndef GTESTHELPER_H
#define GTESTHELPER_H

#include <string>
#include <vector>

class gTestHelper
{
  //------------------------
  // Singleton
  //------------------------
private:
  gTestHelper();
public:
  ~gTestHelper();
  static gTestHelper & getInstance();

public:
  //------------------------
  // Inner classes & structures
  //------------------------

  typedef std::vector<std::string> StringVector;

  //
  // Description:
  //  Wrapper class for FILE* instance.
  //  Which automatically closes the FILE* on wrapper's destruction
  // 
  class FileWrapper
  {
  public:
    FileWrapper(const char * iPath, const char * iMode);
    ~FileWrapper();
    bool isEOF();
    void close();

    FILE * mPointer;
  };

  class SymbolsFlags
  {
  public:
      enum Flags
      {
        Letters=1,            //lowercase letters
        LETTERS=2,            //uppercase letters
        Numbers=4,            //numbers from 0 to 9
        SpecialCharacters=8,  //special characters like @ and !
        All=15,               //all flags
      };
  };

  struct FILE_DIFF
  {
    size_t offset;    //offset in files where a difference is located
    unsigned char c1; //character of first  file
    unsigned char c2; //character of second file
  };

public:
  //------------------------
  // Methods
  //------------------------

  //
  // Description:
  //  Returns true if a given file exists
  // Arguments:
  //   iFilePath: The path of the file to test
  // 
  bool fileExists(const char * iFilePath);

  //
  // Description:
  //  Returns the size of a file.
  // Arguments:
  //   iFilePath: The path of the file
  // 
  long getFileSize(const char* iFilePath);
  long getFileSize(FILE * iFile);

  //
  // Description:
  //  Returns true if the content of two files is identical.
  // Arguments:
  //   iFile1:          The path of the first  file.
  //   iFile2:          The path of the second file.
  //   oReason:         A textual reason why the files are not identical. Empty if files are identical.
  //   iMaxDifferences: The maximum number of textual differences.
  // 
  bool isFileEquals(const char* iFile1, const char* iFile2);
  bool isFileEquals(const char* iFile1, const char* iFile2, std::string & oReason);
  bool isFileEquals(const char* iFile1, const char* iFile2, std::string & oReason, size_t iMaxDifferences);

  //
  // Description:
  //  Returns the location of all differences in two files.
  // Arguments:
  //   iFile1:          The path of the first  file.
  //   iFile2:          The path of the second file.
  //   oDifferences:    The list of all differences within both files. Empty if files are identical.
  //   iMaxDifferences: The maximum number of differences.
  // 
  bool getFileDifferences(const char* iFile1, const char* iFile2, std::vector<FILE_DIFF> & oDifferences, size_t iMaxDifferences);

  //
  // Description:
  //  Returns true if the given text is found in a file.
  // Arguments:
  //   iFilename:   The path of the search file.
  //   iValue:      The search value.
  //   oLine:       The line number where iValue is found.
  //   oCharacter:  The character offset within the line.
  // 
  bool findInFile(const char* iFilename, const char* iValue, int & oLine, int & oCharacter);

  //
  // Description:
  //  Returns the content (each lines) of a file.
  // Arguments:
  //   iFilename:   The path of the file.
  //   oLines:      The content of the file line by line.
  // 
  bool getTextFileContent(const char* iFilename, gTestHelper::StringVector & oLines );

  //
  // Description:
  //  Creates a file of the given size. All bytes are sequential.
  // Arguments:
  //   iFilePath:   The path of the file.
  // 
  void createFile(const char * iFilePath, size_t iSize);

  //
  // Description:
  //  Modify a given byte with the specified value.
  // Arguments:
  //   iFilePath: The path of the file.
  //   iOffset:   The offset of the modified byte.
  //   iValue:    The value of the replacement.
  // 
  void changeFileContent(const char * iFilePath, size_t iOffset, unsigned char iValue);

  //
  // Description:
  //  Finds the given arguments within an argument list.
  //  Returns true if the argument is found. Returns false otherwise.
  //  Arguments must be supplied with the following format: --name=value
  // Arguments:
  //   iName:   The name of the argument
  //   oValue:  The value of the argument (without the name of the argument)
  //   argc:    The number of arguments supplied in argv.
  //   argv:    The argument list.
  // 
  bool findArgument(const char * iName, std::string & oValue, int argc, char **argv);

  //
  // Description:
  //  This function build a GTEST filter automatically based on a given positive and a negative filter.
  //  The result filter format is the following:
  //    positive[-negative] where 
  //        [ ... ] is optionnal
  //        positive and negative are ':'-separated test case name lists
  // Arguments:
  //   iPositiveFilter:   positive filter
  //   iNegativeFilter:   negative filter
  //   iExistingFilter:   existing filter or a pre-build filter (build from a previous call to mergeFilter)
  // Example:
  //   calling mergeFilter("", "C21XB1.testSniperA2GRanging25:C21XB1.testSniperA2GRanging40") return the filter
  //   "*-C21XB1.testSniperA2GRanging25:C21XB1.testSniperA2GRanging40"
  // 
  std::string mergeFilter(const std::string & iPositiveFilter, const std::string & iNegativeFilter);
  std::string mergeFilter(const std::string & iPositiveFilter, const std::string & iNegativeFilter, const char * iExistingFilter);
  std::string mergeFilter(const std::string & iPositiveFilter, const std::string & iNegativeFilter, int argc, char **argv);

  //
  // Description:
  //  This function split a GTEST filter into a positive and a negative filter.
  // Notes:
  //   iFilter must not be NULL
  // Example:
  //   calling splitFilter("", ..., ...) returns the "" as positive filter and "" as negative filter
  //   calling splitFilter("*", ..., ...) returns the "*" as positive filter and "" as negative filter
  //   calling splitFilter("-TestLib.testCase", ..., ...) returns the "" as positive filter and "TestLib.testCase" as negative filter
  //   calling splitFilter("TestFoo.testFoo-TestLib.testCase:TestMath.testMath", ..., ...) returns the "TestFoo.testFoo" as positive filter and "TestLib.testCase:TestMath.testMath" as negative filter
  // 
  void splitFilter(const char * iFilter, std::string & oPositiveFilter, std::string & oNegativeFilter);

  //
  // Description
  //  Returns a list of all runnable test cases in a gTest compatible test.
  //  Each test case is listed in the following format: testsuite.testcase
  // Example:
  //  TestGTestHelper.testFilters
  // Arguments:
  //  iTestCasePath:    Path to the GTest compatible executable file
  //
  gTestHelper::StringVector getTestList(const char * iTestCasePath);

  //
  // Description
  //  Returns a random number.
  //
  int getRandomInt();

  //
  // Description
  //  Returns a random number.
  // Arguments:
  //  iMin: The minimum value that can be generated.
  //  iMax: The maximum value that can be generated.
  //
  int getRandomInt(int iMin, int iMax);

  //
  // Description
  //  Returns a random string.
  // Arguments:
  //  oValue:   The generated output string if arguments are specified.
  //  iMaxLen:  The maximum length of the generated string.
  //  iFlags:   The flags that specify what kind of symbols is allowed for the generation of the string.
  //  iSymbols: The list of symbols that can be used for generating the string.
  //
  std::string getRandomString();
  void getRandomString(std::string & oValue, int iMaxLen);
  void getRandomString(std::string & oValue, int iMaxLen, SymbolsFlags::Flags iFlags);
  void getRandomString(std::string & oValue, int iMaxLen, const char* iSymbols);

  //
  // Description
  //  Returns true if the specified processor flag is enabled
  //
  bool isProcessorX86();
  bool isProcessorX64();
  bool isDebugCode();
  bool isReleaseCode();

  //
  // Description
  //  Returns the name of the current Test Suite name. ie "TestFooClass"
  //
  std::string getTestSuiteName();

  //
  // Description
  //  Returns the name of the current Test Case name. ie "testConstructor"
  //
  std::string getTestCaseName();

  //
  // Description
  //  Returns the fully qualified name of the current test case name. ie "TestFooClass.testConstructor"
  //
  std::string getTestQualifiedName();

  //
  // Description
  //  Removes occurance of unix/windows LF, CR or CRLF into the given string.
  // Arguments:
  //  iBuffer: The given buffer to modify.
  //
  void removeCRLF(char * iBuffer);

  //
  // Description
  //  Splits an input string into multiple string based on the given splitting character.
  // Arguments:
  //  iText:            The input text to split.
  //  iSplitCharacter:  The splitting character.
  //  iSplitPattern:    The splitting pattern.
  // Returns
  //  Returns a list of string.
  //
  gTestHelper::StringVector splitString(const std::string & iText, char iSplitCharacter);
  void splitString(gTestHelper::StringVector & oList, const char * iText, const char * iSplitPattern);

};

#endif //GTESTHELPER_H
