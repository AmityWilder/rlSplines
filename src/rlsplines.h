/*******************************************************************************************
*
*   rlsplines v0.1 - A raylib spline library
*
*   DESCRIPTION:
*       [TODO]
*
*   FEATURES:
*       [TODO]
*
*   CONFIGURATION:
*       #define RLSPLINES_IMPLEMENTATION
*           Generates the implementation of the library into the included file.
*           If not defined, the library is in header only mode and can be included in other headers
*           or source files without problems. But only ONE file should hold the implementation.
*
*       #define RLSPLINES_STANDALONE
*           Avoid raylib.h header inclusion in this file. Data types defined on raylib are defined
*           internally in the library and input management and drawing functions must be provided by
*           the user (check library implementation for further details).
*
*   VERSIONS HISTORY:
*       [TODO]
*
*   CONTRIBUTORS:
*       Amy Wilder:         [TODO] (2025)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) [TODO]
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef RLSPLINES_H
#define RLSPLINES_H

#define RLSPLINES_VERSION_MAJOR 0
#define RLSPLINES_VERSION_MINOR 1
#define RLSPLINES_VERSION_PATCH 0
#define RLSPLINES_VERSION  "0.1"

#if !defined(RLSPLINES_STANDALONE)
    #include "raylib.h"
#endif


// Function specifiers in case library is build/used as a shared library (Windows)
// NOTE: Microsoft specifiers to tell compiler that symbols are imported/exported from a .dll
#if defined(_WIN32)
    #if defined(BUILD_LIBTYPE_SHARED)
        #define RLSPLINESAPI __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
    #elif defined(USE_LIBTYPE_SHARED)
        #define RLSPLINESAPI __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
    #endif
#endif

// Function specifiers definition
#ifndef RLSPLINESAPI
    #define RLSPLINESAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif


//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Allow custom memory allocators
#ifndef RLSPLINES_MALLOC
    #define RLSPLINES_MALLOC(sz)       malloc(sz)
#endif
#ifndef RLSPLINES_CALLOC
    #define RLSPLINES_CALLOC(n,sz)     calloc(n,sz)
#endif
#ifndef RLSPLINES_FREE
    #define RLSPLINES_FREE(p)          free(p)
#endif

// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define RLSPLINES_SUPPORT_LOG_INFO
#if defined(RLSPLINES_SUPPORT_LOG_INFO)
  #define RLSPLINES_LOG(...)           printf(__VA_ARGS__)
#else
  #define RLSPLINES_LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
// NOTE: Some types are required for RLSPLINES_STANDALONE usage
//----------------------------------------------------------------------------------
#if defined(RLSPLINES_STANDALONE)
    #ifndef __cplusplus
    // Boolean type
        #ifndef true
            typedef enum { false, true } bool;
        #endif
    #endif

    // Vector2 type
    typedef struct Vector2 {
        float x;
        float y;
    } Vector2;

    // Vector3 type
    typedef struct Vector3 {
        float x;
        float y;
        float z;
    } Vector3;

    // Color type, RGBA (32bit)
    typedef struct Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } Color;
#endif      // RLSPLINES_STANDALONE

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------


#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

#if defined(__cplusplus)
}            // Prevents name mangling of functions
#endif

#endif      // RLSPLINES_H

/***********************************************************************************
*
*   RLSPLINES IMPLEMENTATION
*
************************************************************************************/

#if defined(RLSPLINES_IMPLEMENTATION)

#ifdef __cplusplus
    #define RLSPLINES_CLITERAL(name) name
#else
    #define RLSPLINES_CLITERAL(name) (name)
#endif

#if defined(RLSPLINES_STANDALONE)
#endif      // RLSPLINES_STANDALONE

#endif      // RLSPLINES_IMPLEMENTATION
