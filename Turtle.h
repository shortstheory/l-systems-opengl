#ifndef TURTLE_H
#define TURTLE_H

#include "Graphics.h"

class Turtle
{
private:
    int thickness;
    int len;
    float rotation;
    int x, y;
    tuple<GLfloat, GLfloat, GLfloat, GLfloat> color;
    stack<tuple<int, int, float>> states;
    Graphics graphics;

    //We use the famous Mersenne Twister PRNG algorithm for giving random colors
    //to the leaves
    random_device rd;
    mt19937 gen;

public:

    /// Default empty constructor for Turtle.
    /// Initialises all values and sets the color to brown for drawing the tree trunk

    Turtle()
    {
        thickness = 1;
        len = 200/40;
        rotation = PI / 2;
        color = make_tuple(0.4, 0.2, 0, 1); // brown color
        graphics.setColor(color);
        graphics.setThickness(thickness);

        gen.seed(rd());
    }

    /// Colors in OpenGL range from 0 -> 1. Hence to change intensity, we have to map
    /// the desired color to these values.

    void changeColor()
    {
        GLfloat red = (float)(rand()%100) / (float)100;
        GLfloat green = (float)(rand()%100) / (float)100;
        GLfloat blue = (float)(rand()%100) / (float)100;
        color = make_tuple(red, green, blue, 1);
        graphics.setColor(color);
    }

    /// As thicnkess of trunks/branches decreases as we go up the tree, we can decrement
    /// it as we draw the tree. Sets the thickness for drawPixel(..)

    void reduceThickness()
    {
        thickness--;
        graphics.setThickness(thickness);
    }

    /// Simply move the Turtle to the desired co-ordinate on the viewport

    void translate(int x_target, int y_target)
    {
        x = x_target;
        y = y_target;
    }

    /// Sets the angle of inclination of the Turtle's pointer with respect to the x-axis.

    void setAngle(float angle)
    {
        rotation = angle;
    }

    /// Draws a line given the starting point, length of the line, and the
    /// current rotation of the Turtle's pointer.

    void draw()
    {
        pair<int, int> temp = graphics.drawVector(x, y, len, rotation);
        x = temp.first;
        y = temp.second;
        // cout<<"reached "<<x<<", "<<y<<endl;
    }

    /// Rotates the Turtle's pointer through an angle.

    void rotate(float angle)
    {
        rotation += angle;
    }

    /// Part of the language of many L-systems involve back-tracking to a state.
    /// Here we can save the current state of the Turtle (current co-ordinate and rotation)
    /// so we can move back to it later. States are stored in an std::stack of tuples.

    void saveState()
    {
        states.push(make_tuple(x, y, rotation));
    }

    /// Pops the last saved stack from the stack and sets the Turtle to that state
    /// for future line drawing. Outputs an error message in case the stack is empty.

    void restoreState()
    {
        if (states.empty()) {
            cout << "No saved state available" << endl;
            return;
        }
        tuple<int, int, float> temp = states.top();
        states.pop();
        x = get<0>(temp);
        y = get<1>(temp);
        rotation = get<2>(temp);
    }

    /// Draws a simple polygonal leaf consisting of a circle and three triangles.
    /// The leaf is drawn at the current state of the Turtle.

    void drawLeaf()
    {
        uniform_real_distribution<> dis(0, 1);
        int yDelta = 5;
        int rad = 2;
        float radian = rotation - PI/2;

        // graphics.setColor(make_tuple(dis(gen), dis(gen), dis(gen), 1));
        graphics.setColor(make_tuple(dis(gen), dis(gen), 0, 1));

        graphics.drawCircle(x, y, rad);

        float piDiv = 4;

        graphics.drawLine(x - (rad)*cos(radian), y + rad*sin(radian), x + yDelta*sin(radian), y + yDelta*cos(radian));
        graphics.drawLine(x + rad*cos(radian), y - rad*sin(radian), x + yDelta*sin(radian), y + yDelta*cos(radian));

        graphics.drawLine(x - (rad)*cos(radian+PI/piDiv), y + rad*sin(radian+PI/piDiv), x + yDelta*sin(radian+PI/piDiv), y + yDelta*cos(radian+PI/piDiv));
        graphics.drawLine(x + rad*cos(radian+PI/piDiv), y - rad*sin(radian+PI/piDiv), x + yDelta*sin(radian+PI/piDiv), y + yDelta*cos(radian+PI/piDiv));

        graphics.drawLine(x - (rad)*cos(radian-PI/piDiv), y + rad*sin(radian-PI/piDiv), x + yDelta*sin(radian-PI/piDiv), y + yDelta*cos(radian-PI/piDiv));
        graphics.drawLine(x + rad*cos(radian-PI/piDiv), y - rad*sin(radian-PI/piDiv), x + yDelta*sin(radian-PI/piDiv), y + yDelta*cos(radian-PI/piDiv));

        graphics.setColor(color);
    }
};

#endif
