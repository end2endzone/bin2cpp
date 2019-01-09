#include "BaseGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdlib.h>

#include "common.h"

#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

namespace bin2cpp
{
  BaseGenerator::BaseGenerator() :
    mCppEncoder(IGenerator::CPP_ENCODER_OCT)
  {
  }

  BaseGenerator::~BaseGenerator()
  {
  }

  void BaseGenerator::setInputFile(const char * iPath)
  {
    if (iPath)
      mInputFile = iPath;
  }

  const char * BaseGenerator::getInputFile() const
  {
    return mInputFile.c_str();
  }

  void BaseGenerator::setFunctionIdentifier(const char * iFunctionIdentifier)
  {
    if (iFunctionIdentifier)
      mFunctionIdentifier = iFunctionIdentifier;
  }

  const char * BaseGenerator::getFunctionIdentifier() const
  {
    return mFunctionIdentifier.c_str();
  }

  void BaseGenerator::setChunkSize(size_t iChunkSize)
  {
    mChunkSize = iChunkSize;
  }

  size_t BaseGenerator::getChunkSize() const
  {
    return mChunkSize;
  }

  void BaseGenerator::setNamespace(const char * iName)
  {
    if (iName)
      mNamespace = iName;
  }

  const char * BaseGenerator::getNamespace() const
  {
    return mNamespace.c_str();
  }

  void BaseGenerator::setBaseClass(const char * iName)
  {
    if (iName)
      mBaseClass = iName;
  }

  const char * BaseGenerator::getBaseClass() const
  {
    return mBaseClass.c_str();
  }

  void BaseGenerator::setCppEncoder(const IGenerator::CppEncoderEnum & iCppEncoder)
  {
    mCppEncoder = iCppEncoder;
  }

  IGenerator::CppEncoderEnum BaseGenerator::getCppEncoder() const
  {
    return mCppEncoder;
  }

  //-------------------------------
  //protected methods
  //-------------------------------

  std::string BaseGenerator::getGetterFunctionName()
  {
    //Uppercase function identifier
    std::string functionIdentifier = ra::strings::capitalizeFirstCharacter(mFunctionIdentifier);

    std::string getter;
    getter.append("get");
    getter.append(functionIdentifier);
    getter.append("File");
    return getter;
  }

  std::string BaseGenerator::getHeaderFilePath(const char * iCppFilePath)
  {
    //Build header file path
    std::string headerPath = iCppFilePath;
    ra::strings::replace(headerPath, ".cpp", ".h");
    return headerPath;
  }

  std::string BaseGenerator::getCppFilePath(const char * iHeaderFilePath)
  {
    //Build header file path
    std::string cppPath = iHeaderFilePath;
    ra::strings::replace(cppPath, ".cpp", ".h");
    return cppPath;
  }

  std::string BaseGenerator::getHeaderTemplate()
  {
    std::string filename = ra::filesystem::getFilename(mInputFile.c_str());
    uint64_t lastModifiedDate = ra::filesystem::getFileModifiedDate(mInputFile);

    std::string header;
    header << 
    header << "/**\n";
    header << " * This file was generated by bin2cpp v" << bin2cpp::getVersionString() << "\n";
    header << " * Copyright (C) 2013-" << bin2cpp::getCopyrightYear() << " end2endzone.com. All rights reserved.\n";
    header << " * bin2cpp is open source software, see http://github.com/end2endzone/bin2cpp\n";
    header << " * Source code for file '" << filename << "', last modified " << lastModifiedDate << ".\n";
    header << " * Do not modify this file.\n";
    header << " */\n";
    return header;
  }

  std::string BaseGenerator::getSaveMethodTemplate()
  {
    std::string output;
    output << "    virtual bool save(const char * iFilename) const\n";
    output << "    {\n";
    output << "      FILE * f = fopen(iFilename, \"wb\");\n";
    output << "      if (!f) return false;\n";
    output << "      size_t fileSize = getSize();\n";
    output << "      const char * buffer = getBuffer();\n";
    output << "      fwrite(buffer, 1, fileSize, f);\n";
    output << "      fclose(f);\n";
    output << "      return true;\n";
    output << "    }\n";
    return output;
  }

  bool BaseGenerator::createCppHeaderFile(const char * iHeaderFilePath)
  {
    FILE * header = fopen(iHeaderFilePath, "w");
    if (!header)
      return false;

    std::string headercomments = getHeaderTemplate();
    fprintf(header, "%s", headercomments.c_str());
    fprintf(header, "#pragma once\n");
    fprintf(header, "#include <stddef.h>\n");
    fprintf(header, "namespace %s\n", mNamespace.c_str());
    fprintf(header, "{\n");
    fprintf(header, "  #ifndef BIN2CPP_EMBEDDEDFILE_CLASS\n");
    fprintf(header, "  #define BIN2CPP_EMBEDDEDFILE_CLASS\n");
    fprintf(header, "  class %s\n", mBaseClass.c_str());
    fprintf(header, "  {\n");
    fprintf(header, "  public:\n");
    fprintf(header, "    virtual size_t getSize() const = 0;\n");
    fprintf(header, "    virtual const char * getFilename() const = 0;\n");
    fprintf(header, "    virtual const char * getBuffer() const = 0;\n");
    fprintf(header, "    virtual bool save(const char * iFilename) const = 0;\n");
    fprintf(header, "  };\n");
    fprintf(header, "  #endif\n");
    fprintf(header, "  const %s & %s();\n", mBaseClass.c_str(), getGetterFunctionName().c_str());
    fprintf(header, "}; //%s\n", mNamespace.c_str());

    fclose(header);

    return true;
  }

}; //bin2cpp