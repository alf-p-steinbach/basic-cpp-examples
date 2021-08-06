#pragma once
#ifdef UNICODE
#   error "UNICODE defined for a `char` based app."
#endif

#undef STRICT
#undef NO_MINMAX
#undef WIN32_LEAN_AND_MEAN

#define STRICT
#define NO_MINMAX
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
