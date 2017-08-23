//-----------------------------------------------
// macros.h
//-----------------------------------------------
// OS_IS_WIN        - Windows
// OS_IS_UNIX_LINUX - Linux / UNIX
// OS_IS_UNIX_MACOS - OSX / UNIX
//-----------------------------------------------
#if defined(_MSC_VER)  ||  \
    defined(__WIN32)   ||  \
    defined(__WIN32__) ||  \
    defined(_WIN32)    ||  \
    defined(WIN32)     ||  \
    defined(__WINNT)   ||  \
    defined(__WINNT__) ||  \
    defined(__WINNT)

    #define OS_IS_WIN            1
//-----------------------------------------------
#elif defined(__linux) ||  \
      defined(__linux__)

    #define OS_IS_UNIX_LINUX     1
//-----------------------------------------------
#elif defined(__APPLE__)

    #define OS_IS_UNIX_MACOS     1
    #include <stdint.h>
    typedef int32_t __int32;
//-----------------------------------------------
#else

    #error Cannot determine OS type
//-----------------------------------------------
#endif
//-----------------------------------------------