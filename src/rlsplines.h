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
*           Allows rlsplines to work without Raylib.
*
*       #define RLSPLINES_SUPPORT_DRAWING  <1 or 0>  (1 by default)
*           Enable support for spline rendering. Requires defining your own implementations for some
*           functions if used with RLSPLINES_STANDALONE.
*
*       #define RLSPLINES_SUPPORT_TRACELOG  <1 or 0>  (1 by default)
*           Enable tracelog for rlsplines.
*
*       #define RLSPLINES_SUPPORT_SPLINE_LINEAR  <1 or 0>  (1 by default)
*           Enable support for linear splines.
*
*       #define RLSPLINES_SUPPORT_SPLINE_BASIS  <1 or 0>  (0 by default)
*           Enable support for B-splines.
*
*       #define RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM  <1 or 0>  (0 by default)
*           Enable support for Bezier splines.
*
*       #define RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD  <1 or 0>  (0 by default)
*           Enable support for Quadratic Bezier splines.
*
*       #define RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC  <1 or 0>  (1 by default)
*           Enable support for Cubic Bezier splines.
*
*       #define RLSPLINES_SUPPORT_SPLINE_CUSTOM  <1 or 0>  (0 by default)
*           Enable support for a custom spline type.
*           Requires defining your own implementations for some functions.
*
*       #define RLSPLINES_SUPPORT_ARR_SPLINE  <1 or 0>  (0 by default)
*           Enable support for splines holding an external buffer instead of
*           allocating their own memory.
*
*       #define RLSPLINES_SUPPORT_3D  <1 or 0>  (0 by default)
*           Include support for 3D splines.
*
*       #define RLSPLINES_SUPPORT_GRADIENT  <1 or 0>  (0 by default)
*           Include support for gradient spline drawing. Implies RLSPLINES_SUPPORT_DRAWING.
*
*       #define RLSPLINES_SUPPORT_ULTRA  <1 or 0>  (0 by default)
*           Include support for "ultra" (`_Ult()`) spline functions (lots of parameters).
*
*       #define RLSPLINES_DIMENSION_ALIASES  <1 or 0>  (0 by default)
*           Define aliases with "1/2D" in the name for unspecified spline types/functions, to
*           help distinguish from their 2/3D counterparts, for consistency, or if you just prefer
*           having that additional specificity.
*
*       #define SPLINE_SEGMENT_DIVISIONS  <integer greater than 0>  (24 by default)
*           Define the number of subdivisions used per spline segment.
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

#ifndef RLSPLINES_STANDALONE
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

// Configuration defaults
#ifndef RLSPLINES_SUPPORT_DRAWING
    #define RLSPLINES_SUPPORT_DRAWING 1
#endif
#ifndef RLSPLINES_SUPPORT_TRACELOG
    #define RLSPLINES_SUPPORT_TRACELOG 1
#endif
#ifndef RLSPLINES_SUPPORT_SPLINE_LINEAR
    #define RLSPLINES_SUPPORT_SPLINE_LINEAR 1
#endif
#ifndef RLSPLINES_SUPPORT_SPLINE_BASIS
    #define RLSPLINES_SUPPORT_SPLINE_BASIS 0
#endif
#ifndef RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
    #define RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM 0
#endif
#ifndef RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
    #define RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD 0
#endif
#ifndef RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
    #define RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC 1
#endif
#ifndef RLSPLINES_SUPPORT_SPLINE_CUSTOM
    #define RLSPLINES_SUPPORT_SPLINE_CUSTOM 0
#endif
#ifndef RLSPLINES_SUPPORT_ARR_SPLINE
    #define RLSPLINES_SUPPORT_ARR_SPLINE 0
#endif
#ifndef RLSPLINES_SUPPORT_3D
    #define RLSPLINES_SUPPORT_3D 0
#endif
#ifndef RLSPLINES_SUPPORT_GRADIENT
    #define RLSPLINES_SUPPORT_GRADIENT 0
#endif
#ifndef RLSPLINES_SUPPORT_ULTRA
    #define RLSPLINES_SUPPORT_ULTRA 0
#endif
#ifndef RLSPLINES_DIMENSION_ALIASES
    #define RLSPLINES_DIMENSION_ALIASES 0
#endif
#ifndef SPLINE_SEGMENT_DIVISIONS
    #define SPLINE_SEGMENT_DIVISIONS  24        /* Spline segments subdivisions */
#endif

#define RLSPLINES_SUPPORT_ARR_SPLINE  1         // Actively being developed
#define RLSPLINES_SUPPORT_3D          0         // Ignore syntax errors until ready to implement
#define RLSPLINES_SUPPORT_GRADIENT    0         // Ignore syntax errors until ready to implement
#define RLSPLINES_SUPPORT_ULTRA       0         // Ignore syntax errors until ready to implement

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Types and Structures Definition
// NOTE: Some types are required for RLSPLINES_STANDALONE usage
//----------------------------------------------------------------------------------
#ifndef __cplusplus
// Boolean type
    #ifndef true
        typedef enum { false, true } bool;
    #endif
#endif

#ifndef RLSPLINES_STANDALONE
    #if RLSPLINES_SUPPORT_DRAWING
        typedef Color {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
        } Color;
    #endif      // RLSPLINES_SUPPORT_DRAWING

    typedef Vector2 {
        float x;
        float y;
    } Vector2;

    #if RLSPLINES_SUPPORT_3D
        typedef Vector3 {
            float x;
            float y;
            float z;
        } Vector3;
    #endif      // RLSPLINES_SUPPORT_3D
#endif      // RLSPLINES_STANDALONE

// Spline, 2D spline point data
typedef struct Spline {
    int splineType;                     // Spline type: See SplineType enum for possible values
    int pointCount;                     // Number of points and control points (spline type controls ratio and order)
    float *points;                      // Points and control points in spline (XY - 2 components per vertex), minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
} Spline;

#ifdef RLSPLINES_SUPPORT_3D
    // Spline3D, 3D spline point data
    typedef struct Spline3D {
        int splineType;                     // Spline type: See SplineType enum for possible values
        int pointCount;                     // Number of points and control points (spline type controls ratio and order)
        float *points;                      // Point and control point positions in spline (XYZ - 3 components per vertex), minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
        float *rotations;                   // Roll (twist about tangent) angles, or NULL if all are 0
        SplineProfile profile;              // Spline profile, the shape that gets extruded along the spline's curve
    } Spline3D;
#endif      // RLSPLINES_SUPPORT_3D

// Spline thickness, variable thickness data in the form of a 1D Cubic Bezier spline with remapped timings (t-values) along the curve of another spline
typedef struct SplineThickness {
    int thicknessCount;                 // Number of thicknesses
    float *timings;                     // Thickness timings (t-values), minimum 4 values (2 control values): [t1, t2, t3, t4, t5, t6...], or NULL if evenly spread
    float *thicknesses;                 // Thicknesses, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
} SplineThickness;

#ifdef RLSPLINES_SUPPORT_3D
    // 2D Spline thickness, variable thickness data in the form of a 2D Cubic Bezier spline with remapped timings (t-values) along the curve of another spline
    // NOTE: Only valid for a 3D spline, a 2D spline only has 1 perpendicular axis
    typedef struct SplineThickness2D {
        int thicknessCount;                 // Number of thicknesses
        float *timings;                     // Thickness timings (t-values), minimum 4 values (2 control values): [p1t, c2t, c3t, p4t, c5t, c6t...], or NULL if evenly spread
        float *thicknesses;                 // Thicknesses, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
    } SplineThickness2D;
#endif      // RLSPLINES_SUPPORT_3D

#ifdef RLSPLINES_SUPPORT_GRADIENT
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

    #ifdef RLSPLINES_SUPPORT_3D
        // Spline gradient, color gradient data along a spline in the form of a linear color spline with remapped 3D timings (t-values) along the curve of another spline
        // NOTE: Only valid for a 3D spline, a 2D spline only has 2 gradientable axis
        typedef struct SplineGradient3D {
            int controlCount;                   // Number of controls
            float *timings;                     // Control timings (XYZ - 3 components per timing), minimum 2 timings, or NULL if evenly spread
            unsigned char *colors;              // Control colors (RGBA - 4 components per vertex)
        } SplineGradient3D;
    #endif      // RLSPLINES_SUPPORT_3D
#endif      // RLSPLINES_SUPPORT_GRADIENT


//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------
// Spline type
typedef enum {
#if RLSPLINES_SUPPORT_SPLINE_LINEAR
    SPLINE_TYPE_LINEAR = 0,             // Linear spline, minimum 2 points
#endif
#if RLSPLINES_SUPPORT_SPLINE_BASIS
    SPLINE_TYPE_BASIS = 1,              // B-Spline, minimum 4 points
#endif
#if RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
    SPLINE_TYPE_CATMULL_ROM = 2,        // Catmull-Rom spline, minimum 4 points
#endif
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
    SPLINE_TYPE_BEZIER_QUAD = 3,        // Quadratic Bezier spline, minimum 3 points (1 control point): [p1, c2, p3, c4...]
#endif
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
    SPLINE_TYPE_BEZIER_CUBIC = 4,       // Cubic Bezier spline, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
#endif
#if RLSPLINES_SUPPORT_SPLINE_CUSTOM
    SPLINE_TYPE_CUSTOM = 5,             // Custom user-defined spline (requires defining your own functions)
#endif
    SPLINE_TYPE_UNKNOWN = -1            // Marker for erroneous spline types
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

#ifdef RLSPLINES_SUPPORT_3D
// Spline profile
typedef enum {
    SPLINE_PROFILE_FLAT = 0,            // Flat spline profile, a plane facing perpendicular to the curve tangent
    SPLINE_PROFILE_TRIANGLE,            // Triangle spline profile, a triangle facing parallel with the curve tangent
    SPLINE_PROFILE_RECTANGLE,           // Triangle spline profile, a rectangle facing parallel with the curve tangent
    SPLINE_PROFILE_CIRCLE               // Triangle spline profile, a circle facing parallel with the curve tangent
} SplineProfile;
#endif      // RLSPLINES_SUPPORT_3D

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
RLSPLINESAPI Spline GenSpline(int splineType, int segmentCount);                        // Generate spline with all points at 0,0
#ifdef RLSPLINES_SUPPORT_ARR_SPLINE
    RLSPLINESAPI Spline GenArrSpline(int splineType, float *buffer, int pointCount);        // Construct a spline whose memory is managed outside of rlsplines (buffer must have an even number of elements)
#endif
#if RLSPLINES_SUPPORT_SPLINE_LINEAR
    RLSPLINESAPI Spline GenSplineLinear(const Vector2 *points, int pointCount);             // Generate spline: Linear, minimum 2 points
#endif
#if RLSPLINES_SUPPORT_SPLINE_BASIS
    RLSPLINESAPI Spline GenSplineBasis(const Vector2 *points, int pointCount);              // Generate spline: B-Spline, minimum 4 points
#endif
#if RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
    RLSPLINESAPI Spline GenSplineCatmullRom(const Vector2 *points, int pointCount);         // Generate spline: Catmull-Rom, minimum 4 points
#endif
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
    RLSPLINESAPI Spline GenSplineBezierQuad(const Vector2 *anchorPoints, const Vector2 *controlPoints, int anchorCount); // Generate spline: Quadratic Bezier, minimum 3 points (1 control point)
#endif
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
    RLSPLINESAPI Spline GenSplineBezierCubic(const Vector2 *anchorPoints, const Vector2 *controlOutPoints, const Vector2 *controlInPoints, int anchorCount); // Generate spline: Cubic Bezier, minimum 4 points (2 control points)
#endif
RLSPLINESAPI Spline ResizeSpline(Spline spline, int newSegmentCount);                   // Unload the spline and generate a new spline with newSegmentCount segments, retaining existing points
RLSPLINESAPI void UnloadSpline(Spline spline);                                          // Unload spline
RLSPLINESAPI bool IsSplineValid(Spline spline);                                         // Check if a spline is valid

RLSPLINESAPI Spline SplineJoin(Spline spline1, Spline spline2);                         // Create a new spline joining two splines of the same type
RLSPLINESAPI Spline SplineJoinMany(const Spline *spline, int splineCount);              // Create a new spline joining multiple splines of the same type

RLSPLINESAPI int GetSplineSegmentCount(int splineType, int pointCount);                 // Get (calculate) number of segments in a spline

// Splines point access functions
RLSPLINESAPI Vector2 GetSplinePoint(Spline spline, int index);                          // Get position of spline point
RLSPLINESAPI Vector2 GetSplineAnchor(Spline spline, int index);                         // Get position of spline anchor point
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
    RLSPLINESAPI Vector2 GetSplineControl(Spline spline, int index);                        // Get position of quadratic bezier spline control point
#endif
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
    RLSPLINESAPI Vector2 GetSplineControlIn(Spline spline, int index);                      // Get position of cubic bezier spline in-control point
    RLSPLINESAPI Vector2 GetSplineControlOut(Spline spline, int index);                     // Get position of cubic bezier spline out-control point
#endif

// Splines point x access functions
RLSPLINESAPI float GetSplinePointX(Spline spline, int index);                           // Get x-position of spline point
RLSPLINESAPI float GetSplineAnchorX(Spline spline, int index);                          // Get x-position of spline anchor point
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
    RLSPLINESAPI float GetSplineControlX(Spline spline, int index);                         // Get x-position of quadratic bezier spline control point
#endif
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
    RLSPLINESAPI float GetSplineControlInX(Spline spline, int index);                       // Get x-position of cubic bezier spline in-control point
    RLSPLINESAPI float GetSplineControlOutX(Spline spline, int index);                      // Get x-position of cubic bezier spline out-control point
#endif

// Splines point y access functions
RLSPLINESAPI float GetSplinePointY(Spline spline, int index);                           // Get y-position of spline point
RLSPLINESAPI float GetSplineAnchorY(Spline spline, int index);                          // Get y-position of spline anchor point
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
    RLSPLINESAPI float GetSplineControlY(Spline spline, int index);                         // Get y-position of quadratic bezier spline control point
#endif
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
    RLSPLINESAPI float GetSplineControlInY(Spline spline, int index);                       // Get y-position of cubic bezier spline in-control point
    RLSPLINESAPI float GetSplineControlOutY(Spline spline, int index);                      // Get y-position of cubic bezier spline out-control point
#endif

// Splines set point functions
RLSPLINESAPI void SetSplinePoint(Spline spline, int index, Vector2 position);           // Set position of spline point
RLSPLINESAPI void SetSplineAnchor(Spline spline, int index, Vector2 position);          // Set position of spline anchor point
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
    RLSPLINESAPI void SetSplineControl(Spline spline, int index, Vector2 position);         // Set position of quadratic bezier spline control point
#endif
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
    RLSPLINESAPI void SetSplineControlIn(Spline spline, int index, Vector2 position);       // Set position of cubic bezier spline in-control point
    RLSPLINESAPI void SetSplineControlOut(Spline spline, int index, Vector2 position);      // Set position of cubic bezier spline out-control point
#endif

// Splines set x functions
RLSPLINESAPI void SetSplinePointX(Spline spline, int index, float value);               // Set x-position of spline point
RLSPLINESAPI void SetSplineAnchorX(Spline spline, int index, float value);              // Set x-position of spline anchor point
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
    RLSPLINESAPI void SetSplineControlX(Spline spline, int index, float value);             // Set x-position of quadratic bezier spline control point
#endif
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
    RLSPLINESAPI void SetSplineControlInX(Spline spline, int index, float value);           // Set x-position of cubic bezier spline in-control point
    RLSPLINESAPI void SetSplineControlOutX(Spline spline, int index, float value);          // Set x-position of cubic bezier spline out-control point
#endif

// Splines set y functions
RLSPLINESAPI void SetSplinePointY(Spline spline, int index, float value);               // Set y-position of spline point
RLSPLINESAPI void SetSplineAnchorY(Spline spline, int index, float value);              // Set y-position of spline anchor point
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
    RLSPLINESAPI void SetSplineControlY(Spline spline, int index, float value);             // Set y-position of quadratic bezier spline control point
#endif
#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
    RLSPLINESAPI void SetSplineControlInY(Spline spline, int index, float value);           // Set y-position of cubic bezier spline in-control point
    RLSPLINESAPI void SetSplineControlOutY(Spline spline, int index, float value);          // Set y-position of cubic bezier spline out-control point
#endif

// Splines position/derivative functions
RLSPLINESAPI Vector2 GetSplinePosition(Spline spline, float t);                         // Calculate position at t [0.0f .. 1.0f] along spline
RLSPLINESAPI Vector2 GetSplineVelocity(Spline spline, float t);                         // Calculate velocity (derivative order 1) at t [0.0f .. 1.0f] along spline
RLSPLINESAPI Vector2 GetSplineAcceleration(Spline spline, float t);                     // Calculate acceleration (derivative order 2) at t [0.0f .. 1.0f] along spline
RLSPLINESAPI Vector2 GetSplineJolt(Spline spline, float t);                             // Calculate jolt/jerk (derivative order 3) at t [0.0f .. 1.0f] along spline
RLSPLINESAPI Vector2 GetSplineNDerivative(Spline spline, int order, float t);           // Calculate derivative order n at t [0.0f .. 1.0f] along spline

RLSPLINESAPI Rectangle GetSplineBounds(Spline spline);                                  // Calculate bounding box of spline

#if RLSPLINES_SUPPORT_DRAWING
    // Splines drawing functions
    RLSPLINESAPI void DrawSpline(Spline spline, Color color);                               // Draw a 2D spline using lines
    RLSPLINESAPI void DrawSplineEx(Spline spline, float thick, Color color);                // Draw a 2D spline with thickness
    RLSPLINESAPI void DrawSplineVar(Spline spline, SplineThickness thick, Color color);     // Draw a 2D spline with variable thickness
    RLSPLINESAPI void DrawSplinePro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, Color color); // Draw a 2D spline with 'pro' parameters
    #ifdef RLSPLINES_SUPPORT_ULTRA
        RLSPLINESAPI void DrawSplineUlt(                                                        // Draw a 2D spline with ultra parameters
            Spline spline,
            SplineThickness thick,
            SplineCap startCap,
            float startCapSize,
            SplineCap endCap,
            float endCapSize,
            const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
            const float *maxSharpnesses,        // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
            Color color
        );
    #endif      // RLSPLINES_SUPPORT_ULTRA

    #ifdef RLSPLINES_SUPPORT_GRADIENT

        // Gradient splines drawing functions
        RLSPLINESAPI void DrawSplineGradient(Spline spline, SplineGradient colors);
        RLSPLINESAPI void DrawSplineGradientEx(Spline spline, float thick, SplineGradient colors);
        RLSPLINESAPI void DrawSplineGradientVar(Spline spline, SplineThickness thick, SplineGradient colors);
        RLSPLINESAPI void DrawSplineGradientPro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient colors);
        #ifdef RLSPLINES_SUPPORT_ULTRA
            RLSPLINESAPI void DrawSplineGradientUlt(
                Spline spline,
                SplineThickness thick,
                SplineCap startCap,
                float startCapSize,
                SplineCap endCap,
                float endCapSize,
                const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
                const float *maxSharpnesses,        // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
                SplineGradient colors
            );
        #endif      // RLSPLINES_SUPPORT_ULTRA

        // 2D Gradient splines drawing functions
        RLSPLINESAPI void DrawSplineGradient2Ex(Spline spline, float thick, SplineGradient2D colors);
        RLSPLINESAPI void DrawSplineGradient2Var(Spline spline, SplineThickness thick, SplineGradient2D colors);
        RLSPLINESAPI void DrawSplineGradient2Pro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient2D colors);
        #ifdef RLSPLINES_SUPPORT_ULTRA
            RLSPLINESAPI void DrawSplineGradient2Ult(
                Spline spline,
                SplineThickness thick,
                SplineCap startCap,
                float startCapSize,
                SplineCap endCap,
                float endCapSize,
                const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
                const float *maxSharpnesses,        // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
                SplineGradient2D colors
            );
        #endif      // RLSPLINES_SUPPORT_ULTRA

    #endif      // RLSPLINES_SUPPORT_GRADIENT

    #ifdef RLSPLINES_SUPPORT_3D

        // 3D Splines drawing functions
        RLSPLINESAPI void DrawSpline3D(Spline3D spline, Color color);
        RLSPLINESAPI void DrawSpline3DEx(Spline3D spline, float thick, Color color);
        RLSPLINESAPI void DrawSpline3DVar(Spline3D spline, SplineThickness2D thick, Color color);
        RLSPLINESAPI void DrawSpline3DPro(Spline3D spline, SplineThickness2D thick, float capSize, SplineElbow elbow, float maxSharpness, Color color);
        #ifdef RLSPLINES_SUPPORT_ULTRA
            RLSPLINESAPI void DrawSpline3DUlt(
                Spline3D spline,
                SplineThickness2D thick,
                SplineCap startCap,
                float startCapSize,
                SplineCap endCap,
                float endCapSize,
                const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
                const float *maxSharpnesses,        // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
                Color color
            );
        #endif      // RLSPLINES_SUPPORT_ULTRA

        #ifdef RLSPLINES_SUPPORT_GRADIENT

            // Gradient 3D splines drawing functions
            RLSPLINESAPI void DrawSpline3DGradient(Spline3D spline, SplineGradient colors);
            RLSPLINESAPI void DrawSpline3DGradientEx(Spline3D spline, float thick, SplineGradient colors);
            RLSPLINESAPI void DrawSpline3DGradientVar(Spline3D spline, SplineThickness2D thick, SplineGradient colors);
            RLSPLINESAPI void DrawSpline3DGradientPro(Spline3D spline, SplineThickness2D thick, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient colors);
            #ifdef RLSPLINES_SUPPORT_ULTRA
                RLSPLINESAPI void DrawSpline3DGradientUlt(
                    Spline3D spline,
                    SplineThickness2D thick,
                    SplineCap startCap,
                    float startCapSize,
                    SplineCap endCap,
                    float endCapSize,
                    const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
                    const float *maxSharpnesses,        // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
                    SplineGradient colors
                );
            #endif      // RLSPLINES_SUPPORT_ULTRA

            // 2D Gradient 3D splines drawing functions
            RLSPLINESAPI void DrawSpline3DGradient2Ex(Spline3D spline, float thick, SplineGradient colors);
            RLSPLINESAPI void DrawSpline3DGradient2Var(Spline3D spline, SplineThickness2D thick, SplineGradient colors);
            RLSPLINESAPI void DrawSpline3DGradient2Pro(Spline3D spline, SplineThickness2D thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness, SplineGradient colors);
            #ifdef RLSPLINES_SUPPORT_ULTRA
                RLSPLINESAPI void DrawSpline3DGradient2Ult(
                    Spline3D spline,
                    SplineThickness2D thick,
                    SplineCap startCap,
                    float startCapSize,
                    SplineCap endCap,
                    float endCapSize,
                    const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
                    const float *maxSharpnesses,        // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
                    SplineGradient colors
                );
            #endif      // RLSPLINES_SUPPORT_ULTRA

        #endif      // RLSPLINES_SUPPORT_GRADIENT

    #endif      // RLSPLINES_SUPPORT_3D

#endif      // RLSPLINES_SUPPORT_DRAWING

// Splines outline functions
RLSPLINESAPI Spline SplineOutlineEx(Spline spline, float thick);                // Calculate spline outlines as a new spline
RLSPLINESAPI Spline SplineOutlineVar(Spline spline, SplineThickness thick);     // Calculate spline outlines as a new spline
RLSPLINESAPI Spline SplineOutlinePro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness); // Calculate spline outlines as a new spline
#ifdef RLSPLINES_SUPPORT_ULTRA
    RLSPLINESAPI Spline SplineOutlineUlt(                                           // Calculate spline outlines as a new spline
        Spline spline,
        SplineThickness thick,
        SplineCap startCap,
        float startCapSize,
        SplineCap endCap,
        float endCapSize,
        const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
        const float *maxSharpnesses         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    );
#endif      // RLSPLINES_SUPPORT_ULTRA

#ifdef RLSPLINES_SUPPORT_3D

// 3D Splines outline functions
RLSPLINESAPI Spline Spline3DOutlineEx(Spline spline, float thick);              // Calculate spline outlines as a new spline
RLSPLINESAPI Spline Spline3DOutlineVar(Spline spline, SplineThickness thick);   // Calculate spline outlines as a new spline
RLSPLINESAPI Spline Spline3DOutlinePro(Spline spline, SplineThickness thick, SplineCap cap, float capSize, SplineElbow elbow, float maxSharpness); // Calculate spline outlines as a new spline
#ifdef RLSPLINES_SUPPORT_ULTRA
    RLSPLINESAPI Spline Spline3DOutlineUlt(                                         // Calculate spline outlines as a new spline
        Spline3D spline,
        SplineThickness2D thick,
        SplineCap startCap,
        float startCapSize,
        SplineCap endCap,
        float endCapSize,
        const SplineElbow *elbows,          // Number of elbows must match number of spline segments (total points (excluding control points) minus 1)
        const float *maxSharpnesses         // Can be NULL if no elbow is SPLINE_ELBOW_SHARP
    );
#endif      // RLSPLINES_SUPPORT_ULTRA

#endif      // RLSPLINES_SUPPORT_3D

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

// Allow custom memory allocators
#if !defined(RLSPLINES_MALLOC) || !defined(RLSPLINES_CALLOC) || !defined(RLSPLINES_REALLOC) || !defined(RLSPLINES_FREE)
    #include <stdlib.h>     // Required for: malloc(), calloc(), realloc(), free()
#endif
#ifndef RLSPLINES_MALLOC
    #define RLSPLINES_MALLOC(sz)       malloc(sz)
#endif
#ifndef RLSPLINES_CALLOC
    #define RLSPLINES_CALLOC(n,sz)     calloc(n,sz)
#endif
#ifndef RLSPLINES_REALLOC
    #define RLSPLINES_REALLOC(p,sz)    realloc(p,sz)
#endif
#ifndef RLSPLINES_FREE
    #define RLSPLINES_FREE(p)          free(p)
#endif

// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#if RLSPLINES_SUPPORT_TRACELOG
    #define RLSPLINES_TRACELOG(level, ...)           TraceLog(level, __VA_ARGS__)
#else
    #define RLSPLINES_TRACELOG(level, ...)           (void)0
#endif

#ifdef RLSPLNES_SUPPORT_DRAWING
    #ifndef RLSPLINES_STANDALONE
        #include "rlgl.h"
        #define BeginLines() rlBegin()
        #define BeginTriangles() rlBegin()
        #define SetDrawColor(COLOR) rlColor4ub(COLOR.r, COLOR.g, COLOR.b, COLOR.a)
        #define PushVertex2D(x, y) rlVertex2f(x, y)
        #define PushVertex3D(x, y, z) rlVertex2f(x, y, z)
        #define EndLines() rlEnd()
        #define EndTriangles() rlEnd()
    #else
        void BeginLines();
        void BeginTriangles();
        void SetDrawColor(COLOR);
        void PushVertex2D(x, y);
        void PushVertex3D(x, y, z);
        void EndLines();
        void EndTriangles();
    #endif
#endif

#include <math.h>               // Required for

#if RLSPLINES_SUPPORT_SPLINE_CUSTOM

    // Define these yourself

    int GetSplineSegmentCountCustom(int pointCount);
    extern int CUSTOM_SPLINE_MIN_SEGMENTS;
    extern int CUSTOM_SPLINE_MAX_SEGMENTS;
    extern int CUSTOM_SPLINE_CONTROLS_PER_SEGMENT;

#endif      // RLSPLINES_SUPPORT_SPLINE_CUSTOM

#ifdef __cplusplus
    #define RLSPLINES_CLITERAL(name) name
#else
    #define RLSPLINES_CLITERAL(name) (name)
#endif

// Generate spline with all points at 0,0
// splineType: SPLINE_LINEAR, SPLINE_BASIS, SPLINE_CATMULL_ROM, SPLINE_BEZIER_QUAD, or SPLINE_BEZIER_CUBIC
Spline GenSpline(int splineType, int segmentCount)
{
    Spline spline = { 0 };
    spline.splineType = splineType;

    int pointCount = 0;
    switch (splineType)
    {
    #if RLSPLINES_SUPPORT_SPLINE_LINEAR
        case SPLINE_TYPE_LINEAR: pointCount = segmentCount; break;
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_BASIS
        case SPLINE_TYPE_BASIS: pointCount = segmentCount; break;               // TODO: Confirm segments work like this in B-splines
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
        case SPLINE_TYPE_CATMULL_ROM: pointCount = segmentCount; break;         // TODO: Confirm segments work like this in Catmull-Rom splines
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
        case SPLINE_TYPE_BEZIER_QUAD: pointCount = 2*segmentCount + 1; break;
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
        case SPLINE_TYPE_BEZIER_CUBIC: pointCount = 3*segmentCount + 1; break;
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_CUSTOM
        case SPLINE_TYPE_CUSTOM: // TODO
    #endif
        default: RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Unsupported spline type: %i", type); break;
    }

    if (pointCount > 0)
    {
        spline.pointCount = pointCount;
        spline.points = (float *)RLSPLINES_CALLOC(pointCount*2, sizeof(float));
    }

    return spline;
}

#if RLSPLINES_SUPPORT_ARR_SPLINE
// Construct a spline whose memory is managed outside of rlsplines
//
// IMPORTANT:
// Because this memory is being managed outside of rlsplines,
// the following functions must NOT be used on the return of this
// function because they will try to deallocate the memory:
// - ResizeSpline
// - UnloadSpline
//
// The following functions, which may appear unsafe, ARE safe to use
// because they only read the memory without deallocating:
// - SplineJoin
// - SplineJoinMany
Spline GenSplineEM(int splineType, Vector2 *buffer, int pointCount)
{
    Spline spline = { 0 };

    spline.splineType = splineType;
    spline.pointCount = pointCount;
    spline.points = (float *)buffer;

    return spline;
}
#endif      // RLSPLINES_SUPPORT_ARR_SPLINE

#if RLSPLINES_SUPPORT_SPLINE_LINEAR
// Generate spline: Linear, minimum 2 points
// Use NULL for points to get a spline with uninitialized points.
Spline GenSplineLinear(const Vector2 *points, int pointCount)
{
    Spline spline = { 0 };
    spline.type = SPLINE_TYPE_LINEAR;

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
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Linear spline requires minimum 2 points");

    return spline;
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_BASIS
// Generate spline: B-Spline, minimum 4 points
// Use NULL for points to get a spline with uninitialized points.
Spline GenSplineBasis(const Vector2 *points, int pointCount)
{
    Spline spline = { 0 };
    spline.type = SPLINE_TYPE_BASIS;

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
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Basis spline requires minimum 4 points");

    return spline;
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
// Generate spline: Catmull-Rom, minimum 4 points
// Use NULL for points to get a spline with uninitialized points.
Spline GenSplineCatmullRom(const Vector2 *points, int pointCount)
{
    Spline spline = { 0 };
    spline.type = SPLINE_TYPE_CATMULL_ROM;

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
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Catmull-Rom spline requires minimum 4 points");

    return spline;
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
// Generate spline: Quadratic Bezier, minimum 3 points (1 control point): [p1, p3...], [c2, c4...]
// Use NULL for anchorPoints to get a spline with uninitialized points.
// NOTE: controlPoints must have anchorPointCount - 1 elements.
Spline GenSplineBezierQuad(const Vector2 *anchorPoints, const Vector2 *controlPoints, int anchorPointCount)
{
    Spline spline = { 0 };
    spline.type = SPLINE_TYPE_BEZIER_QUAD;

    if (anchorPointCount >= 2)
    {
        spline.pointCount = 2*anchorPointCount - 1;
        spline.points = (float *)RLSPLINES_MALLOC(spline.pointCount*sizeof(float)*2);
        if ((anchorPoints != NULL) &&
            (controlPoints != NULL))
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
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Quadratic bezier spline requires minimum 2 points + 1 control point");

    return spline;
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
// Generate spline: Cubic Bezier, minimum 4 points (2 control points): [p1, p4...], [c2, c5...], [c3, c6...]
// Use NULL for anchorPoints to get a spline with uninitialized points.
// NOTE: controlOutPoints and controlInPoints must have anchorPointCount - 1 elements each.
Spline GenSplineBezierCubic(const Vector2 *anchorPoints, const Vector2 *controlOutPoints, const Vector2 *controlInPoints, int anchorPointCount)
{
    Spline spline = { 0 };
    spline.type = SPLINE_TYPE_BEZIER_CUBIC;

    if (anchorPointCount >= 2)
    {
        spline.pointCount = 3*anchorPointCount - 2;
        spline.points = (float *)RLSPLINES_MALLOC(spline.pointCount*sizeof(float)*2);
        if ((anchorPoints != NULL) &&
            (controlOutPoints != NULL) &&
            (controlInPoints != NULL))
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
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Cubic bezier spline requires minimum 2 points + 2 control points");

    return spline;
}
#endif

// Unload the spline and generate a new spline with newSegmentCount segments, retaining existing points
// NOTE: Input spline is guaranteed to be unloaded. Returned spline points will be NULL if newSegmentCount is 0.
Spline ResizeSpline(Spline spline, int newSegmentCount)
{
    int newPointCount = -1;
    switch (spline.type)
    {
        #if RLSPLINES_SUPPORT_SPLINE_LINEAR
            case SPLINE_TYPE_LINEAR: newPointCount = newSegmentCount + 1; break;
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_BASIS
            case SPLINE_TYPE_BASIS: newPointCount = newSegmentCount + 1; break;               // TODO: Confirm segments work like this in B-splines
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
            case SPLINE_TYPE_CATMULL_ROM: newPointCount = newSegmentCount + 1; break;         // TODO: Confirm segments work like this in Catmull-Rom splines
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
            case SPLINE_TYPE_BEZIER_QUAD: newPointCount = 2*newSegmentCount + 1; break;
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
            case SPLINE_TYPE_BEZIER_CUBIC: newPointCount = 3*newSegmentCount + 1; break;
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_CUSTOM
            case SPLINE_TYPE_CUSTOM: // TODO
        #endif
        default: RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Unsupported spline type: %i", spline.type); break;
    }

    if ((newPointCount > 0) && (spline.points != NULL))
    {
        spline.points = (float *)RLSPLINES_REALLOC(spline.points, newPointCount*sizeof(float)*2);
        for (int i = 2*spline.pointCount; i < (2*newPointCount); ++i) spline.points[i] = 0.0f;
        spline.pointCount = newPointCount;
    }
    else
    {
        RLSPLINES_FREE(spline.points);
        spline.points = NULL;
        spline.pointCount = 0;
    }

    return spline;
}

// Unload spline from CPU memory (RAM)
void UnloadSpline(Spline spline)
{
    RLSPLINES_FREE(spline.points);
}

// Check if a spline is valid
bool IsSplineValid(Spline spline)
{
    bool result = false;

    if ((spline.pointCount != 0) &&
        (spline.points != NULL) &&
        (
         #if RLSPLINES_SUPPORT_SPLINE_LINEAR
            (spline.type == SPLINE_TYPE_LINEAR) ||
         #endif
         #if RLSPLINES_SUPPORT_SPLINE_BASIS
            (spline.type == SPLINE_TYPE_BASIS) ||
         #endif
         #if RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
            (spline.type == SPLINE_TYPE_CATMULL_ROM) ||
         #endif
         #if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
            (spline.type == SPLINE_TYPE_BEZIER_QUAD) ||
         #endif
         #if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
            (spline.type == SPLINE_TYPE_BEZIER_CUBIC) ||
         #endif
         #if RLSPLINES_SUPPORT_SPLINE_CUSTOM
            (spline.type == SPLINE_TYPE_CUSTOM) ||
         #endif
         false // eat trailing 'or'
        )) result = true;

    return result;
}


// Create a new spline joining two separate splines of the same type
// NOTE: Does not unload input splines
Spline SplineJoin(Spline spline1, Spline spline2)
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
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: SplineJoin requires spline1 and spline2 to be the same type");

    return joined;
}

// Create a new spline joining multiple splines of the same type
// NOTE: Does not unload input splines
Spline SplineJoinMany(const Spline *splines, int splineCount)
{
    Spline joined = { 0 };

    if ((splines != NULL) &&
        (splineCount > 1))
    {
        joined.type = splines[0].type;
        for (int i = 1; i < splineCount; ++i)
        {
            if ((splines[i].type == splines[0].type) &&
                (splines[i].points != NULL))
            {
                joined.pointCount += splines[i].pointCount;
            }
            else
            {
                RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: SplineJoinMany requires every spline to be the same type and not NULL");
                return RLSPLINES_CLITERAL(Spline){ 0 };
            }
        }

        joined.points = (float *)RLSPLINES_MALLOC(joined.pointCount*sizeof(float)*2);
        for (int i = 0, j = 0; i < splineCount; ++i)
        {
            for (int k = 0; k < (2*splines[i].pointCount); ++k)
            {
                joined.points[j++] = splines[i].points[k];
            }
        }
    }

    return joined;
}

// Get (calculate) number of segments (sub-splines) in a Spline, equal to the number of points (excluding control points) minus 1
int GetSplineSegmentCount(int type, int pointCount)
{
    int segmentCount = -1;

    switch (type)
    {
    #if RLSPLINES_SUPPORT_SPLINE_LINEAR
        case SPLINE_TYPE_LINEAR: segmentCount = pointCount - 1; break;
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_BASIS
        case SPLINE_TYPE_BASIS: segmentCount = pointCount - 1; break;               // TODO: Confirm segments work like this in B-splines
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
        case SPLINE_TYPE_CATMULL_ROM: segmentCount = pointCount - 1; break;         // TODO: Confirm segments work like this in Catmull-Rom splines
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
        case SPLINE_TYPE_BEZIER_QUAD: segmentCount = (pointCount + 1)/2; break;     // inverse of 2*pointCount - 1
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
        case SPLINE_TYPE_BEZIER_CUBIC: segmentCount = (pointCount + 2)/3; break;    // inverse of 3*pointCount - 2
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_CUSTOM
        case SPLINE_TYPE_CUSTOM: // TODO
    #endif
        default: RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Unsupported spline type: %i", type); break;
    }

    return segmentCount;
}

// Splines point access functions

static int SplineAnchorIndex(int type, int index)
{
    int idx = -1;

    switch (type)
    {
    #if RLSPLINES_SUPPORT_SPLINE_LINEAR
        case SPLINE_TYPE_LINEAR: idx = index; break;
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_BASIS
        case SPLINE_TYPE_BASIS: idx = index; break;               // TODO: Confirm segments work like this in B-splines
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
        case SPLINE_TYPE_CATMULL_ROM: idx = index; break;         // TODO: Confirm segments work like this in Catmull-Rom splines
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
        case SPLINE_TYPE_BEZIER_QUAD: idx = 2*index; break;
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
        case SPLINE_TYPE_BEZIER_CUBIC: idx = 3*index; break;
    #endif
    #if RLSPLINES_SUPPORT_SPLINE_CUSTOM
        case SPLINE_TYPE_CUSTOM: // TODO
    #endif
        default: RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Unsupported spline type: %i", type); break;
    }

    return idx;
}

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
static int SplineControlIndex(int type, int index)
{
    int idx = -1;

    if (type == SPLINE_TYPE_BEZIER_QUAD) idx = 4*index + 2;
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Control only valid for SPLINE_TYPE_BEZIER_QUAD");

    return idx;
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
static int SplineControlInIndex(int type, int index)
{
    int idx = -1;

    if (type == SPLINE_TYPE_BEZIER_CUBIC) idx = 6*index + 2;
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: ControlIn only valid for SPLINE_TYPE_BEZIER_CUBIC");

    return idx;
}

static int SplineControlOutIndex(int type, int index)
{
    int idx = -1;

    if (type == SPLINE_TYPE_BEZIER_CUBIC) idx = 6*index + 4;
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: ControlOut only valid for SPLINE_TYPE_BEZIER_CUBIC");

    return idx;
}
#endif

// Get position of spline point
Vector2 GetSplinePoint(Spline spline, int index)
{
    Vector2 point = { 0,0 };

    if ((0 <= index) &&
        (index < spline.pointCount) &&
        (spline.points != NULL))
    {
        point.x = spline.points[2*index];
        point.y = spline.points[2*index + 1];
    }
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Index out of bounds");

    return point;
}

// Get position of spline anchor point
Vector2 GetSplineAnchor(Spline spline, int index)
{
    return GetSplinePoint(spline, SplineAnchorIndex(spline.type, index));
}

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
// Get position of quadratic bezier spline control point
Vector2 GetSplineControl(Spline spline, int index)
{
    return GetSplinePoint(spline, SplineControlIndex(spline.type, index));
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
// Get position of cubic bezier spline in-control point
Vector2 GetSplineControlIn(Spline spline, int index)
{
    return GetSplinePoint(spline, SplineControlInIndex(spline.type, index));
}

// Get position of cubic bezier spline out-control point
Vector2 GetSplineControlOut(Spline spline, int index)
{
    return GetSplinePoint(spline, SplineControlOutIndex(spline.type, index));
}
#endif

// Splines point x access functions

// Get x-position of spline point
float GetSplinePointX(Spline spline, int index)
{
    float result = 0.0f;

    if ((0 <= index) &&
        (index < spline.pointCount) &&
        (spline.points != NULL)) result = spline.points[2*index];
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Index out of bounds");

    return result;
}

// Get x-position of spline anchor point
float GetSplineAnchorX(Spline spline, int index)
{
    return GetSplinePointX(spline, SplineAnchorIndex(spline.type, index));
}

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
// Get x-position of quadratic bezier spline control point
float GetSplineControlX(Spline spline, int index)
{
    return GetSplinePointX(spline, SplineControlIndex(spline.type, index));
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
// Get x-position of cubic bezier spline in-control point
float GetSplineControlInX(Spline spline, int index)
{
    return GetSplinePointX(spline, SplineControlInIndex(spline.type, index));
}

// Get x-position of cubic bezier spline out-control point
float GetSplineControlOutX(Spline spline, int index)
{
    return GetSplinePointX(spline, SplineControlOutIndex(spline.type, index));
}
#endif

// Splines point y access functions

// Get y-position of spline point
float GetSplinePointY(Spline spline, int index)
{
    float result = 0.0f;

    if ((0 <= index) &&
        (index < spline.pointCount) &&
        (spline.points != NULL)) result = spline.points[2*index + 1];
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Index out of bounds");

    return result;
}

// Get y-position of spline anchor point
float GetSplineAnchorY(Spline spline, int index)
{
    return GetSplinePointY(spline, SplineAnchorIndex(spline.type, index));
}

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
// Get y-position of quadratic bezier spline control point
float GetSplineControlY(Spline spline, int index)
{
    return GetSplinePointY(spline, SplineControlIndex(spline.type, index));
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
// Get y-position of cubic bezier spline in-control point
float GetSplineControlInY(Spline spline, int index)
{
    return GetSplinePointY(spline, SplineControlInIndex(spline.type, index));
}

// Get y-position of cubic bezier spline out-control point
float GetSplineControlOutY(Spline spline, int index)
{
    return GetSplinePointY(spline, SplineControlOutIndex(spline.type, index));
}
#endif

// Splines set point functions

// Set position of spline point
void SetSplinePoint(Spline spline, int index, Vector2 position)
{
    if ((0 <= index) &&
        (index < spline.pointCount) &&
        (spline.points != NULL))
    {
        spline.points[2*index] = position.x;
        spline.points[2*index + 1] = position.y;
    }
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Index out of bounds");
}

// Set position of spline anchor point
void SetSplineAnchor(Spline spline, int index, Vector2 position)
{
    SetSplinePoint(spline, SplineAnchorIndex(spline.type, index), position);
}

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
// Set position of quadratic bezier spline control point
void SetSplineControl(Spline spline, int index, Vector2 position)
{
    SetSplinePoint(spline, SplineControlIndex(spline.type, index), position);
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
// Set position of cubic bezier spline in-control point
void SetSplineControlIn(Spline spline, int index, Vector2 position)
{
    SetSplinePoint(spline, SplineControlInIndex(spline.type, index), position);
}

// Set position of cubic bezier spline out-control point
void SetSplineControlOut(Spline spline, int index, Vector2 position)
{
    SetSplinePoint(spline, SplineControlOutIndex(spline.type, index), position);
}
#endif

// Splines set x functions

// Set x-position of spline point
void SetSplinePointX(Spline spline, int index, float value)
{
    if ((0 <= index) &&
        (index < spline.pointCount) &&
        (spline.points != NULL)) spline.points[2*index] = value;
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Index out of bounds");
}

// Set x-position of spline anchor point
void SetSplineAnchorX(Spline spline, int index, float value)
{
    SetSplinePointX(spline, SplineAnchorIndex(spline.type, index), value);
}

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
// Set x-position of quadratic bezier spline control point
void SetSplineControlX(Spline spline, int index, float value)
{
    SetSplinePointX(spline, SplineControlIndex(spline.type, index), value);
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
// Set x-position of cubic bezier spline in-control point
void SetSplineControlInX(Spline spline, int index, float value)
{
    SetSplinePointX(spline, SplineControlInIndex(spline.type, index), value);
}

// Set x-position of cubic bezier spline out-control point
void SetSplineControlOutX(Spline spline, int index, float value)
{
    SetSplinePointX(spline, SplineControlOutIndex(spline.type, index), value);
}
#endif

// Splines set y functions

// Set y-position of spline point
void SetSplinePointY(Spline spline, int index, float value)
{
    if ((0 <= index) &&
        (index < spline.pointCount) &&
        (spline.points != NULL)) spline.points[2*index + 1] = value;
    else RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Index out of bounds");
}

// Set y-position of spline anchor point
void SetSplineAnchorY(Spline spline, int index, float value)
{
    SetSplinePointY(spline, SplineAnchorIndex(spline.type, index), value);
}

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
// Set y-position of quadratic bezier spline control point
void SetSplineControlY(Spline spline, int index, float value)
{
    SetSplinePointY(spline, SplineControlIndex(spline.type, index), value);
}
#endif

#if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
// Set y-position of cubic bezier spline in-control point
void SetSplineControlInY(Spline spline, int index, float value)
{
    SetSplinePointY(spline, SplineControlInIndex(spline.type, index), value);
}

// Set y-position of cubic bezier spline out-control point
void SetSplineControlOutY(Spline spline, int index, float value)
{
    SetSplinePointY(spline, SplineControlOutIndex(spline.type, index), value);
}
#endif

// Splines position/derivative functions

static bool SplineTIndex(int segmentCount, float *t, int *index)
{
    bool success = false;

    if ((0.0f <= (*t)) && ((*t) <= 1.0f))
    {
        float tMajor = (*t)*segmentCount;
        *index = (int)(tMajor);
        *t = tMajor - (float)(*index);
        success = true;
    }

    return success;
}

// Calculate position at t [0.0f .. 1.0f] along spline
Vector2 GetSplinePosition(Spline spline, float t)
{
    Vector2 result = { 0 };

    int index;
    switch (spline.type)
    {
        #if RLSPLINES_SUPPORT_SPLINE_LINEAR
        case SPLINE_TYPE_LINEAR:
        {
            if (SplineTIndex())
            {
                float a = (1 - t);
                float b = t;

                index *= 2;
                float x1 = spline.points[index++];
                float y1 = spline.points[index++];
                float x2 = spline.points[index++];
                float y2 = spline.points[index];

                result.x = a*x1 + b*x2;
                result.y = a*y1 + b*y2;
            }

            break;
        }
        #endif      // RLSPLINES_SUPPORT_SPLINE_LINEAR
        #if RLSPLINES_SUPPORT_SPLINE_BASIS
        // TODO: case SPLINE_TYPE_BASIS
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
        // TODO: case SPLINE_TYPE_CATMULL_ROM
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
        case SPLINE_TYPE_BEZIER_QUAD:
        {
            float a = (1 - t)*(1 - t);
            float b = 2*(1 - t)*t;              // the '2' comes from Pascal's Triangle (binomial expansion)
            float c = t*t;

            index *= 4;
            float x1 = spline.points[index++];
            float y1 = spline.points[index++];
            float x2 = spline.points[index++];
            float y2 = spline.points[index++];
            float x3 = spline.points[index++];
            float y3 = spline.points[index];

            result.x = a*x1 + b*x2 + c*x3;
            result.y = a*y1 + b*y2 + c*y3;

            break;
        }
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
        case SPLINE_TYPE_BEZIER_CUBIC:
        {
            float a = (1 - t)*(1 - t)*(1 - t);
            float b = 3*(1 - t)*(1 - t)*t;      // the '3' comes from Pascal's Triangle (trinomial expansion)
            float c = 3*(1 - t)*t*t;            // the '3' comes from Pascal's Triangle (trinomial expansion)
            float d = t*t*t;

            index *= 6;
            float x1 = spline.points[index++];
            float y1 = spline.points[index++];
            float x2 = spline.points[index++];
            float y2 = spline.points[index++];
            float x3 = spline.points[index++];
            float y3 = spline.points[index++];
            float x4 = spline.points[index++];
            float y4 = spline.points[index];

            result.x = a*x1 + b*x2 + c*x3 + d*x4;
            result.y = a*y1 + b*y2 + c*y3 + d*y4;

            break;
        }
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_CUSTOM
        case SPLINE_TYPE_CUSTOM: // TODO
        #endif
        default: RLSPLINES_TRACELOG(LOG_WARNING, "RLSPLINES: Unsupported spline type: %i", type); break;
    }

    return result;
}

// Calculate velocity (derivative order 1) at t [0.0f .. 1.0f] along spline
Vector2 GetSplineVelocity(Spline spline, float t)
{
    // TODO
}

// Calculate acceleration (derivative order 2) at t [0.0f .. 1.0f] along spline
Vector2 GetSplineAcceleration(Spline spline, float t)
{
    // TODO
}

// Calculate jolt/jerk (derivative order 3) at t [0.0f .. 1.0f] along spline
Vector2 GetSplineJolt(Spline spline, float t)
{
    // TODO
}

// Calculate derivative order n at t [0.0f .. 1.0f] along spline
Vector2 GetSplineNDerivative(Spline spline, int order, float t)
{
    // TODO
}

// Calculate bounding box of spline
Rectangle GetSplineBounds(Spline spline)
{
    // TODO
}

// Draw a 2D spline using lines
void DrawSpline(Spline spline, Color color)
{
    const float step = 1.0f/SPLINE_SEGMENT_DIVISIONS;

    if (spline.points != NULL)
    {
        switch (spline.type)
        {
        #if RLSPLINES_SUPPORT_SPLINE_LINEAR
            case SPLINE_TYPE_LINEAR:
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
                break;
            }
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_BASIS
            case SPLINE_TYPE_BASIS:
            {
                // TODO
                break;
            }
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM
            case SPLINE_TYPE_CATMULL_ROM:
            {
                // TODO
                break;
            }
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD
            case SPLINE_TYPE_BEZIER_QUAD:
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
                break;
            }
        #endif
        #if RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC
            case SPLINE_TYPE_BEZIER_CUBIC:
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
                break;
            }
        #endif
            default: break;
        }
    }
}

#endif      // RLSPLINES_IMPLEMENTATION
