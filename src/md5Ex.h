#pragma once

#include <string>

#include "md5.h"

struct MD5DIGEST
{
  unsigned char bytes[16];
};

void md5Encode(unsigned char * iBuffer, unsigned int iLength, MD5DIGEST & oDigest);

std::string toString(const MD5DIGEST & iDigest);

void MD5Update (MD5_CTX * context, const std::string & iValue);
