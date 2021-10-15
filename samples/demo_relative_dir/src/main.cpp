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

int main(int argc, char* argv[])
{
  float degrees;
 
  printf("Enter a value in degrees:  ");
  scanf ("%f",&degrees);

  float sin_value = sindeg(degrees);
  float cos_value = cosdeg(degrees);
  if (sin_value == LIBTRIGO_INVALID || cos_value == LIBTRIGO_INVALID)
  {
    printf("Error. Unable to compute sin() or cos() value for %f degrees.\n", degrees);
    return APP_ERROR_OPERATION_FAILED;
  }

  printf("sin( %10.8f deg ) = %f\n", degrees, sin_value);
  printf("cos( %10.8f deg ) = %f\n", degrees, cos_value);

  return APP_ERROR_SUCCESS;
}
