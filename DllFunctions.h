#ifndef DLL_FUNCTIONS_H
#define DLL_FUNCTIONS_H

#include <string>

#ifdef _WIN32
#define DLLEXPORT __declspec (dllexport) 
#else
#define DLLEXPORT extern 
#endif

#ifdef __cplusplus
extern "C"
{
#endif

DLLEXPORT int GetVersion(wchar_t **str);

#ifdef __cplusplus
}
#endif

#endif