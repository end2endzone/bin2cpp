#include "nativefunc.h"

#ifdef WIN32
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN 1
  #endif
  #include <windows.h> // for Sleep()
  #undef min
  #undef max
#elif _POSIX_C_SOURCE >= 199309L
  #include <time.h>   // for nanosleep()
#else
  #include <unistd.h> // for usleep()
#endif


namespace nativefunc
{

  int millisleep(uint32_t milliseconds)
  {
    //code from https://stackoverflow.com/a/14818830 and https://stackoverflow.com/a/28827188
  #if defined(WIN32)
    SetLastError(0);
    Sleep(milliseconds);
    return GetLastError() ?-1 :0;
  #elif _POSIX_C_SOURCE >= 199309L
    /* prefer to use nanosleep() */
    const struct timespec ts = {
      (__time_t)milliseconds / 1000, /* seconds */
      ((__syscall_slong_t)milliseconds % 1000) * 1000 * 1000 /* nano seconds */
    };
    return nanosleep(&ts, NULL);
  #elif _BSD_SOURCE || \
    (_XOPEN_SOURCE >= 500 || \
       _XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED) && \
    !(_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700)

    /* else fallback to obsolte usleep() */
    return usleep(1000 * milliseconds);
  #else
    # error ("No millisecond sleep available for this platform!")
    return -1;
  #endif
  }

}; //nativefunc
