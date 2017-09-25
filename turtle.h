#ifndef TURTLE_H
#define TURTLE_H

#include "graphics.h"

/// Turle-like graphics for interpreting l system generated code
class Turtle
{
private:
    int thickness;
    float branch_contraction_ratio;
    float tapering_ratio;
    int len;
    float rotation;
    int x, y;
    tuple<GLfloat, GLfloat, GLfloat, GLfloat> color;
    stack<tuple<int, int, float, int>> states;
    Graphics graphics;

    //We use the famous Mersenne Twister PRNG algorithm for giving random colors
    //to the leaves
    random_device rd;
    mt19937 gen;

public:

    /// Default empty constructor for Turtle.
    /// Initialise all values and set the color to brown for drawing the tree trunk

    Turtle()
    {
        thickness = 20;
        branch_contraction_ratio = 0.8;
        tapering_ratio = 0.95;
        len = 200/40;
        rotation = PI / 2;
        color = make_tuple(0.4, 0.2, 0, 1); // brown color
        graphics.setColor(color);
        graphics.setThickness(thickness);

        gen.seed(rd());
    }

    /// As thicnkess of trunks/branches decreases as we go up the tree, we can decrement
    /// it as we draw the tree. Set the thickness for drawPixel(..)

    void setThickness(int val = 0)
    {
        if (val == 0) {
            thickness *= branch_contraction_ratio;
            val = thickness;
        }
        if (val == -1) {
            thickness *= tapering_ratio;
            val = thickness;
        }
        graphics.setThickness(val);
    }

    /// Move the Turtle to the desired co-ordinate on the viewport

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

    /// Rotate the Turtle's pointer through an angle.

    void rotate(float angle)
    {
        rotation += angle;
    }

    /// Part of the language of many L-systems involve back-tracking to a state.
    /// Here we can save the current state of the Turtle (current co-ordinate and rotation)
    /// so we can move back to it later. States are stored in an std::stack of tuples.

    void saveState()
    {
        states.push(make_tuple(x, y, rotation, thickness));
    }

    /// Pop the last saved stack from the stack and set the Turtle to that state
    /// for future line drawing. Output an error message in case the stack is empty.

    void restoreState()
    {
        if (states.empty()) {
            cout << "No saved state available" << endl;
            return;
        }
        tuple<int, int, float, int> temp = states.top();
        states.pop();
        x = get<0>(temp);
        y = get<1>(temp);
        rotation = get<2>(temp);
        thickness = get<3>(temp);
    }

    /// Draw a simple polygonal leaf consisting of a circle and three triangles.
    /// The leaf is drawn at the current state of the Turtle.

    void drawLeaf(int season)
    {
        graphics.setThickness(1); // Leaves are always of thickness 1
        uniform_real_distribution<> dis(0, 1);
        int yDelta = 5;
        int rad = 2;
        float radian = rotation - PI/2;

        GLfloat green = 1;
        GLfloat red = 0;
        GLfloat blue = 0;
        if (season == 0) { // Summer/Spring
            green = dis(gen);
            red = 0;
        }
        else if (season == 1) { // Autumn
            green = dis(gen);
            red = dis(gen);
        }
        else if (season == 2) { // Winter
            if (dis(gen) < 0.9)
                return;
            green = dis(gen)/2;
            red = 0;
        }
        graphics.setColor(make_tuple(red, green, blue, 1));

        graphics.drawCircle(x, y, rad);

        float piDiv = 4;

        graphics.drawLine(x - (rad)*cos(radian), y + rad*sin(radian), x + yDelta*sin(radian), y + yDelta*cos(radian));
        graphics.drawLine(x + rad*cos(radian), y - rad*sin(radian), x + yDelta*sin(radian), y + yDelta*cos(radian));

        graphics.drawLine(x - (rad)*cos(radian+PI/piDiv), y + rad*sin(radian+PI/piDiv), x + yDelta*sin(radian+PI/piDiv), y + yDelta*cos(radian+PI/piDiv));
        graphics.drawLine(x + rad*cos(radian+PI/piDiv), y - rad*sin(radian+PI/piDiv), x + yDelta*sin(radian+PI/piDiv), y + yDelta*cos(radian+PI/piDiv));

        graphics.drawLine(x - (rad)*cos(radian-PI/piDiv), y + rad*sin(radian-PI/piDiv), x + yDelta*sin(radian-PI/piDiv), y + yDelta*cos(radian-PI/piDiv));
        graphics.drawLine(x + rad*cos(radian-PI/piDiv), y - rad*sin(radian-PI/piDiv), x + yDelta*sin(radian-PI/piDiv), y + yDelta*cos(radian-PI/piDiv));

        graphics.setColor(color);
        graphics.setThickness(thickness); // Restore original thickness after leaf drawing complete
    }
};

#endif
