#include "Turtle.h"
#include <unistd.h>

#define WIDTH 800
#define HEIGHT 800

void drawPattern(string sentence)
{
    Turtle turtle;
    float angle = (PI / 180) * 22.5;

    turtle.translate(WIDTH/2, 0);

    for (int i = 0; i < sentence.length(); i++) {
        char current = sentence[i];

        if((i%2) == 0) {
            cout << i << endl;
            // turtle.changeColor();
            // turtle.reduceThickness();
        }

        if (current == 'F') {
            turtle.setThickness(3);
            turtle.draw();
            turtle.setThickness(2);
        } else if (current == '+') {
            turtle.rotate(-angle);
        } else if (current == '-') {
            turtle.rotate(angle);
        } else if (current == '[') {
            turtle.saveState();
        } else if (current == ']') {
            turtle.restoreState();
        } else if (current == 'X' && sentence[i] != '[') {
            // turtle.changeColor();
            turtle.drawLeaf();
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
    const int generations = 7;

    srand(time(NULL));

    // OpenGL initialisation code

    if(glfwInit() == false) {
        fprintf(stderr, "Error initializing GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "IS F311 Computer Graphics Project", NULL, NULL);

    if(!window) {
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
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapInterval(0);

    string sentence = "X";
    int depth = 1;
    Turtle turtle;

    // Render loop for the OpenGL window. The scene is redrawn on every refresh.

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        if (depth < generations) {
            generateString(sentence, depth++);
        }
        drawPattern(generatedString);

        glfwSwapBuffers(window);
        glfwPollEvents();
        usleep(0.25 * 1e6);
        // sleep(1);
        // for (long long i = 0; i < 200000000; i++);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
