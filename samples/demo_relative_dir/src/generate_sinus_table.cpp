#include <stdio.h>
#include <string>
#include <iostream>

#include "libtrigo/sincos.h"

enum APP_ERROR_CODES
{
  APP_ERROR_SUCCESS = 0,
  APP_ERROR_OPERATION_FAILED,
  APP_ERROR_UNKNOWN_SYMBOL,
};

int generate_sinus_table(const char * filename)
{
  FILE * f = fopen(filename, "wb");
  if (!f)
    return APP_ERROR_OPERATION_FAILED;

  const float DEGREES_MIN = 0.0f;
  const float DEGREES_MAX = 180.0f;
  const size_t NUM_DIVISIONS = 10000;

  for(size_t i=0; i<NUM_DIVISIONS; i++)
  {
    float degrees = DEGREES_MIN + ((DEGREES_MAX-DEGREES_MIN)*i)/float(NUM_DIVISIONS);
    float rad = DEG2RAD(degrees);
    float value = sin(rad);

    fwrite(&degrees, sizeof(degrees), 1, f);
    fwrite(&value, sizeof(value), 1, f);
  }

  fclose(f);
  return APP_ERROR_SUCCESS;
}

int main(int argc, char* argv[])
{
  const char * output_file = "sinus.dat";
  int error_code = generate_sinus_table(output_file);
  if (error_code == APP_ERROR_SUCCESS)
    printf("Generated sinus table in file %s.\n", output_file);
  else
    printf("ERROR 0x%x! Failed generating file %s.\n", error_code, output_file);
  return error_code;
}
