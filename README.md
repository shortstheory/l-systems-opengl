# Introduction

L-systems are used for modelling the growth of plants through recursion. This project is an implmentation of an L-system in C++11 to render a tree in OpenGL.

# Requirements

* GPU with OpenGL 3.3+ support
* Ubuntu

# Dependencies

* GLFW - required for creating an OpenGL context. Instructions for installing can be found [here](http://www.glfw.org/download.html).
* CMake - install using ```sudo apt install cmake```

# Project Structure

This project consists of the following files:

* ```graphics.h``` - class containing primitive (line and circle) drawing methods
* ```turtle.h``` - class implementing turtle like graphics for interpreting L-system generated code
* ```primitives.cpp``` - demonstrates the primitive drawing method by rendering a bicycle wheel
* ```tree.cpp``` - procedurally generates a tree changing colours with the seasons using an L-system

# Buidling

Clone or unzip the project to your favorite folder. Then run:

```
mkdir build
cd build
cmake ..
make
./tree
```

To run the primitive drawing demonstration program, make sure your active directory is set to ```build/``` and run ```./primitives```.

# Contributors

* Arnav Dhamija
* Pranav Asthana
