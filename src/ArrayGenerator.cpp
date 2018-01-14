#include "ArrayGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "common.h"
#include "cppencoder.h"
#include "md5support.h"
#include "md5.h"
#include "md5Ex.h"

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

  bin2cpp::ErrorCodes ArrayGenerator::createCppSourceFile(const char * iInputFilename, const char * iCppFilePath, const char * iFunctionIdentifier, size_t iChunkSize)
  {
    //check if input file exists
    FILE * input = fopen(iInputFilename, "rb");
    if (!input)
      return bin2cpp::ErrorCodes::InputFileNotFound;

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
      return bin2cpp::ErrorCodes::UnableToCreateOutputFiles;
    }

    //determine file properties
    long fileSize = getFileSize(input);
    std::string filename = getFilename(iInputFilename);

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
    fprintf(cpp, "namespace bin2cpp\n");
    fprintf(cpp, "{\n");
    fprintf(cpp, "  class %s : public virtual bin2cpp::File\n", className.c_str());
    fprintf(cpp, "  {\n");
    fprintf(cpp, "  public:\n");
    fprintf(cpp, "    %s() {}\n", className.c_str());
    fprintf(cpp, "    ~%s() {}\n", className.c_str());
    fprintf(cpp, "    virtual size_t getSize() const { return %d; }\n", fileSize);
    fprintf(cpp, "    virtual const char * getFilename() const { return \"%s\"; }\n", getFilename(iInputFilename).c_str());
    fprintf(cpp, "    virtual const char * getBuffer() const\n");
    fprintf(cpp, "    {\n");
    fprintf(cpp, "      static const unsigned char buffer[] = {\n");

    //Compute MD5 while generating cpp code
    MD5_CTX context;
    MD5Init(&context);

    //create buffer for each chunks from input buffer
    int numLinePrinted = 0;
    unsigned char * buffer = new unsigned char[iChunkSize];
    while(!feof(input))
    {
      //read a chunk of the file
      size_t readSize = fread(buffer, 1, iChunkSize, input);

      bool isLastChunk = !(readSize == iChunkSize);

      if (readSize > 0)
      {
        if (numLinePrinted > 0)
        {
          //end previous line
          fprintf(cpp, ",\n");
        }

        //send to MD5 for analysist
        MD5Update(&context, buffer, readSize);

        //output
        fprintf(cpp, "        %s", toCppCharactersArray(buffer, readSize).c_str());
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

    //compute final digest
    MD5DIGEST digest;
    MD5Final(digest.bytes, &context);
    std::string md5String = toString(digest);

    //write cpp file footer
    fprintf(cpp, "      return (const char *)buffer;\n");
    fprintf(cpp, "    }\n");
    fprintf(cpp, "    virtual const char * getMd5() const { return \"%s\"; }\n", md5String.c_str() );
    fprintf(cpp, "%s", getSaveMethodImplementation().c_str());
    fprintf(cpp, "  };\n");
    fprintf(cpp, "  const File & %s() { static %s _instance; return _instance; }\n", getterFunctionName.c_str(), className.c_str());
    fprintf(cpp, "}; //bin2cpp\n");

    fclose(input);
    fclose(cpp);

    return bin2cpp::ErrorCodes::Success;
  }

}; //bin2cpp