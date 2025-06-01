/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

#ifndef ENUMS_H
#define ENUMS_H
 
namespace bin2cpp
{

  ///<summary>
  ///Error codes returned by the application
  ///</summary>
  enum APP_ERROR_CODES
  {
    APP_ERROR_SUCCESS = 0,
    APP_ERROR_MISSINGARGUMENTS,
    APP_ERROR_INPUTFILENOTFOUND,
    APP_ERROR_UNABLETOCREATEOUTPUTFILES,
    APP_ERROR_TOOMANYARGUMENTS,
    APP_ERROR_INPUTDIRNOTFOUND,
    AAP_ERROR_NOTSUPPORTED,
    APP_ERROR_OPERATIONHASFAILED,
    APP_ERROR_INVALIDVALUE,
  };

  ///<summary>
  ///File update modes.
  ///</summary>
  enum FILE_UPDATE_MODE
  {
    WRITING,
    UPDATING,
    OVERWRITING,
    SKIPPING,
  };

  ///<summary>
  ///Defines the different types of cpp encoding.
  ///</summary>
  enum CppEncoderEnum
  {
      CPP_ENCODER_OCT,
      CPP_ENCODER_HEX,
  };

  ///<summary>
  ///Defines the different types of programming language code output.
  ///</summary>
  enum CodeGenerationEnum
  {
      CODE_GENERATION_UNKNOW,
      CODE_GENERATION_CPP,
      CODE_GENERATION_C,
  };
  
}; //bin2cpp

#endif //ENUMS_H
