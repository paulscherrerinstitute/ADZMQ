#ifndef INC_ADZMQAPI_H
#define INC_ADZMQAPI_H

#if defined(_WIN32) || defined(__CYGWIN__)

#  if !defined(epicsStdCall)
#    define epicsStdCall __stdcall
#  endif

#  if defined(BUILDING_ADZMQ_API) && defined(EPICS_BUILD_DLL)
/* Building library as dll */
#    define ADZMQ_API __declspec(dllexport)
#  elif !defined(BUILDING_ADZMQ_API) && defined(EPICS_CALL_DLL)
/* Calling library in dll form */
#    define ADZMQ_API __declspec(dllimport)
#  endif

#elif __GNUC__ >= 4
#  define ADZMQ_API __attribute__ ((visibility("default")))
#endif

#if !defined(ADZMQ_API)
#  define ADZMQ_API
#endif

#if !defined(epicsStdCall)
#  define epicsStdCall
#endif

#endif /* INC_ADZMQAPI_H */

