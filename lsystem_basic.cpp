#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <tuple>
#include <stack>
#include <iostream>
#include <math.h>

using namespace std;

#define WIDTH 800
#define HEIGHT 600
#define PI 3.1415

char sentence[] = "X";
string rule = "F[+F]F[-F]F";
// string rule = "FF+[+F-F-F]-[-F+F+F]";
int len = 200;
// F -> FF+[+F-F-F]-[-F+F+F]

stack<tuple<int, int, float>> v;

void drawPixel(int x, int y)
{
    GLfloat vertex[] = {(GLfloat) x, (GLfloat) y};
    glEnableClientState(GL_VERTEX_ARRAY);
    glPointSize(1);
    glVertexPointer(2, GL_FLOAT, 0, vertex);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void drawLine(int start_x, int start_y, int end_x, int end_y) // we have to take care of too many cases :P
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

pair<int, int> drawVector(int x0, int y0, int len, float angle)
{
    int a = x0 + (int) (len * cos(angle));
    int b = y0 + (int) (len * sin(angle));
    drawLine(x0, y0, a, b);
    return make_pair(a, b);
}

void drawCirclePixels(int x0, int y0, int x, int y)
{
    drawPixel(x + x0, y + y0);
    drawPixel(-x + x0, y + y0);
    drawPixel(x + x0, -y + y0);
    drawPixel(-x + x0, -y + y0);
    drawPixel(y + x0, x + y0);
    drawPixel(-y + x0, x + y0);
    drawPixel(y + x0, -x + y0);
    drawPixel(-y + x0, -x + y0);
}

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

void drawPattern(string sentence)
{
    float rotation = PI / 2;
    float angle = (PI / 180) * 22.5;

    // translate(WIDTH/2, 0);
    int x0 = WIDTH / 2;
    int y0 = 0;

    for (int i = 0; i < sentence.length(); i++) {
        char current = sentence[i];

        if (current == 'F') {
            pair<int, int> temp = drawVector(x0, y0, len, rotation);
            x0 = temp.first;
            y0 = temp.second;

            // y0 += len;
        } else if (current == '+') {
            // rotate(angle);
            rotation -= angle;
        } else if (current == '-') {
            // rotate(-angle);
            rotation += angle;
        } else if (current == '[') {
            v.push(make_tuple(x0, y0, rotation));
            // push();
        } else if (current == ']') {
            tuple<int, int, float> temp = v.top();
            v.pop();
            x0 = get<0>(temp);
            y0 = get<1>(temp);
            rotation = get<2>(temp);
        }
    }
}

string generatedString;

void generateString(string sentence, int depth)
{
    if(depth == 0) {
        cout<<sentence;
        generatedString = sentence;
        return;
    }

    len *= 0.5;
    string nextSentence;

    for (int i = 0; i < sentence.length(); i++) {
        if (sentence[i] == 'X') {
            nextSentence += "F-[[X]+X]+F[+FX]-X";
            continue;
        }
        if (sentence[i] == 'F') {
            nextSentence += "FF";
            continue;
        }
        nextSentence += sentence[i];
    }
    cout<<nextSentence<<depth<<endl;
    generateString(nextSentence, depth - 1);
}


int main()
{
    if (glfwInit() == false) {
        fprintf(stderr, "Error initializing GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        fprintf(stderr, "Error while creating a window\n");
        return -1;
    }
    glfwMakeContextCurrent(window);
    glViewport(0.0f, 0.0f, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // glClear( GL_COLOR_BUFFER_BIT );
    // glfwSwapInterval(0);

    generateString(sentence, 5);


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        drawPattern(generatedString);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
