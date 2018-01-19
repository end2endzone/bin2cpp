#include "SegmentGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "common.h"
#include "cppencoder.h"

namespace bin2cpp
{
  SegmentGenerator::SegmentGenerator()
  {
  }

  SegmentGenerator::~SegmentGenerator()
  {
  }

  const char * SegmentGenerator::getName() const
  {
    return "segment";
  }

  bool SegmentGenerator::createCppSourceFile(const char * iInputFilename, const char * iCppFilePath, const char * iFunctionIdentifier, size_t iChunkSize, const char * iNamespace, const char * iBaseClass)
  {
    //check if input file exists
    FILE * input = fopen(iInputFilename, "rb");
    if (!input)
      return false;

    //Uppercase function identifier
    std::string functionIdentifier = iFunctionIdentifier;
    functionIdentifier[0] = (char)toupper(functionIdentifier[0]);

    //Build header and cpp file path
    std::string headerPath = getHeaderFilePath(iCppFilePath);
    std::string cppPath = iCppFilePath;
    std::string headerFilename = getFilename(headerPath.c_str());
    std::string cppFilename = getFilename(iCppFilePath);

    //create cpp file
    FILE * cpp = fopen(cppPath.c_str(), "w");
    if (!cpp)
    {
      fclose(input);
      return false;
    }

    //determine file properties
    long fileSize = getFileSize(input);
    std::string filename = getFilename(iInputFilename);
    //long lastSegmentSize = fileSize%iChunkSize;
    //size_t numSegments = fileSize/iChunkSize + (lastSegmentSize == 0 ? 0 : 1);

    //Build class name
    std::string className;
    className.append(functionIdentifier.c_str());
    className.append("File");

    //Build function 
    std::string getterFunctionName = getGetterFunctionName(functionIdentifier.c_str());

    //write cpp file heading
    fprintf(cpp, "%s", getFileHeading(iInputFilename).c_str());
    fprintf(cpp, "#include \"%s\"\n", headerFilename.c_str() );
    fprintf(cpp, "#include <stdio.h> //for FILE\n");
    fprintf(cpp, "#include <string> //for memcpy\n");
    fprintf(cpp, "namespace %s\n", iNamespace);
    fprintf(cpp, "{\n");
    fprintf(cpp, "  class %s : public virtual %s::%s\n", className.c_str(), iNamespace, iBaseClass);
    fprintf(cpp, "  {\n");
    fprintf(cpp, "  public:\n");
    fprintf(cpp, "    %s() { build(); }\n", className.c_str());
    fprintf(cpp, "    ~%s() {}\n", className.c_str());
    fprintf(cpp, "    virtual size_t getSize() const { return %d; }\n", fileSize);
    fprintf(cpp, "    virtual const char * getFilename() const { return \"%s\"; }\n", getFilename(iInputFilename).c_str());
    fprintf(cpp, "    virtual const char * getBuffer() const { return mBuffer.c_str(); }\n");
    fprintf(cpp, "    void build()\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      mBuffer.clear();\n");
    fprintf(cpp, "      mBuffer.reserve(getSize()); //allocate all required memory at once to prevent reallocations\n");

    //create buffer for each chunks from input buffer
    unsigned char * buffer = new unsigned char[iChunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, iChunkSize, input);

      //bool isLastChunk = !(readSize == iChunkSize);

      if (readSize == 0)
        continue; //nothing to output if nothing was read

      //output
      fprintf(cpp, "      mBuffer.append(\"%s\", %d);\n", toCppString(buffer, readSize).c_str(), readSize);
    }
    delete[] buffer;
    buffer = NULL;

    //write cpp file footer
    fprintf(cpp, "    }\n");
    fprintf(cpp, "%s", getSaveMethodImplementation().c_str());
    fprintf(cpp, "  private:\n");
    fprintf(cpp, "    std::string mBuffer;\n");
    fprintf(cpp, "  };\n");
    fprintf(cpp, "  const %s & %s() { static %s _instance; return _instance; }\n", iBaseClass, getterFunctionName.c_str(), className.c_str());
    fprintf(cpp, "}; //%s\n", iNamespace);

    fclose(input);
    fclose(cpp);

    return true;
  }

}; //bin2cpp