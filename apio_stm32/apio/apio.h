// INCLUDE FILE generated by "Pelles C for Windows, version 8.00".

#ifndef _APIO_H
#define _APIO_H

#ifdef _APIO_
#define APIOAPI  __declspec(dllexport)
#else
#define APIOAPI  __declspec(dllimport)
#endif /* _APIO_ */

#ifndef WINAPI
#define WINAPI  __stdcall
#endif

APIOAPI int WINAPI SampleFunction(int, int);

#endif /* _APIO_H */
