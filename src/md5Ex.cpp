#include "md5Ex.h"

void md5Encode(unsigned char * iBuffer, unsigned int iLength, MD5DIGEST & oDigest)
{
  MD5_CTX context;
  MD5Init(&context);
  MD5Update(&context, iBuffer, iLength);
  MD5Final(oDigest.bytes, &context);
}

std::string toString(const MD5DIGEST & iDigest)
{
  std::string hexValue = "";

  static const int bufferSize = 5;
  char buffer[bufferSize];

  for(int i=0; i<16; i++)
  {
    sprintf(buffer, "%02x", iDigest.bytes[i]);
    hexValue.append(buffer);
  }

  return hexValue;
}

void MD5Update (MD5_CTX * context, const std::string & iValue)
{
  unsigned char * buffer = (unsigned char *)iValue.c_str();
  size_t length = iValue.size();

  MD5Update(context, buffer, length);
}
