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

public:
    Turtle()
    {
        thickness = 1;
        len = 200/32;
        rotation = PI / 2;
        color = make_tuple(0.4, 0.2, 0, 1); // brown color
        graphics.setColor(color);
        graphics.setThickness(thickness);
    }

    void changeColor()
    {
        GLfloat red = (float)(rand()%100) / (float)100;
        GLfloat green = (float)(rand()%100) / (float)100;
        GLfloat blue = (float)(rand()%100) / (float)100;
        color = make_tuple(red, green, blue, 1);
        graphics.setColor(color);
    }

    void reduceThickness()
    {
        thickness--;
        graphics.setThickness(thickness);
    }

    void translate(int x_target, int y_target)
    {
        x = x_target;
        y = y_target;
    }

    void setAngle(float angle)
    {
        rotation = angle;
    }

    void draw()
    {
        pair<int, int> temp = graphics.drawVector(x, y, len, rotation);
        x = temp.first;
        y = temp.second;
        // cout<<"reached "<<x<<", "<<y<<endl;
    }

    void rotate(float angle)
    {
        rotation += angle;
    }

    void saveState()
    {
        states.push(make_tuple(x, y, rotation));
    }

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

    void drawLeaf()
    {
        int yDelta = 5;
        int rad = 2;
        float radian = rotation - PI/2;

        graphics.setColor(make_tuple(0, 1, 0, 0.1));

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
