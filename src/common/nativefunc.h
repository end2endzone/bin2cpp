#ifndef NATIVEFUNC_H
#define NATIVEFUNC_H

#include <stdint.h>

namespace nativefunc
{

  ///<summary>
  ///Sleep the current thread for the given amount of milliseconds
  ///</summary>
  ///<param name="iTime">The time in milliseconds where the thread must sleep.</param>
  ///<return>On successfully sleeping for the requested interval, function returns 0.
  ///        If the call is interrupted or encounters an error, then it returns -1<return>
  int millisleep(uint32_t milliseconds);

}; //nativefunc

#endif //NATIVEFUNC_H
