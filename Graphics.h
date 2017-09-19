#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <tuple>
#include <stack>
#include <iostream>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define PI 3.1415

using namespace std;

class Graphics
{
private:
    int thickness;
    tuple<GLfloat, GLfloat, GLfloat, GLfloat> color;

    /// Draws a pixel on the screen given parameters of co-ordinate wrt viewport,
    /// thickness of point, and color of pixel.
    ///
    /// Only called through other member functions.

    void drawPixel(int x, int y, int thickness, tuple<GLfloat, GLfloat, GLfloat, GLfloat> color)
    {
        GLfloat vertex[] = {(GLfloat) x, (GLfloat) y};
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        GLfloat red = get<0>(color);
        GLfloat green = get<1>(color);
        GLfloat blue = get<2>(color);
        GLfloat alpha = get<3>(color);
        GLfloat color_vector[] = {red, green, blue, alpha};
        glColorPointer(4, GL_FLOAT, 0, color_vector);
        glPointSize(thickness);
        glVertexPointer(2, GL_FLOAT, 0, vertex);
        glDrawArrays(GL_POINTS, 0, 1);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

    /// A circle can be represented as a combination of eight arcs in four quadrants
    /// using cases of whether |y| > |x| to reduce calculations.
    ///
    /// This takes circle parameters and draws it on the viewport using drawPixel(..).
    ///
    /// Called from drawCircle(..) which calculates the arc of the circle.

    void drawCirclePixels(int x0, int y0, int x, int y)
    {
        drawPixel(x + x0, y + y0, thickness, color);
        drawPixel(-x + x0, y + y0, thickness, color);
        drawPixel(x + x0, -y + y0, thickness, color);
        drawPixel(-x + x0, -y + y0, thickness, color);
        drawPixel(y + x0, x + y0, thickness, color);
        drawPixel(-y + x0, x + y0, thickness, color);
        drawPixel(y + x0, -x + y0, thickness, color);
        drawPixel(-y + x0, -x + y0, thickness, color);
    }


public:
    void setThickness(int _thickness)
    {
        thickness = _thickness;
    }

    void setColor(tuple<GLfloat, GLfloat, GLfloat, GLfloat> _color)
    {
        color = _color;
    }

    /// Draws a line on the viewport given the starting point and ending point.
    /// This function uses Bresenham's line drawing algorithm for rendering clearly
    /// and efficiently.
    ///
    /// Using a modification of Bresenham's algorithm, this function can draw lines in all four quadrants

    void drawLine(int start_x, int start_y, int end_x, int end_y)
    {
        // Swap start and end point in case starting X pixel is after the ending X pixel
        if (start_x > end_x) {
            std::swap(start_x, end_x);
            std::swap(start_y, end_y);
        }
        int dx = end_x - start_x;
        int dy = end_y - start_y;

        //when line is steeper than 1
        if (abs(dy) > abs(dx)) {
            if (dy > 0) { //when line has m>1 && m<infinity
                int p = -2*dx + dy; //initial delta
                int northDelta = -2*dx;
                int northEastDelta = 2*dy - 2*dx;
                for (int x = start_x, y = start_y; y<= end_y; y++) {
                    if (p > 0) {
                        p = p + northDelta;
                    } else {
                        p = p + northEastDelta;
                        x++;
                    }
                    drawPixel(x, y, thickness, color);
                }
            } else { //when it spills over to second quadrant, but still has abs(m) > 1
                int p = 2*dx - dy; //initial delta
                int southDelta = 2*dx;
                int southEastDelta = 2*(dy + dx);
                for (int x = start_x, y = start_y; y >= end_y; y--) {
                    if (p < 0) {
                        p = p + southDelta;
                    } else {
                        p = p + southEastDelta;
                        x++;
                    }
                    drawPixel(x, y, thickness, color);
                }
            }
        } else {
            if (dy > 0) {
                int p = 2*dy - dx;
                int eastDelta = 2*dy;
                int northEastDelta = 2*(dy - dx);
                for (int x = start_x, y = start_y; x<= end_x; x++) {
                    if (p < 0) {
                        p = p + eastDelta;
                    } else {
                        p = p + northEastDelta;
                        y++;
                    }
                    drawPixel(x, y, thickness, color);
                }
            } else {
                int p = 2*dy + dx; //initial delta
                int eastDelta = 2*dy;
                int southEastDelta = 2*(dy + dx);
                for (int x = start_x, y = start_y; x<= end_x; x++) {
                    if (p > 0) {
                        p = p + eastDelta;
                    } else {
                        p = p + southEastDelta;
                        y--;
                    }
                    drawPixel(x, y, thickness, color);
                }
            }
        }
    }

    /// In many cases, we have to draw a line only given its starting point, length and angle.
    ///
    /// drawVector(..) takes these parameters and calculates the endpoints for such Lines using simple trignometry.
    /// Lines are rendered using a call to drawLine(..)
    ///
    /// Returns an std::pair<int, int> with endpoints of the given line

    pair<int, int> drawVector(int x0, int y0, int len, float angle)
    {
        int a = x0 + (int) (len * cos(angle));
        int b = y0 + (int) (len * sin(angle));
        drawLine(x0, y0, a, b);
        return make_pair(a, b);
    }

    /// Takes the centre of the circle as x0, y0 values, and the radius of the circle.
    /// The circle is rendered using the mid-point algorithm.

    void drawCircle(int x0, int y0, int radius)
    {
        int x = 0;
        int y = radius;
        int d = 1 - radius;
        int deltaE = 3;
        int deltaSE = -2 * radius + 5;
        drawCirclePixels(x0, y0, x, y);
        while(y > x) {
            if (d < 0) { // E pixel
                d += deltaE;
                deltaE += 2;
                deltaSE += 2;
            } else { // SE pixel
                d += deltaSE;
                deltaE += 2;
                deltaSE += 4;
                y--;
            }
            x++;
            drawCirclePixels(x0, y0, x, y);
        }
    }
};

#endif
unr
