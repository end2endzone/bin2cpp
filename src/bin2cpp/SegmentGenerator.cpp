#include "SegmentGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "rapidassist/cppencoder.h"
#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

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

  bool SegmentGenerator::createCppSourceFile(const char * iCppFilePath)
  {
    //check if input file exists
    FILE * input = fopen(mInputFile.c_str(), "rb");
    if (!input)
      return false;

    //Uppercase function identifier
    std::string functionIdentifier = ra::strings::capitalizeFirstCharacter(mFunctionIdentifier);

    //Build header and cpp file path
    std::string headerPath = getHeaderFilePath(iCppFilePath);
    std::string cppPath = iCppFilePath;
    std::string headerFilename = ra::filesystem::getFilename(headerPath.c_str());
    std::string cppFilename = ra::filesystem::getFilename(iCppFilePath);

    //create cpp file
    FILE * cpp = fopen(cppPath.c_str(), "w");
    if (!cpp)
    {
      fclose(input);
      return false;
    }

    //determine file properties
    long fileSize = ra::filesystem::getFileSize(input);
    std::string filename = ra::filesystem::getFilename(mInputFile.c_str());
    //long lastSegmentSize = fileSize%iChunkSize;
    //size_t numSegments = fileSize/iChunkSize + (lastSegmentSize == 0 ? 0 : 1);

    //Build class name
    std::string className;
    className.append(functionIdentifier.c_str());
    className.append("File");

    //Build function 
    std::string getterFunctionName = getGetterFunctionName();

    //write cpp file heading
    fprintf(cpp, "%s", getHeaderTemplate().c_str());
    fprintf(cpp, "#include \"%s\"\n", headerFilename.c_str() );
    fprintf(cpp, "#include <stdio.h> //for FILE\n");
    fprintf(cpp, "#include <string> //for memcpy\n");
    fprintf(cpp, "namespace %s\n", mNamespace.c_str());
    fprintf(cpp, "{\n");
    fprintf(cpp, "  class %s : public virtual %s::%s\n", className.c_str(), mNamespace.c_str(), mBaseClass.c_str());
    fprintf(cpp, "  {\n");
    fprintf(cpp, "  public:\n");
    fprintf(cpp, "    %s() { build(); }\n", className.c_str());
    fprintf(cpp, "    ~%s() {}\n", className.c_str());
    fprintf(cpp, "    virtual size_t getSize() const { return %d; }\n", fileSize);
    fprintf(cpp, "    virtual const char * ra::filesystem::getFilename() const { return \"%s\"; }\n", ra::filesystem::getFilename(mInputFile.c_str()).c_str());
    fprintf(cpp, "    virtual const char * getBuffer() const { return mBuffer.c_str(); }\n");
    fprintf(cpp, "    void build()\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      mBuffer.clear();\n");
    fprintf(cpp, "      mBuffer.reserve(getSize()); //allocate all required memory at once to prevent reallocations\n");

    //create buffer for each chunks from input buffer
    unsigned char * buffer = new unsigned char[mChunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, mChunkSize, input);

      //bool isLastChunk = !(readSize == iChunkSize);

      if (readSize == 0)
        continue; //nothing to output if nothing was read

      //convert to cpp string
      std::string cppEncoder;
      switch(mCppEncoder)
      {
      case IGenerator::CPP_ENCODER_HEX:
        cppEncoder = ra::cppencoder::toHexString(buffer, readSize);
        break;
      case IGenerator::CPP_ENCODER_OCT:
      default:
        cppEncoder = ra::cppencoder::toOctString(buffer, readSize, false);
        break;
      };

      //output
      fprintf(cpp, "      mBuffer.append(\"%s\", %d);\n", cppEncoder.c_str(), readSize);
    }
    delete[] buffer;
    buffer = NULL;

    //write cpp file footer
    fprintf(cpp, "    }\n");
    fprintf(cpp, "%s", getSaveMethodTemplate().c_str());
    fprintf(cpp, "  private:\n");
    fprintf(cpp, "    std::string mBuffer;\n");
    fprintf(cpp, "  };\n");
    fprintf(cpp, "  const %s & %s() { static %s _instance; return _instance; }\n", mBaseClass.c_str(), getterFunctionName.c_str(), className.c_str());
    fprintf(cpp, "}; //%s\n", mNamespace.c_str());

    fclose(input);
    fclose(cpp);

    return true;
  }

}; //bin2cpp