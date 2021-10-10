#pragma once
#ifdef UNICODE
#   error "The UNICODE preprocessor symbol should not be defined for an UTF-8-based application."
#   include <stop-compilation>
#endif

#undef STRICT
#undef NO_MINMAX
#undef WIN32_LEAN_AND_MEAN

#define STRICT
#define NO_MINMAX
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
