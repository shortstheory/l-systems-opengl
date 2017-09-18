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

using namespace std;

#define WIDTH 800
#define HEIGHT 900
#define PI 3.1415

char sentence[] = "X";
string rule = "F[+F]F[-F]F";
tuple<GLfloat, GLfloat, GLfloat, GLfloat> color = make_tuple(1, 1, 1, 1);
// string rule = "FF+[+F-F-F]-[-F+F+F]";
int len = 200;
// int thickness = 1;
// F -> FF+[+F-F-F]-[-F+F+F]

stack<tuple<int, int, float>> v;

class Graphics
{
    int thickness;
    tuple<GLfloat, GLfloat, GLfloat, GLfloat> color;// = make_tuple(1, 1, 1, 1);

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
        void setThickness(int thickness)
        {
            this -> thickness = thickness;
        }

        void setColor(tuple<GLfloat, GLfloat, GLfloat, GLfloat> color)
        {
            this -> color = color;
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
                        drawPixel(x, y, thickness, color);
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

        pair<int, int> drawVector(int x0, int y0, int len, float angle)
        {
            int a = x0 + (int) (len * cos(angle));
            int b = y0 + (int) (len * sin(angle));
            drawLine(x0, y0, a, b);
            return make_pair(a, b);
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
};

class Turtle
{
    int thickness;
    int len;
    float rotation;
    int x, y;
    tuple<GLfloat, GLfloat, GLfloat, GLfloat> color;
    stack<tuple<int, int, float>> states;

    Graphics * graphics = new Graphics();

    public: Turtle()
    {
        thickness = 20;
        len = 200/32;
        rotation = PI / 2;
        color = make_tuple(0.4, 0.2, 0, 1); // brown color
        graphics -> setColor(color);
        graphics -> setThickness(thickness);
    }

    void changeColor()
    {
        GLfloat red = (float)(rand()%100) / (float)100;
        GLfloat green = (float)(rand()%100) / (float)100;
        GLfloat blue = (float)(rand()%100) / (float)100;
        // cout << red << green << blue;
        color = make_tuple(red, green, blue, 1);
        graphics -> setColor(color);
    }

    void reduceThickness()
    {
        thickness--;
        graphics -> setThickness(thickness);
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
        pair<int, int> temp = graphics -> drawVector(x, y, len, rotation);
        x = temp.first;
        y = temp.second;
        cout<<"reached "<<x<<", "<<y<<endl;
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
        if (states.empty()){
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
        int xDelta = len / 2;//5;
        int yDelta = len * 2;//30;
        int rad = len;//12;

        float radian = PI/180*rotation;

        graphics -> setColor(make_tuple(0, 1, 0, 0.1));

        graphics -> drawCircle(x, y, rad);

        float piDiv = 4;
        // //top
        graphics -> drawLine(x - (rad)*cos(radian), y + rad*sin(radian), x + yDelta*sin(radian), y + yDelta*cos(radian));
        graphics -> drawLine(x + rad*cos(radian), y - rad*sin(radian), x + yDelta*sin(radian), y + yDelta*cos(radian));

        graphics -> drawLine(x - (rad)*cos(radian+PI/piDiv), y + rad*sin(radian+PI/piDiv), x + yDelta*sin(radian+PI/piDiv), y + yDelta*cos(radian+PI/piDiv));
        graphics -> drawLine(x + rad*cos(radian+PI/piDiv), y - rad*sin(radian+PI/piDiv), x + yDelta*sin(radian+PI/piDiv), y + yDelta*cos(radian+PI/piDiv));

        graphics -> drawLine(x - (rad)*cos(radian-PI/piDiv), y + rad*sin(radian-PI/piDiv), x + yDelta*sin(radian-PI/piDiv), y + yDelta*cos(radian-PI/piDiv));
        graphics -> drawLine(x + rad*cos(radian-PI/piDiv), y - rad*sin(radian-PI/piDiv), x + yDelta*sin(radian-PI/piDiv), y + yDelta*cos(radian-PI/piDiv));

        graphics -> setColor(color);
    }

};


void drawPattern(string sentence)
{
    Turtle * turtle = new Turtle();
    // float rotation = PI / 2;
    float angle = (PI / 180) * 22.5;

    // translate(WIDTH/2, 0);
    // int x0 = WIDTH / 2;
    // int y0 = 0;
    turtle -> translate(WIDTH/2, 0);

    for (int i = 0; i < sentence.length(); i++) {
        char current = sentence[i];

        if((i%2) == 0) {
            cout << i << endl;
            // turtle -> changeColor();
            // turtle -> reduceThickness();
        }

        if (current == 'F') {
            turtle -> draw();
        } else if (current == '+') {
            turtle -> rotate(-angle);
        } else if (current == '-') {
            turtle -> rotate(angle);
        } else if (current == '[') {
            turtle -> saveState();
        } else if (current == ']') {
            turtle -> restoreState();
        } else if (current == 'X') {
            // turtle -> changeColor();
            turtle -> drawLeaf();
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
    srand(time(NULL));
    if(glfwInit() == false)
    {
        fprintf(stderr, "Error initializing GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Example", NULL, NULL);
    if(!window)
    {
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
    glClearColor(1, 1, 1, 1);
    glClear( GL_COLOR_BUFFER_BIT );
    glfwSwapInterval(0);
    // generateString(sentence, 5);

    // turtle -> draw();
    // turtle -> draw();
    // turtle -> rotate(-PI/2);
    // turtle -> draw();

    int depth = 1;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        if (depth < 6)
            generateString(sentence, depth++);
        drawPattern(generatedString);

        // turtle -> draw();
        // turtle -> rotate(-PI/4);

        glfwSwapBuffers(window);
        glfwPollEvents();
        sleep(1);
        // for(long long i = 0; i < 10000000000; i++);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
