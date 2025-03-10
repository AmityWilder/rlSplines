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
*       #define RLSPLINES_3D
*           Include support for 3D splines.
*
*       #define RLSPLINES_GRADIENT
*           Include support for gradient spline drawing.
*
*       #define RLSPLINES_ULTRA
*           Include support for "ultra" (`_Ult()`) spline functions (lots of parameters).
*
*       #define RLSPLINES_DIMENSION_ALIASES
*           Define aliases with "1/2D" in the name for unspecified spline types/functions, to
*           help distinguish from their 2/3D counterparts, for consistency, or if you just prefer
*           having that additional specificity.
*
*   VERSIONS HISTORY:
*       [TODO]
*
*   DEPENDENCIES:
*       math.h  - Math
*       raylib 5.0  - Shapes drawing
*
*   CONTRIBUTORS:
*       Amy Wilder:         Initial implementation of basic components (2025)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2025 Amy Wilder (@AmityWilder)
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
#define RLSPLINES_VERSION_SEGMENT 0
#define RLSPLINES_VERSION  "0.1"

#include "raylib.h"
#include "rlgl.h"
#include "math.h"

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

#undef RLSPLINES_3D             // Under construction
#undef RLSPLINES_GRADIENT       // Under construction

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Allow custom memory allocators
#ifndef RLSPLINES_MALLOC
    #include <malloc.h>
    #define RLSPLINES_MALLOC(sz)       malloc(sz)
#endif
#ifndef RLSPLINES_CALLOC
    #include <malloc.h>
    #define RLSPLINES_CALLOC(n,sz)     calloc(n,sz)
#endif
#ifndef RLSPLINES_FREE
    #include <malloc.h>
    #define RLSPLINES_FREE(p)          free(p)
#endif

// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
//#define RLSPLINES_SUPPORT_TRACELOG
#ifdef RLSPLINES_SUPPORT_TRACELOG
  #define TRACELOG(level, ...)           TraceLog(level, __VA_ARGS__)
#else
  #define TRACELOG(level, ...)           (void)0
#endif

#ifndef SPLINE_SEGMENT_DIVISIONS
    #define SPLINE_SEGMENT_DIVISIONS        24      // Spline segments subdivisions
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
#ifndef __cplusplus
// Boolean type
    #ifndef true
        typedef enum { false, true } bool;
    #endif
#endif

// Spline, 2D spline point data
typedef struct Spline {
    int type;                           // Spline type: SPLINE_LINEAR, SPLINE_BASIS, SPLINE_CATMULL_ROM, SPLINE_BEZIER_QUAD, or SPLINE_BEZIER_CUBIC
    int pointCount;                     // Number of points and control points (spline type controls ratio and order)
    float *points;                      // Points and control points in spline (XY - 2 components per vertex), minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
} Spline;

#ifdef RLSPLINES_3D
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

#ifdef RLSPLINES_3D
// 2D Spline thickness, variable thickness data in the form of a 2D Cubic Bezier spline with remapped timings (t-values) along the curve of another spline
// NOTE: Only valid for a 3D spline, a 2D spline only has 1 perpendicular axis
typedef struct SplineThickness2D {
    int thicknessCount;                 // Number of thicknesses
    float *timings;                     // Thickness timings (t-values), minimum 4 values (2 control values): [p1t, c2t, c3t, p4t, c5t, c6t...], or NULL if evenly spread
    Vector2 *thicknesses;               // Thicknesses, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
} SplineThickness2D;
#endif      // RLSPLINES_3D

#ifdef RLSPLINES_GRADIENT

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

#ifdef RLSPLINES_3D
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
    SPLINE_ELBOW_SPLIT = 0,             // Split spline elbow, segments are not joined
    SPLINE_ELBOW_BEVEL,                 // Bevel spline elbow, segments are joined by bridging their endpoints
    SPLINE_ELBOW_SHARP,                 // Sharp spline elbow, segments are joined by extending their endpoints until they meet (with a distance no greater than maxSharpness)
    SPLINE_ELBOW_ROUND                  // Round spline elbow, segments are joined with circles
} SplineElbow;

#ifdef RLSPLINES_3D
// Spline profile
typedef enum {
    SPLINE_PROFILE_FLAT = 0,            // Flat spline profile, a plane facing perpendicular to the curve tangent
    SPLINE_PROFILE_TRIANGLE,            // Triangle spline profile, a triangle facing parallel with the curve tangent
    SPLINE_PROFILE_RECTANGLE,           // Triangle spline profile, a rectangle facing parallel with the curve tangent
    SPLINE_PROFILE_CIRCLE               // Triangle spline profile, a circle facing parallel with the curve tangent
} SplineProfile;
#endif      // RLSPLINES_3D

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

// Splines constructor functions
// NOTE: Points and controls will be copied, the arrays passed to these functions will NOT be stored in the returned Spline.
RLSPLINESAPI Spline GenSplineLinear(const Vector2 *points, int pointCount);             // Generate spline: Linear, minimum 2 points
RLSPLINESAPI Spline GenSplineBasis(const Vector2 *points, int pointCount);              // Generate spline: B-Spline, minimum 4 points
RLSPLINESAPI Spline GenSplineCatmullRom(const Vector2 *points, int pointCount);         // Generate spline: Catmull-Rom, minimum 4 points
RLSPLINESAPI Spline GenSplineBezierQuad(const Vector2 *anchorPoints, const Vector2 *controlPoints, int anchorCount); // Generate spline: Quadratic Bezier, minimum 3 points (1 control point)
RLSPLINESAPI Spline GenSplineBezierCubic(const Vector2 *anchorPoints, const Vector2 *controlOutPoints, const Vector2 *controlInPoints, int anchorCount); // Generate spline: Cubic Bezier, minimum 4 points (2 control points)
RLSPLINESAPI void UnloadSpline(Spline spline);                                          // Unload spline from CPU memory (RAM)

RLSPLINESAPI Spline SplineJoin(Spline spline1, Spline spline2);                         // Create a new spline joining two splines of the same type
RLSPLINESAPI Spline SplineJoinMany(const Spline *spline, int splineCount);              // Create a new spline joining multiple splines of the same type

RLSPLINESAPI int GetSplineSegmentCount(int type, int pointCount);                       // Get (calculate) number of segments in a spline

// Splines point access functions

RLSPLINESAPI Vector2 GetSplinePoint(Spline spline, int index);                          // Get position of spline point
RLSPLINESAPI Vector2 GetSplineAnchor(Spline spline, int index);                         // Get position of spline anchor point
RLSPLINESAPI Vector2 GetSplineControl(Spline spline, int index);                        // Get position of quadratic bezier spline control point
RLSPLINESAPI Vector2 GetSplineControlIn(Spline spline, int index);                      // Get position of cubic bezier spline in-control point
RLSPLINESAPI Vector2 GetSplineControlOut(Spline spline, int index);                     // Get position of cubic bezier spline out-control point

RLSPLINESAPI void SetSplinePoint(Spline spline, int index, Vector2 position);           // Set position of spline point
RLSPLINESAPI void SetSplineAnchor(Spline spline, int index, Vector2 position);          // Set position of spline anchor point
RLSPLINESAPI void SetSplineControl(Spline spline, int index, Vector2 position);         // Set position of quadratic bezier spline control point
RLSPLINESAPI void SetSplineControlIn(Spline spline, int index, Vector2 position);       // Set position of cubic bezier spline in-control point
RLSPLINESAPI void SetSplineControlOut(Spline spline, int index, Vector2 position);      // Set position of cubic bezier spline out-control point

// Splines drawing functions
RLSPLINESAPI void DrawSpline(Spline spline, Color color);                               // Draw a 2D spline using lines
RLSPLINESAPI void DrawSplineEx(Spline spline, float thick, Color color);                // Draw a 2D spline with thickness
RLSPLINESAPI void DrawSplineVar(Spline spline, SplineThickness thick, Color color);     // Draw a 2D spline with variable thickness
RLSPLINESAPI void DrawSplinePro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, Color color); // Draw a 2D spline with 'pro' parameters
#ifdef RLSPLINES_ULTRA
RLSPLINESAPI void DrawSplineUlt(                                                        // Draw a 2D spline with ultra parameters
    Spline spline,
    SplineThickness thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    Color color
);
#endif      // RLSPLINES_ULTRA

#ifdef RLSPLINES_GRADIENT

// Gradient splines drawing functions
RLSPLINESAPI void DrawSplineGradient(Spline spline, SplineGradient colors);
RLSPLINESAPI void DrawSplineGradientEx(Spline spline, float thick, SplineGradient colors);
RLSPLINESAPI void DrawSplineGradientVar(Spline spline, SplineThickness thick, SplineGradient colors);
RLSPLINESAPI void DrawSplineGradientPro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient colors);
#ifdef RLSPLINES_ULTRA
RLSPLINESAPI void DrawSplineGradientUlt(
    Spline spline,
    SplineThickness thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    SplineGradient colors
);
#endif      // RLSPLINES_ULTRA

// 2D Gradient splines drawing functions
RLSPLINESAPI void DrawSplineGradient2Ex(Spline spline, float thick, SplineGradient2D colors);
RLSPLINESAPI void DrawSplineGradient2Var(Spline spline, SplineThickness thick, SplineGradient2D colors);
RLSPLINESAPI void DrawSplineGradient2Pro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient2D colors);
#ifdef RLSPLINES_ULTRA
RLSPLINESAPI void DrawSplineGradient2Ult(
    Spline spline,
    SplineThickness thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    SplineGradient2D colors
);
#endif      // RLSPLINES_ULTRA

#endif      // RLSPLINES_GRADIENT

#ifdef RLSPLINES_3D

// 3D Splines drawing functions
RLSPLINESAPI void DrawSpline3D(Spline3D spline, Color color);
RLSPLINESAPI void DrawSpline3DEx(Spline3D spline, float thick, Color color);
RLSPLINESAPI void DrawSpline3DVar(Spline3D spline, SplineThickness2D thick, Color color);
RLSPLINESAPI void DrawSpline3DPro(Spline3D spline, SplineThickness2D thick, float capSize, SplineElbow elbow, float maxSharpness, Color color);
#ifdef RLSPLINES_ULTRA
RLSPLINESAPI void DrawSpline3DUlt(
    Spline3D spline,
    SplineThickness2D thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    Color color
);
#endif      // RLSPLINES_ULTRA

#ifdef RLSPLINES_GRADIENT

// Gradient 3D splines drawing functions
RLSPLINESAPI void DrawSpline3DGradient(Spline3D spline, SplineGradient colors);
RLSPLINESAPI void DrawSpline3DGradientEx(Spline3D spline, float thick, SplineGradient colors);
RLSPLINESAPI void DrawSpline3DGradientVar(Spline3D spline, SplineThickness2D thick, SplineGradient colors);
RLSPLINESAPI void DrawSpline3DGradientPro(Spline3D spline, SplineThickness2D thick, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient colors);
#ifdef RLSPLINES_ULTRA
RLSPLINESAPI void DrawSpline3DGradientUlt(
    Spline3D spline,
    SplineThickness2D thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    SplineGradient colors
);
#endif      // RLSPLINES_ULTRA

// 2D Gradient 3D splines drawing functions
RLSPLINESAPI void DrawSpline3DGradient2Ex(Spline3D spline, float thick, SplineGradient colors);
RLSPLINESAPI void DrawSpline3DGradient2Var(Spline3D spline, SplineThickness2D thick, SplineGradient colors);
RLSPLINESAPI void DrawSpline3DGradient2Pro(Spline3D spline, SplineThickness2D thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient colors);
#ifdef RLSPLINES_ULTRA
RLSPLINESAPI void DrawSpline3DGradient2Ult(
    Spline3D spline,
    SplineThickness2D thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
    const float maxSharpnesses,         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    SplineGradient colors
);
#endif      // RLSPLINES_ULTRA

#endif      // RLSPLINES_GRADIENT

#endif      // RLSPLINES_3D

// Splines outline functions
RLSPLINESAPI Spline SplineOutlineEx(Spline spline, float thick);                // Calculate spline outlines as a new spline
RLSPLINESAPI Spline SplineOutlineVar(Spline spline, SplineThickness thick);     // Calculate spline outlines as a new spline
RLSPLINESAPI Spline SplineOutlinePro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness); // Calculate spline outlines as a new spline
#ifdef RLSPLINES_ULTRA
RLSPLINESAPI Spline SplineOutlineUlt(                                           // Calculate spline outlines as a new spline
    Spline spline,
    SplineThickness thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
    const float maxSharpnesses          // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
);
#endif      // RLSPLINES_ULTRA

#ifdef RLSPLINES_3D

// 3D Splines outline functions
RLSPLINESAPI Spline Spline3DOutlineEx(Spline spline, float thick);              // Calculate spline outlines as a new spline
RLSPLINESAPI Spline Spline3DOutlineVar(Spline spline, SplineThickness thick);   // Calculate spline outlines as a new spline
RLSPLINESAPI Spline Spline3DOutlinePro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness); // Calculate spline outlines as a new spline
#ifdef RLSPLINES_ULTRA
RLSPLINESAPI Spline Spline3DOutlineUlt(                                         // Calculate spline outlines as a new spline
    Spline3D spline,
    SplineThickness2D thick,
    SplineCap startCap,
    float startCapSize,
    SplineCap endCap,
    float endCapSize,
    const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
    const float maxSharpnesses          // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
);
#endif      // RLSPLINES_ULTRA

#endif      // RLSPLINES_3D

#ifdef RLSPLINES_DIMENSION_ALIASES

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

#ifdef RLSPLINES_GRADIENT

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

#ifdef RLSPLINES_3D
#define DrawSpline3DGradient1 DrawSpline3DGradient              // Alias for DrawSpline3DGradient
#define DrawSpline3DGradient1Ex DrawSpline3DGradientEx          // Alias for DrawSpline3DGradientEx
#define DrawSpline3DGradient1Var DrawSpline3DGradientVar        // Alias for DrawSpline3DGradientVar
#define DrawSpline3DGradient1Pro DrawSpline3DGradientPro        // Alias for DrawSpline3DGradientPro
#define DrawSpline3DGradient1Ult DrawSpline3DGradientUlt        // Alias for DrawSpline3DGradientUlt
#endif      // RLSPLINES_3D

#endif      // RLSPLINES_GRADIENT

#endif      // RLSPLINES_DIMENSION_ALIASES

#ifdef __cplusplus
}            // Prevents name mangling of functions
#endif

#endif      // RLSPLINES_H

/***********************************************************************************
*
*   RLSPLINES IMPLEMENTATION
*
************************************************************************************/

#ifdef RLSPLINES_IMPLEMENTATION

#ifdef __cplusplus
    #define RLSPLINES_CLITERAL(name) name
#else
    #define RLSPLINES_CLITERAL(name) (name)
#endif

// Generate spline: Linear, minimum 2 points
// Use NULL for points to get a spline with uninitialized points.
RLSPLINESAPI Spline GenSplineLinear(const Vector2 *points, int pointCount)
{
    Spline spline = { 0 };
    spline.type = SPLINE_LINEAR;

    if (pointCount >= 2)
    {
        spline.pointCount = pointCount;
        spline.points = (float *)RLSPLINES_MALLOC(spline.pointCount*sizeof(float)*2);
        if (points != NULL)
        {
            for (int i = 0, j = 0; i < pointCount; ++i)
            {
                spline.points[j++] = points[i].x;
                spline.points[j++] = points[i].y;
            }
        }
    }
    else TRACELOG(LOG_WARNING, "RLSPLINES: Linear spline requires minimum 2 points");

    return spline;
}

// Generate spline: B-Spline, minimum 4 points
// Use NULL for points to get a spline with uninitialized points.
RLSPLINESAPI Spline GenSplineBasis(const Vector2 *points, int pointCount)
{
    Spline spline = { 0 };
    spline.type = SPLINE_BASIS;

    if (pointCount >= 4)
    {
        spline.pointCount = pointCount;
        spline.points = (float *)RLSPLINES_MALLOC(spline.pointCount*sizeof(float)*2);
        if (points != NULL)
        {
            for (int i = 0, j = 0; i < pointCount; ++i)
            {
                spline.points[j++] = points[i].x;
                spline.points[j++] = points[i].y;
            }
        }
    }
    else TRACELOG(LOG_WARNING, "RLSPLINES: Basis spline requires minimum 4 points");

    return spline;
}

// Generate spline: Catmull-Rom, minimum 4 points
// Use NULL for points to get a spline with uninitialized points.
RLSPLINESAPI Spline GenSplineCatmullRom(const Vector2 *points, int pointCount)
{
    Spline spline = { 0 };
    spline.type = SPLINE_CATMULL_ROM;

    if (pointCount >= 4)
    {
        spline.pointCount = pointCount;
        spline.points = (float *)RLSPLINES_MALLOC(spline.pointCount*sizeof(float)*2);
        if (points != NULL)
        {
            for (int i = 0, j = 0; i < pointCount; ++i)
            {
                spline.points[j++] = points[i].x;
                spline.points[j++] = points[i].y;
            }
        }
    }
    else TRACELOG(LOG_WARNING, "RLSPLINES: Catmull-Rom spline requires minimum 4 points");

    return spline;
}

// Generate spline: Quadratic Bezier, minimum 3 points (1 control point): [p1, p3...], [c2, c4...]
// Use NULL for anchorPoints to get a spline with uninitialized points.
// NOTE: controlPoints must have anchorPointCount - 1 elements.
RLSPLINESAPI Spline GenSplineBezierQuad(const Vector2 *anchorPoints, const Vector2 *controlPoints, int anchorPointCount)
{
    Spline spline = { 0 };
    spline.type = SPLINE_BEZIER_QUAD;

    if (anchorPointCount >= 2)
    {
        spline.pointCount = 2*anchorPointCount - 1;
        spline.points = (float *)RLSPLINES_MALLOC(spline.pointCount*sizeof(float)*2);
        if ((anchorPoints != NULL) && (controlPoints != NULL))
        {
            for (int i = 0, j = 0; i < anchorPointCount; ++i)
            {
                spline.points[j++] = anchorPoints[i].x;
                spline.points[j++] = anchorPoints[i].y;
                if (i < (anchorPointCount - 1))
                {
                    spline.points[j++] = controlPoints[i].x;
                    spline.points[j++] = controlPoints[i].y;
                }
            }
        }
    }
    else TRACELOG(LOG_WARNING, "RLSPLINES: Quadratic bezier spline requires minimum 2 points + 1 control point");

    return spline;
}

// Generate spline: Cubic Bezier, minimum 4 points (2 control points): [p1, p4...], [c2, c5...], [c3, c6...]
// Use NULL for anchorPoints to get a spline with uninitialized points.
// NOTE: controlOutPoints and controlInPoints must have anchorPointCount - 1 elements each.
RLSPLINESAPI Spline GenSplineBezierCubic(const Vector2 *anchorPoints, const Vector2 *controlOutPoints, const Vector2 *controlInPoints, int anchorPointCount)
{
    Spline spline = { 0 };
    spline.type = SPLINE_BEZIER_CUBIC;

    if (anchorPointCount >= 2)
    {
        spline.pointCount = 3*anchorPointCount - 2;
        spline.points = (float *)RLSPLINES_MALLOC(spline.pointCount*sizeof(float)*2);
        if ((anchorPoints != NULL) && (controlOutPoints != NULL) && (controlInPoints != NULL))
        {
            for (int i = 0, j = 0; i < anchorPointCount; ++i)
            {
                spline.points[j++] = anchorPoints[i].x;
                spline.points[j++] = anchorPoints[i].y;
                if (i < (anchorPointCount - 1))
                {
                    spline.points[j++] = controlOutPoints[i].x;
                    spline.points[j++] = controlOutPoints[i].y;
                    spline.points[j++] = controlInPoints[i].x;
                    spline.points[j++] = controlInPoints[i].y;
                }
            }
        }
    }
    else TRACELOG(LOG_WARNING, "RLSPLINES: Cubic bezier spline requires minimum 2 points + 2 control points");

    return spline;
}

// Unload spline from CPU memory (RAM)
RLSPLINESAPI void UnloadSpline(Spline spline)
{
    RLSPLINES_FREE(spline.points);
}

// Create a new spline joining two separate splines of the same type
// NOTE: Does not unload input splines
RLSPLINESAPI Spline SplineJoin(Spline spline1, Spline spline2)
{
    Spline joined = { 0 };

    if ((spline1.type == spline2.type) && (spline1.points != NULL) && (spline2.points != NULL))
    {
        joined.type = spline1.type;
        joined.pointCount = spline1.pointCount + spline2.pointCount;
        joined.points = (float *)RLSPLINES_MALLOC(joined.pointCount*sizeof(float)*2);
        int j = 0;
        if (spline1.points != NULL)
        {
            for (int k = 0; k < 2*spline1.pointCount; ++k)
            {
                joined.points[j++] = spline1.points[k];
            }
        }
        if (spline2.points != NULL)
        {
            for (int k = 0; k < 2*spline1.pointCount; ++k)
            {
                joined.points[j++] = spline1.points[k];
            }
        }
    }
    else TRACELOG(LOG_WARNING, "RLSPLINES: SplineJoin requires spline1 and spline2 to be the same type");

    return joined;
}

// Create a new spline joining multiple splines of the same type
// NOTE: Does not unload input splines
RLSPLINESAPI Spline SplineJoinMany(const Spline *splines, int splineCount)
{
    Spline joined = { 0 };

    if ((splines != NULL) && (splineCount > 1))
    {
        joined.type = splines[0].type;
        for (int i = 1; i < splineCount; ++i)
        {
            if ((splines[i].type == splines[0].type) && (splines[i].points != NULL))
            {
                joined.pointCount += splines[i].pointCount;
            }
            else
            {
                TRACELOG(LOG_WARNING, "RLSPLINES: SplineJoinMany requires every spline to be the same type and not NULL");
                return RLSPLINES_CLITERAL(Spline){ 0 };
            }
        }

        joined.points = (float *)RLSPLINES_MALLOC(joined.pointCount*sizeof(float)*2);
        for (int i = 0, j = 0; i < splineCount; ++i)
        {
            for (int k = 0; k < 2*splines[i].pointCount; ++k)
            {
                joined.points[j++] = splines[i].points[k];
            }
        }
    }

    return joined;
}

// Get (calculate) number of segments (sub-splines) in a Spline, equal to the number of points (excluding control points) minus 1
RLSPLINESAPI int GetSplineSegmentCount(int type, int pointCount)
{
    int segmentCount = -1;

    if (type == SPLINE_LINEAR) segmentCount = pointCount - 1;
    else if (type == SPLINE_BASIS) segmentCount = pointCount - 1;               // TODO: Confirm segments work like this in B-splines
    else if (type == SPLINE_CATMULL_ROM) segmentCount = pointCount - 1;         // TODO: Confirm segments work like this in Catmull-Rom splines
    else if (type == SPLINE_BEZIER_QUAD) segmentCount = (pointCount + 1)/2;     // inverse of 2*pointCount - 1
    else if (type == SPLINE_BEZIER_CUBIC) segmentCount = (pointCount + 2)/3;    // inverse of 3*pointCount - 2
    else TRACELOG(LOG_WARNING, "RLSPLINES: Unknown spline type: %i", type);

    return segmentCount;
}

// Splines point access functions

static int SplineAnchorIndex(int type, int index)
{
    int idx = -1;

    if (type == SPLINE_LINEAR) idx = index;
    else if (type == SPLINE_BASIS) idx = index;                          // TODO: Confirm anchors work like this in B-splines
    else if (type == SPLINE_CATMULL_ROM) idx = index;                    // TODO: Confirm anchors work like this in Catmull-Rom splines
    else if (type == SPLINE_BEZIER_QUAD) idx = 2*index;
    else if (type == SPLINE_BEZIER_CUBIC) idx = 3*index;
    else TRACELOG(LOG_WARNING, "RLSPLINES: Unknown spline type: %i", spline.type);

    return idx;
}

// Get position of spline point
Vector2 GetSplinePoint(Spline spline, int index)
{
    Vector2 point = { 0,0 };
    
    int idx = 2*index;
    if ((0 <= idx) && (idx < spline.pointCount - 1) && (spline.points != NULL))
    {
        point.x = spline.points[idx];
        point.y = spline.points[idx + 1];
    }
    else TRACELOG(LOG_WARNING, "RLSPLINES: Index out of bounds");

    return point;
}

// Get position of spline anchor point
Vector2 GetSplineAnchor(Spline spline, int index)
{
    return GetSplinePoint(spline, SplineAnchorIndex(spline.type, index));
}

// Get position of quadratic bezier spline control point
Vector2 GetSplineControl(Spline spline, int index)
{
    int idx = -1;
    if (spline.type == SPLINE_BEZIER_QUAD) idx = 4*index + 2;
    else TRACELOG(LOG_WARNING, "RLSPLINES: GetSplineControl only valid for SPLINE_BEZIER_QUAD");
    
    return GetSplinePoint(spline, idx);
}

// Get position of cubic bezier spline in-control point
Vector2 GetSplineControlIn(Spline spline, int index)
{
    int idx = -1;
    if (spline.type == SPLINE_BEZIER_CUBIC) idx = 6*index + 2;
    else TRACELOG(LOG_WARNING, "RLSPLINES: GetSplineControlIn only valid for SPLINE_BEZIER_CUBIC");
    
    return GetSplinePoint(spline, idx);
}

// Get position of cubic bezier spline out-control point
Vector2 GetSplineControlOut(Spline spline, int index)
{
    int idx = -1;
    if (spline.type == SPLINE_BEZIER_CUBIC) idx = 6*index + 4;
    else TRACELOG(LOG_WARNING, "RLSPLINES: GetSplineControlIn only valid for SPLINE_BEZIER_CUBIC");
    
    return GetSplinePoint(spline, idx);
}

// Set position of spline point
void SetSplinePoint(Spline spline, int index, Vector2 position)
{
    int idx = 2*index;
    if ((0 <= idx) && (idx < spline.pointCount - 1) && (spline.points != NULL))
    {
        spline.points[idx] = position.x;
        spline.points[idx + 1] = position.y;
    }
    else TRACELOG(LOG_WARNING, "RLSPLINES: Index out of bounds");
}

// Set position of spline anchor point
void SetSplineAnchor(Spline spline, int index, Vector2 position)
{
    return GetSplinePoint(spline, SplineAnchorIndex(spline.type, index), position);
}

// Set position of quadratic bezier spline control point
void SetSplineControl(Spline spline, int index, Vector2 position)
{
    int idx = -1;
    if (spline.type == SPLINE_BEZIER_QUAD) idx = 4*index + 2;
    else TRACELOG(LOG_WARNING, "RLSPLINES: SetSplineControl only valid for SPLINE_BEZIER_QUAD");
    
    return SetSplinePoint(spline, idx, position);
}

// Set position of cubic bezier spline in-control point
void SetSplineControlIn(Spline spline, int index, Vector2 position)
{
    int idx = -1;
    if (spline.type == SPLINE_BEZIER_CUBIC) idx = 6*index + 2;
    else TRACELOG(LOG_WARNING, "RLSPLINES: SetSplineControlIn only valid for SPLINE_BEZIER_CUBIC");
    
    return SetSplinePoint(spline, idx, position);
}

// Set position of cubic bezier spline out-control point
void SetSplineControlOut(Spline spline, int index, Vector2 position)
{
    int idx = -1;
    if (spline.type == SPLINE_BEZIER_CUBIC) idx = 6*index + 4;
    else TRACELOG(LOG_WARNING, "RLSPLINES: SetSplineControlIn only valid for SPLINE_BEZIER_CUBIC");
    
    return SetSplinePoint(spline, idx, position);
}

// Draw a 2D spline using lines
void DrawSpline(Spline spline, Color color)
{
    const float step = 1.0f/SPLINE_SEGMENT_DIVISIONS;

    if (spline.points != NULL)
    {
        if (spline.type == SPLINE_LINEAR)
        {
            if (spline.pointCount >= 2)
            {
                rlBegin(RL_LINES);
                    rlColor4ub(color.r, color.g, color.b, color.a);
                    float xPrev = spline.points[0];
                    float yPrev = spline.points[1];
                    for (int i = 2; i < (2*spline.pointCount - 1); i += 2)
                    {
                        float x = spline.points[i];
                        float y = spline.points[i + 1];
                        rlVertex2f(xPrev, yPrev);
                        rlVertex2f(x, y);
                        xPrev = x;
                        yPrev = y;
                    }
                rlEnd();
            }
        }
        else if (spline.type == SPLINE_BASIS)
        {
            // TODO
        }
        else if (spline.type == SPLINE_CATMULL_ROM)
        {
            // TODO
        }
        else if (spline.type == SPLINE_BEZIER_QUAD)
        {
            if (spline.pointCount >= 3)
            {
                float startPosX = spline.points[0], startPosY = spline.points[1];
                float controlPosX, controlPosY;
                float endPosX, endPosY;

                float xPrev = startPosX,
                      yPrev = startPosY;

                rlBegin(RL_LINES);
                    rlColor4ub(color.r, color.g, color.b, color.a);
                    for (int i = 2; i < 2*spline.pointCount - 3; )
                    {
                        controlPosX = spline.points[i++];
                        controlPosY = spline.points[i++];
                        endPosX = spline.points[i++];
                        endPosY = spline.points[i++];
                        for (int division = 1; division <= SPLINE_SEGMENT_DIVISIONS; ++division)
                        {
                            float t = division*step;

                            float a = powf(1.0f - t, 2);
                            float b = 2.0f*(1.0f - t)*t;
                            float c = t*t;

                            float x = a*startPosX + b*controlPosX + c*endPosX;
                            float y = a*startPosY + b*controlPosY + c*endPosY;

                            rlVertex2f(xPrev, yPrev);
                            rlVertex2f(x, y);

                            xPrev = x;
                            yPrev = y;
                        }
                        startPosX = endPosX;
                        startPosY = endPosY;
                    }
                rlEnd();
            }
        }
        else if (spline.type == SPLINE_BEZIER_CUBIC)
        {
            if (spline.pointCount >= 4)
            {
                float startPosX = spline.points[0], startPosY = spline.points[1];
                float controlOutPosX, controlOutPosY;
                float controlInPosX, controlInPosY;
                float endPosX, endPosY;

                float xPrev = startPosX,
                      yPrev = startPosY;

                rlBegin(RL_LINES);
                    rlColor4ub(color.r, color.g, color.b, color.a);
                    for (int i = 2; i < 2*spline.pointCount - 5; )
                    {
                        controlOutPosX = spline.points[i++];
                        controlOutPosY = spline.points[i++];
                        controlInPosX = spline.points[i++];
                        controlInPosY = spline.points[i++];
                        endPosX = spline.points[i++];
                        endPosY = spline.points[i++];
                        for (int division = 1; division <= SPLINE_SEGMENT_DIVISIONS; ++division)
                        {
                            float t = division*step;

                            float a = powf(1.0f - t, 3);
                            float b = 3.0f*powf(1.0f - t, 2)*t;
                            float c = 3.0f*(1.0f - t)*t*t;
                            float d = t*t*t;

                            float x = a*startPosX + b*controlOutPosX + c*controlInPosX + d*endPosX;
                            float y = a*startPosY + b*controlOutPosY + c*controlInPosY + d*endPosY;

                            rlVertex2f(xPrev, yPrev);
                            rlVertex2f(x, y);

                            xPrev = x;
                            yPrev = y;
                        }
                        startPosX = endPosX;
                        startPosY = endPosY;
                    }
                rlEnd();
            }
        }
    }
}

#endif      // RLSPLINES_IMPLEMENTATION
