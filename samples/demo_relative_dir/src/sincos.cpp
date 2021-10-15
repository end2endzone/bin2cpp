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

#include "libtrigo/sincos.h"
#include "bin2cpp/file_sinus.h"

struct SINUS_TABLE_ENTRY
{
  float degrees;
  float value;
};

inline float map(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float sindeg(float degrees)
{
  float result;
  if (degrees < 0)
  {
    result = -sindeg(-degrees);
    return result;
  }
  if (degrees > 360.0f)
  {
    int count = (int)(degrees/360.0f);
    degrees -= count*360.0f;
  }
  if (degrees > 180.0f)
  {
    result = -sindeg(degrees-180.0f);
    return result;
  }
  if (degrees == 0.0f)
    return 0.0f;
  else if (degrees == 90.0f)
    return 1.0f;
  else if (degrees == 180.0f)
    return 0.0f;

  // resolve the sinus value from a table
  const bin2cpp::File & sinus_table = bin2cpp::getSinusTableFile();
  const SINUS_TABLE_ENTRY * entries = (const SINUS_TABLE_ENTRY*)sinus_table.getBuffer();
  size_t num_entries = sinus_table.getSize()/sizeof(SINUS_TABLE_ENTRY);

  for(size_t i=0; i<(num_entries-1); i++)
  {
    const SINUS_TABLE_ENTRY & low = entries[i];
    const SINUS_TABLE_ENTRY & high = entries[i+1];
    if (low.degrees <= degrees && degrees <= high.degrees)
    {
      // proceed with linear interpolation 
      result = map(degrees, low.degrees, high.degrees, low.value, high.value);
      return result;
    }
  }

  result = LIBTRIGO_INVALID;
  return result;
}

float cosdeg(float degrees)
{
  return sindeg(degrees + 90.0f);
}
