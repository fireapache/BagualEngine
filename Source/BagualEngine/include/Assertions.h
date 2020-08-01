#pragma once

#include <cassert>

#ifndef __FUNCTION_NAME__
#ifdef _WIN32   //WINDOWS
#define __FUNCTION_NAME__   __FUNCTION__  
#else          //*NIX
#define __FUNCTION_NAME__   __func__ 
#endif
#endif

#define BGL_NEED_IMPLEMENTATION assert((false && __FUNCTION_NAME__ && " Needs implementation!"));
#define BGL_ASSERT(...) assert(__VA_ARGS__ && __FUNCTION_NAME__)