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
*       #define RLSPLINES_3D
*           Include support for 3D splines.
*
*       #define RLSPLINES_GRADIENT
*           Include support for gradient spline drawing.
*
*       #define RLSPLINES_ULTIMATE
*           Include support for "ultimate" (`_Ult()`) spline functions (lots of parameters).
*
*       #define RLSPLINES_DIMENSION_ALIASES
*           Define aliases with "1/2D" in the name for unspecified spline types/functions, to
*           help distinguish from their 2/3D counterparts, for consistency, or if you just prefer
*           having that additional specificity.
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

#define AMY_IS_A_DUMMY_AND_FORGOT_TO_DISABLE_THIS_MACRO // ! ONLY FOR TESTING: DISABLE BEFORE PUSHING !
#if defined(AMY_IS_A_DUMMY_AND_FORGOT_TO_DISABLE_THIS_MACRO)
    #define RLSPLINES_STANDALONE
    #define RLSPLINES_IMPLEMENTATION
    #define RLSPLINES_3D
    #define RLSPLINES_GRADIENT
    #define RLSPLINES_ULTIMATE
    #define RLSPLINES_DIMENSION_ALIASES
#endif

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

    #if defined(RLSPLINES_3D)
    // Vector3 type
    typedef struct Vector3 {
        float x;
        float y;
        float z;
    } Vector3;
    #endif      // RLSPLINES_3D

    // Color type, RGBA (32bit)
    typedef struct Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } Color;
#endif      // RLSPLINES_STANDALONE

// Spline, 2D spline point data
typedef struct Spline {
    int type;                           // Spline type: SPLINE_LINEAR, SPLINE_BASIS, SPLINE_CATMULL_ROM, SPLINE_BEZIER_QUAD, or SPLINE_BEZIER_CUBIC
    int pointCount;                     // Number of points and control points (spline type controls ratio and order)
    float *points;                      // Points and control points in spline (XY - 2 components per vertex), minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
} Spline;

#if defined(RLSPLINES_3D)
// Spline3D, 3D spline point data
typedef struct Spline3D {
    int type;                           // Spline type: SPLINE_LINEAR, SPLINE_BASIS, SPLINE_CATMULL_ROM, SPLINE_BEZIER_QUAD, or SPLINE_BEZIER_CUBIC
    int pointCount;                     // Number of points and control points (spline type controls ratio and order)
    float *points;                      // Point and control point positions in spline (XYZ - 3 components per vertex), minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
    float *rotations;                   // Roll (twist about tangent) angles, or NULL if all are 0
    SplineProfile profile;              // Spline profile, the shape that gets extruded along the spline's curve
} Spline3D;
#endif      // RLSPLINES_3D

// Spline thickness, variable thickness data in the form of a 1D Cubic Bezier spline with remapped timings (t-values) along the curve of another spline
typedef struct SplineThickness {
    int thicknessCount;                 // Number of thicknesses
    float *timings;                     // Thickness timings (t-values), minimum 4 values (2 control values): [t1, t2, t3, t4, t5, t6...], or NULL if evenly spread
    float *thicknesses;                 // Thicknesses, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
} SplineThickness;

#if defined(RLSPLINES_3D)
// 2D Spline thickness, variable thickness data in the form of a 2D Cubic Bezier spline with remapped timings (t-values) along the curve of another spline
// NOTE: Only valid for a 3D spline, a 2D spline only has 1 perpendicular axis
typedef struct SplineThickness2D {
    int thicknessCount;                 // Number of thicknesses
    float *timings;                     // Thickness timings (t-values), minimum 4 values (2 control values): [p1t, c2t, c3t, p4t, c5t, c6t...], or NULL if evenly spread
    Vector2 *thicknesses;               // Thicknesses, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
} SplineThickness2D;
#endif      // RLSPLINES_3D

#if defined(RLSPLINES_GRADIENT)

// Spline gradient, color gradient data along a spline in the form of a linear color spline with remapped timings (t-values) along the curve of another spline
typedef struct SplineGradient {
    int controlCount;                   // Number of controls
    float *timings;                     // Control timings, minimum 2 timings, or NULL if evenly spread
    unsigned char *colors;              // Control colors (RGBA - 4 components per vertex)
} SplineGradient;

// TODO:
//   I'm not sure if this is the correct way of handling a 2D/3D gradient.
//   The effect I'm going for is to allow multiple parallel colors at the same time-along-curve, but on separate perpendicular axis.
//   However, I don't know whether that belongs in the "timing remap" field or a separate field entirely.

// Spline gradient, color gradient data along a spline in the form of a linear color spline with remapped 2D timings (t-values) along the curve of another spline
// NOTE: Only valid for a spline with thickness, a 1-pixel-wide spline only has one gradientable axis
typedef struct SplineGradient2D {
    int controlCount;                   // Number of controls
    float *timings;                     // Control timings (XY - 2 components per timing), minimum 2 timings, or NULL if evenly spread
    unsigned char *colors;              // Control colors (RGBA - 4 components per vertex)
} SplineGradient2D;

#if defined(RLSPLINES_3D)
// Spline gradient, color gradient data along a spline in the form of a linear color spline with remapped 3D timings (t-values) along the curve of another spline
// NOTE: Only valid for a 3D spline, a 2D spline only has 2 gradientable axis
typedef struct SplineGradient3D {
    int controlCount;                   // Number of controls
    float *timings;                     // Control timings (XYZ - 3 components per timing), minimum 2 timings, or NULL if evenly spread
    unsigned char *colors;              // Control colors (RGBA - 4 components per vertex)
} SplineGradient3D;
#endif      // RLSPLINES_3D

#endif      // RLSPLINES_GRADIENT


//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------
// Spline type
typedef enum {
    SPLINE_LINEAR = 0,                  // Linear spline, minimum 2 points
    SPLINE_BASIS,                       // B-Spline, minimum 4 points
    SPLINE_CATMULL_ROM,                 // Catmull-Rom spline, minimum 4 points
    SPLINE_BEZIER_QUAD,                 // Quadratic Bezier spline, minimum 3 points (1 control point): [p1, c2, p3, c4...]
    SPLINE_BEZIER_CUBIC                 // Cubic Bezier spline, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
} SplineType;

// Spline cap type
typedef enum {
    SPLINE_CAP_FLAT = 0,                // Flat spline cap, endpoint is cut off with a hard edge perpendicular to the curve, ignores capSize
    SPLINE_CAP_SQUARE,                  // Square spline cap, endpoint is extended with a rectangle parallel to the along the curve's tangent with a length of capSize
    SPLINE_CAP_SHARP,                   // Sharp spline cap, endpoint is extended with a triangle triangle parallel to the cureve's tangent with a length of capSize
    SPLINE_CAP_ROUND                    // Round spline cap, endpoint is extended with a half-oval parallel to the curve's tangent with a 2nd radius of capSize
} SplineCap;

// Spline elbow type
typedef enum {
    SPLINE_ELBOW_SPLIT = 0,             // Split spline elbow, patches are not joined
    SPLINE_ELBOW_BEVEL,                 // Bevel spline elbow, patches are joined by bridging their endpoints
    SPLINE_ELBOW_SHARP,                 // Sharp spline elbow, patches are joined by extending their endpoints until they meet (with a distance no greater than maxSharpness)
    SPLINE_ELBOW_ROUND                  // Round spline elbow, patches are joined with circles
} SplineElbow;

#if defined(RLSPLINES_3D)
// Spline gradient align
typedef enum {
    SPLINE_PROFILE_FLAT = 0,            // Gradient within spline, mask a world-aligned gradient with the spline
    SPLINE_PROFILE_TRIANGLE,            // Gradient along spline, gradient transitions parallel/perpendicular along the curve
    SPLINE_PROFILE_RECTANGLE,           // Gradient along spline, gradient transitions parallel/perpendicular along the curve
    SPLINE_PROFILE_CIRCLE               // Gradient along spline, gradient transitions parallel/perpendicular along the curve
} SplineProfile;
#endif      // RLSPLINES_3D

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

// Splines constructor functions
// NOTE: Points and controls will be copied, the arrays passed to these functions will NOT be stored in the returned Spline.
RLSPLINESAPI Spline GenSpline(int type, const Vector2 *points, const Vector2 *controls, int pointCount); // Generate arbitrary spline, number of controls depends on type
RLSPLINESAPI Spline GenSplineLinear(const Vector2 *points, int pointCount);             // Generate spline: Linear, minimum 2 points
RLSPLINESAPI Spline GenSplineBasis(const Vector2 *points, int pointCount);              // Generate spline: B-Spline, minimum 4 points
RLSPLINESAPI Spline GenSplineCatmullRom(const Vector2 *points, int pointCount);         // Generate spline: Catmull-Rom, minimum 4 points
RLSPLINESAPI Spline GenSplineBezierQuad(const Vector2 *points, const Vector2 *controls, int pointCount); // Generate spline: Quadratic Bezier, minimum 3 points (1 control point): [p1, c2, p3, c4...]
RLSPLINESAPI Spline GenSplineBezierCubic(const Vector2 *points, const Vector2 *controls, int pointCount); // Generate spline: Cubic Bezier, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
RLSPLINESAPI void UnloadSpline(Spline spline);                                          // Unload spline from CPU memory (RAM)

RLSPLINESAPI Spline GetSplinePatchCount(int type, int pointCount);                      // Get (calculate) number of patches (sub-splines) in a Spline

// Splines drawing functions
RLSPLINESAPI void DrawSpline(Spline spline, Color color);
RLSPLINESAPI void DrawSplineEx(Spline spline, float thick, Color color);
RLSPLINESAPI void DrawSplineVar(Spline spline, SplineThickness thick, Color color);
RLSPLINESAPI void DrawSplinePro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, Color color);
#if defined(RLSPLINES_ULTIMATE)
RLSPLINESAPI void DrawSplineUlt(
    Spline spline,
    SplineThickness thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline patches (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    Color color
);
#endif      // RLSPLINES_ULTIMATE

#if defined(RLSPLINES_GRADIENT)

// Gradient splines drawing functions
RLSPLINESAPI void DrawSplineGradient(Spline spline, SplineGradient colors);
RLSPLINESAPI void DrawSplineGradientEx(Spline spline, float thick, SplineGradient colors);
RLSPLINESAPI void DrawSplineGradientVar(Spline spline, SplineThickness thick, SplineGradient colors);
RLSPLINESAPI void DrawSplineGradientPro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient colors);
#if defined(RLSPLINES_ULTIMATE)
RLSPLINESAPI void DrawSplineGradientUlt(
    Spline spline,
    SplineThickness thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline patches (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    SplineGradient colors
);
#endif      // RLSPLINES_ULTIMATE

// 2D Gradient splines drawing functions
RLSPLINESAPI void DrawSplineGradient2Ex(Spline spline, float thick, SplineGradient2D colors);
RLSPLINESAPI void DrawSplineGradient2Var(Spline spline, SplineThickness thick, SplineGradient2D colors);
RLSPLINESAPI void DrawSplineGradient2Pro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient2D colors);
#if defined(RLSPLINES_ULTIMATE)
RLSPLINESAPI void DrawSplineGradient2Ult(
    Spline spline,
    SplineThickness thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline patches (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    SplineGradient2D colors
);
#endif      // RLSPLINES_ULTIMATE

#endif      // RLSPLINES_GRADIENT

#if defined(RLSPLINES_3D)

// 3D Splines drawing functions
RLSPLINESAPI void DrawSpline3D(Spline3D spline, Color color);
RLSPLINESAPI void DrawSpline3DEx(Spline3D spline, float thick, Color color);
RLSPLINESAPI void DrawSpline3DVar(Spline3D spline, SplineThickness2D thick, Color color);
RLSPLINESAPI void DrawSpline3DPro(Spline3D spline, SplineThickness2D thick, float capSize, SplineElbow elbow, float maxSharpness, Color color);
#if defined(RLSPLINES_ULTIMATE)
RLSPLINESAPI void DrawSpline3DUlt(
    Spline3D spline,
    SplineThickness2D thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline patches (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    Color color
);
#endif      // RLSPLINES_ULTIMATE

#if defined(RLSPLINES_GRADIENT)

// Gradient 3D splines drawing functions
RLSPLINESAPI void DrawSpline3DGradient(Spline3D spline, SplineGradient colors);
RLSPLINESAPI void DrawSpline3DGradientEx(Spline3D spline, float thick, SplineGradient colors);
RLSPLINESAPI void DrawSpline3DGradientVar(Spline3D spline, SplineThickness2D thick, SplineGradient colors);
RLSPLINESAPI void DrawSpline3DGradientPro(Spline3D spline, SplineThickness2D thick, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient colors);
#if defined(RLSPLINES_ULTIMATE)
RLSPLINESAPI void DrawSpline3DGradientUlt(
    Spline3D spline,
    SplineThickness2D thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline patches (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    SplineGradient colors
);
#endif      // RLSPLINES_ULTIMATE

// 2D Gradient 3D splines drawing functions
RLSPLINESAPI void DrawSpline3DGradient2Ex(Spline3D spline, float thick, SplineGradient colors);
RLSPLINESAPI void DrawSpline3DGradient2Var(Spline3D spline, SplineThickness2D thick, SplineGradient colors);
RLSPLINESAPI void DrawSpline3DGradient2Pro(Spline3D spline, SplineThickness2D thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient colors);
#if defined(RLSPLINES_ULTIMATE)
RLSPLINESAPI void DrawSpline3DGradient2Ult(
    Spline3D spline,
    SplineThickness2D thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline patches (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    SplineGradient colors
);
#endif      // RLSPLINES_ULTIMATE

#endif      // RLSPLINES_GRADIENT

#endif      // RLSPLINES_3D

// Splines outline functions
RLSPLINESAPI Spline SplineOutlineEx(Spline spline, float thick);                // Calculate spline outlines as a new spline
RLSPLINESAPI Spline SplineOutlineVar(Spline spline, SplineThickness thick);     // Calculate spline outlines as a new spline
RLSPLINESAPI Spline SplineOutlinePro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness); // Calculate spline outlines as a new spline
#if defined(RLSPLINES_ULTIMATE)
RLSPLINESAPI Spline SplineOutlineUlt(                                           // Calculate spline outlines as a new spline
    Spline3D spline,
    SplineThickness2D thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline patches (total points (excluding control points) minus 1)
    const float maxSharpnesses          // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
);
#endif      // RLSPLINES_ULTIMATE

#if defined(RLSPLINES_3D)

// 3D Splines outline functions
RLSPLINESAPI Spline Spline3DOutlineEx(Spline spline, float thick);              // Calculate spline outlines as a new spline
RLSPLINESAPI Spline Spline3DOutlineVar(Spline spline, SplineThickness thick);   // Calculate spline outlines as a new spline
RLSPLINESAPI Spline Spline3DOutlinePro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness); // Calculate spline outlines as a new spline
#if defined(RLSPLINES_ULTIMATE)
RLSPLINESAPI Spline Spline3DOutlineUlt(                                         // Calculate spline outlines as a new spline
    Spline3D spline,
    SplineThickness2D thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline patches (total points (excluding control points) minus 1)
    const float maxSharpnesses          // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
);
#endif      // RLSPLINES_ULTIMATE

#endif      // RLSPLINES_3D

#if defined(RLSPLINES_DIMENSION_ALIASES)

// Spline2D, same as Spline
typedef Spline Spline2D;

// SplineThickness1D, same as SplineThickness
typedef SplineThickness SplineThickness1D;

// SplineGradient1D, same as SplineGradient
typedef SplineGradient SplineGradient1D;

#define DrawSpline2D DrawSpline                                 // Alias for DrawSpline
#define DrawSpline2DEx DrawSplineEx                             // Alias for DrawSplineEx
#define DrawSpline2DVar DrawSplineVar                           // Alias for DrawSplineVar
#define DrawSpline2DPro DrawSplinePro                           // Alias for DrawSplinePro
#define DrawSpline2DUlt DrawSplineUlt                           // Alias for DrawSplineUlt
#define Spline2DOutlineEx SplineOutlineEx                       // Alias for SplineOutlineEx
#define Spline2DOutlineVar SplineOutlineVar                     // Alias for SplineOutlineVar
#define Spline2DOutlinePro SplineOutlinePro                     // Alias for SplineOutlinePro
#define Spline2DOutlineUlt SplineOutlineUlt                     // Alias for SplineOutlineUlt

#if defined(RLSPLINES_GRADIENT)

#define DrawSpline2DGradient DrawSplineGradient                 // Alias for DrawSplineGradient
#define DrawSpline2DGradientEx DrawSplineGradientEx             // Alias for DrawSplineGradientEx
#define DrawSpline2DGradientVar DrawSplineGradientVar           // Alias for DrawSplineGradientVar
#define DrawSpline2DGradientPro DrawSplineGradientPro           // Alias for DrawSplineGradientPro
#define DrawSpline2DGradientUlt DrawSplineGradientUlt           // Alias for DrawSplineGradientUlt
#define DrawSpline2DGradient1 DrawSplineGradient                // Alias for DrawSplineGradient
#define DrawSpline2DGradient1Ex DrawSplineGradientEx            // Alias for DrawSplineGradientEx
#define DrawSpline2DGradient1Var DrawSplineGradientVar          // Alias for DrawSplineGradientVar
#define DrawSpline2DGradient1Pro DrawSplineGradientPro          // Alias for DrawSplineGradientPro
#define DrawSpline2DGradient1Ult DrawSplineGradientUlt          // Alias for DrawSplineGradientUlt
#define DrawSpline2DGradient2Ex DrawSplineGradient2Ex           // Alias for DrawSplineGradient2Ex
#define DrawSpline2DGradient2Var DrawSplineGradient2Var         // Alias for DrawSplineGradient2Var
#define DrawSpline2DGradient2Pro DrawSplineGradient2Pro         // Alias for DrawSplineGradient2Pro
#define DrawSpline2DGradient2Ult DrawSplineGradient2Ult         // Alias for DrawSplineGradient2Ult

#if defined(RLSPLINES_3D)
#define DrawSpline3DGradient1 DrawSpline3DGradient              // Alias for DrawSpline3DGradient
#define DrawSpline3DGradient1Ex DrawSpline3DGradientEx          // Alias for DrawSpline3DGradientEx
#define DrawSpline3DGradient1Var DrawSpline3DGradientVar        // Alias for DrawSpline3DGradientVar
#define DrawSpline3DGradient1Pro DrawSpline3DGradientPro        // Alias for DrawSpline3DGradientPro
#define DrawSpline3DGradient1Ult DrawSpline3DGradientUlt        // Alias for DrawSpline3DGradientUlt
#endif      // RLSPLINES_3D

#endif      // RLSPLINES_GRADIENT

#endif      // RLSPLINES_DIMENSION_ALIASES

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

// Generate arbitrary spline, number of controls depends on type
RLSPLINESAPI Spline GenSpline(int type, const Vector2 *points, const Vector2 *controls, int pointCount)
{
    Spline spline = { 0 };
    spline.type = type;

    if ((type == SPLINE_LINEAR) || (type == SPLINE_BASIS) || (type == SPLINE_CATMULL_ROM))
    {
        if (((type == SPLINE_LINEAR) && (pointCount >= 2)) ||
            ((type == SPLINE_BASIS) && (pointCount >= 4)) ||
            ((type == SPLINE_CATMULL_ROM) && (pointCount >= 4)))
        {
            spline.pointCount = pointCount;
            spline.points = (float *)RLSPLINES_MALLOC(pointCount*2*sizeof(float));
            for (int i = 0; i < pointCount; ++i)
            {
                spline.points[i*2 + 0] = points[i].x;
                spline.points[i*2 + 1] = points[i].y;
            }
        }
        else RLSPLINES_LOG("ERROR: [RLSPLINES]: Insufficient points (%i) for spline type (%i)", pointCount, type);
    }
    // else if (type == SPLINE_BEZIER_QUAD)
    // {
    //     if (pointCount >= 3)
    //     {
    //         spline.pointCount = pointCount;
    //         spline.points = (float *)RLSPLINES_MALLOC((pointCount + pointCount / 2)*2*sizeof(float));
    //         for (int i = 0; i < pointCount; ++i)
    //         {
    //             spline.points[i*4 + 0] = points[i].x;
    //             spline.points[i*4 + 1] = points[i].y;
    //         }
    //         for (int i = 0; i < pointCount - 1; ++i)
    //         {
    //             spline.points[i*4 + 2] = controls[i].x;
    //             spline.points[i*4 + 3] = controls[i].y;
    //         }
    //     }
    //     else RLSPLINES_LOG("ERROR: [RLSPLINES]: Insufficient points (%i) for spline type (%i)", pointCount, type);
    // }
    // else if (type == SPLINE_BEZIER_CUBIC)
    // {
    //     if (pointCount >= 3)
    //     {
    //         spline.pointCount = pointCount;
    //         spline.points = (float *)RLSPLINES_MALLOC((pointCount*3 - 2)*2*sizeof(float));
    //         for (int i = 0; i < pointCount; ++i)
    //         {
    //             spline.points[i*4 + 0] = points[i].x;
    //             spline.points[i*4 + 1] = points[i].y;
    //         }
    //         for (int i = 0; i < pointCount - 1; ++i)
    //         {
    //             spline.points[i*4 + 2] = controls[i].x;
    //             spline.points[i*4 + 3] = controls[i].y;
    //         }
    //     }
    //     else RLSPLINES_LOG("ERROR: [RLSPLINES]: Insufficient points (%i) for spline type (%i)", pointCount, type);
    // }
    else RLSPLINES_LOG("ERROR: [RLSPLINES]: Unsupported spline type (%i)", type);

    return spline;
}

// Generate spline: Linear, minimum 2 points
RLSPLINESAPI Spline GenSplineLinear(const Vector2 *points, int pointCount)
{

}

// Generate spline: B-Spline, minimum 4 points
RLSPLINESAPI Spline GenSplineBasis(const Vector2 *points, int pointCount)
{

}

// Generate spline: Catmull-Rom, minimum 4 points
RLSPLINESAPI Spline GenSplineCatmullRom(const Vector2 *points, int pointCount)
{

}

// Generate spline: Quadratic Bezier, minimum 3 points (1 control point): [p1, c2, p3, c4...]
RLSPLINESAPI Spline GenSplineBezierQuad(const Vector2 *points, const Vector2 *controls, int pointCount)
{

}

// Generate spline: Cubic Bezier, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
RLSPLINESAPI Spline GenSplineBezierCubic(const Vector2 *points, const Vector2 *controls, int pointCount)
{

}

// Unload spline from CPU memory (RAM)
RLSPLINESAPI void UnloadSpline(Spline spline)
{

}

// Get (calculate) number of patches (sub-splines) in a Spline
RLSPLINESAPI Spline GetSplinePatchCount(int type, int pointCount)
{

}

#if defined(RLSPLINES_STANDALONE)
#endif      // RLSPLINES_STANDALONE

#endif      // RLSPLINES_IMPLEMENTATION
