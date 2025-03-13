#ifndef RLSPLINEMATH_H
#define RLSPLINEMATH_H

typedef struct BoundsRect {
    Vector2 min;
    Vector2 max;
} BoundsRect;

// Spline, spline point data
typedef struct Spline {
    int pointCount;                     // Number of points
    unsigned int dimension;             // Number of components per point
    unsigned int segmentSize;           // Number of points per segment minus one (linear: 1, quadratic: 2, cubic: 3)
    float *points;                      // Point positions
} Spline;

typedef float(*SplinePositionCallback)(const float *points, float t);        // Function to calculate bezier position given spline points
typedef float(*SplineVelocityCallback)(const float *points, float t);        // Derivative (See `math.md`) of corresponding SplinePositionCallback
typedef float(*SplineAccelerationCallback)(const float *points, float t);    // Derivative (See `math.md`) of corresponding SplineVelocityCallback



typedef struct SplineInterp {
    SplinePositionCallback position;
    SplineVelocityCallback velocity;
    SplineAccelerationCallback acceleration;
} SplineInterp;

#endif      // RLSPLINEMATH_H