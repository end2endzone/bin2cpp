#include "ArrayGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "rapidassist/cppencoder.h"
#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

namespace bin2cpp
{
  ArrayGenerator::ArrayGenerator()
  {
  }

  ArrayGenerator::~ArrayGenerator()
  {
  }

  const char * ArrayGenerator::getName() const
  {
    return "array";
  }

  bool ArrayGenerator::createCppSourceFile(const char * iCppFilePath)
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
    uint32_t fileSize = ra::filesystem::getFileSize(input);
    std::string filename = ra::filesystem::getFilename(mInputFile.c_str());

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
    fprintf(cpp, "    %s() {}\n", className.c_str());
    fprintf(cpp, "    ~%s() {}\n", className.c_str());
    fprintf(cpp, "    virtual size_t getSize() const { return %lu; }\n", fileSize);
    fprintf(cpp, "    virtual const char * getFilename() const { return \"%s\"; }\n", ra::filesystem::getFilename(mInputFile.c_str()).c_str());
    fprintf(cpp, "    virtual const char * getBuffer() const\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      static const unsigned char buffer[] = {\n");

    //create buffer for each chunks from input buffer
    int numLinePrinted = 0;
    unsigned char * buffer = new unsigned char[mChunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, mChunkSize, input);

      bool isLastChunk = !(readSize == mChunkSize);

      if (readSize > 0)
      {
        if (numLinePrinted > 0)
        {
          //end previous line
          fprintf(cpp, ",\n");
        }

        //output
        fprintf(cpp, "        %s", ra::cppencoder::toCppCharactersArray(buffer, readSize).c_str());
        numLinePrinted++;
      }

      //end the array. all the file content is printed
      if (isLastChunk)
      {
        fprintf(cpp, "\n");
        fprintf(cpp, "      };\n");
      }
    }
    delete[] buffer;
    buffer = NULL;

    //write cpp file footer
    fprintf(cpp, "      return (const char *)buffer;\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "%s", getSaveMethodTemplate().c_str());
    fprintf(cpp, "  };\n");
    fprintf(cpp, "  const %s & %s() { static %s _instance; return _instance; }\n", mBaseClass.c_str(), getterFunctionName.c_str(), className.c_str());
    fprintf(cpp, "}; //%s\n", mNamespace.c_str());

    fclose(input);
    fclose(cpp);

    return true;
  }

}; //bin2cpp