#include <stdio.h>
#include <string>
#include <iostream>

#include "libcalc/calc.h" //a single include file is all you need

enum APP_ERROR_CODES
{
  APP_ERROR_SUCCESS = 0,
  APP_ERROR_OPERATION_FAILED,
  APP_ERROR_UNKNOWN_SYMBOL,
};

int INT_main(int argc, char* argv[])
{
  int left_value;
  char symbol[10];
  int right_value;
 
  printf("Enter the left value of mathematical operation:  ");
  scanf ("%d",&left_value);
  printf("Enter the mathematical symbol of the operation:  ");
  scanf ("%5s",symbol);
  printf("Enter the right value of mathematical operation: ");
  scanf ("%d",&right_value);

  int success = 0;
  int result = 0;
  switch (symbol[0])
  {
  case '+':
    success = calc_add(left_value, right_value, &result);
    break;
  case '-':
    success = calc_substract(left_value, right_value, &result);
    break;
  case '*':
    success = calc_multiply(left_value, right_value, &result);
    break;
  case '/':
  case '\\':
    success = calc_divide(left_value, right_value, &result);
    break;
  default:
    printf("Error. Unknown mathematical symbol: '%c'.\n", symbol[0]);
    return APP_ERROR_UNKNOWN_SYMBOL;
  };

  if (success)
  {
    printf("%d%c%d=%d\n", left_value, symbol[0], right_value, result);
    return APP_ERROR_SUCCESS;
  }
  else
  {
    printf("Error. The '%c' operation has failed.\n", symbol[0]);
    return APP_ERROR_OPERATION_FAILED;
  }
}

int main(int argc, char* argv[])
{
  float degrees;
 
  printf("Enter a value in degrees:  ");
  scanf ("%f",&degrees);

  float sin_value = calc_sin(degrees);
  float cos_value = calc_cos(degrees);
  if (sin_value == CALC_INVALID || cos_value == CALC_INVALID)
  {
    printf("Error. Unable to compute sin() or cos() value for %f degrees.\n", degrees);
    return APP_ERROR_OPERATION_FAILED;
  }

  printf("sin( %10.8f deg ) = %f\n", degrees, sin_value);
  printf("cos( %10.8f deg ) = %f\n", degrees, cos_value);

  return APP_ERROR_SUCCESS;
}
