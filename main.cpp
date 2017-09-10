// #include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

GLfloat pointVertex[1000][2];

void drawPixel(GLfloat x, GLfloat y)
{
    GLfloat pointVertex[] = {x, y};
    glEnable(GL_POINT_SMOOTH); // make the point circular
    glEnableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're using a vertex array for fixed-function attribute
    glPointSize(2); // must be added before glDrawArrays is called
    glVertexPointer(2, GL_FLOAT, 0, pointVertex); // point to the vertices to be used
    glDrawArrays(GL_POINTS, 0, 1); // draw the vertixes
    glDisableClientState(GL_VERTEX_ARRAY); // tell OpenGL that you're finished using the vertex arrayattribute
    glDisable(GL_POINT_SMOOTH); // stop the smoothing to make the points circular
}

void drawLine(GLfloat start_x, GLfloat start_y, GLfloat end_x, GLfloat end_y) // we have to take care of too many cases :P
{
    if (start_x > end_x) {
        std::swap(start_x, end_x);
        std::swap(start_y, end_y);
    }
    int dx = end_x - start_x;
    int dy = end_y - start_y;

    //when line is steeper than 1
    if (abs(dy) > abs(dx)) {
        if (dy > 0) { //when line has m>1 && m<=infinity
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
                drawPixel(x, y);
            }
        } else { //when it spills over to second quadrant but still has abs(m) > 1
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
                drawPixel(x, y);
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
                drawPixel(x, y);
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
                drawPixel(x, y);
            }
        }
    }
}

void drawCirclePixel(int x, int y, int origin_x, int origin_y)
{
    GLfloat _x = x;
    GLfloat _y = y;

    GLfloat _ox = origin_x;
    GLfloat _oy = origin_y;
    // draw pixel 8 times for covering symmetry of the circle
    drawPixel(_x+_ox, _y+_oy);
    drawPixel(_x*-1+_ox, _y+_oy);
    drawPixel(_x+_ox, _y*-1+_oy);
    drawPixel(_x*-1+_ox, _y*-1+_oy);
    drawPixel(_y+_ox, _x+_oy);
    drawPixel(_y*-1+_ox, _x+_oy);
    drawPixel(_y+_ox, _x*-1+_oy);
    drawPixel(_y*-1+_ox, _x*-1+_oy);

}

void drawCircle(int radius, int origin_x, int origin_y)
{
    int x = 0;
    int y = radius;

    int d = 1 - radius;
    int deltaE = 3;
    int deltaSE = -2*radius + 5;
    drawCirclePixel(x, y, origin_x, origin_y);

    while(y > x) {
        if (d < 0) { //east pixel
            d += deltaE;
            deltaE += 2;
            deltaSE += 2;
        } else { //south-east pixel
            d += deltaSE;
            deltaE += 2;
            deltaSE += 4;
            y--;
        }
        x++;
        drawCirclePixel(x, y, origin_x, origin_y);
    }
}

int main( void )
{
    GLFWwindow *window;

    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity(); // same as above comment


    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        // drawLine(200, 200, 200, 600);
        // drawLine(200, 200, 300, 500);

        drawLine(300, 600, 350, 200);
        // drawLine(200, 200, 400, 200);
        //
        // drawLine(200, 200, 400, 300);
        // drawLine(200, 200, 400, 500);
        //
        // drawLine(0, 400, 200, 200);
        //
        drawPixel(300, 400);
        drawCircle(200, 300, 400);

        // Swap front and back buffers
        glfwSwapBuffers( window );

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
