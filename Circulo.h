#ifndef CLObject_H
#define CLObject_H
#include <GL/glfw.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

class Circulo
{

public:
    Circulo();
    Circulo(GLfloat radius);
    Circulo(GLfloat x, GLfloat y);
    Circulo(GLfloat x, GLfloat y, GLfloat radius);
    void Draw();
    void addX(GLfloat add);
    void addY(GLfloat add);
    void setX(GLfloat newX);
    void setY(GLfloat newY);
    void setColor(GLfloat red, GLfloat green, GLfloat blue);
    GLfloat getX();
    GLfloat getY();
    GLfloat getRadius();
	void printColor();
    void setRadius(GLfloat val);
    void setVertices(int n);
    int getVertices();
    GLfloat red, green,  blue;
protected:
    GLfloat x,y,radius;
    int nVertices;

};


#endif
