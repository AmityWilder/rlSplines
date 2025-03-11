/*******************************************************************************************
*
*   Spline 2D - An example demo for 2D splines
*
*   DEPENDENCIES:
*       raylib 4.0  - Windowing/input management and drawing.
*       rlsplines 0.1  - Splines
*
*   COMPILATION (Windows - MinGW):
*       gcc -o $(NAME_PART).exe $(FILE_NAME) -I../../src -lraylib -lopengl32 -lgdi32 -std=c99
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2025 Amy Wilder (@AmityWilder)
*
**********************************************************************************************/

#include "raylib.h"

#define RLSPLINES_IMPLEMENTATION
#define RLSPLINES_SUPPORT_SPLINE_LINEAR 1
#define RLSPLINES_SUPPORT_SPLINE_BASIS 1
#define RLSPLINES_SUPPORT_SPLINE_CATMULL_ROM 1
#define RLSPLINES_SUPPORT_SPLINE_BEZIER_QUAD 1
#define RLSPLINES_SUPPORT_SPLINE_BEZIER_CUBIC 1
#define RLSPLINES_SUPPORT_SPLINE_CUSTOM 1
#define RLSPLINES_SUPPORT_ULTRA
#include "../../src/rlsplines.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //---------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 540;

    InitWindow(screenWidth, screenHeight, "rlsplines - spline_2D");
    SetTargetFPS(60);

    Spline splineLinear;
    {
        // Points only need to be copied, the array doesn't have to outlive the Spline
        Vector2 points[] = { { 50,100 }, { 100,200 }, { 150,100 }, { 200,200 } };
        splineLinear = GenSplineLinear(points, 4);
    }

    Spline splineQuad;
    {
        // Points only need to be copied, the array doesn't have to outlive the Spline
        Vector2 points[] = { { 50,100 }, { 100,200 }, { 150,100 }, { 200,200 } };
        Vector2 controlPoints[] = { { 50,200 }, { 150,200 }, { 200,100 } };
        splineQuad = GenSplineBezierQuad(points, controlPoints, 4);
    }

    Spline splineCubic;
    {
        splineCubic = GenSplineBezierCubic(NULL, NULL, NULL, 4);
    }

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------

        // TODO

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawSpline(splineQuad, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();
    //--------------------------------------------------------------------------------------

    UnloadSpline(splineQuad);

    UnloadSpline(splineLinear);

    return 0;
}
