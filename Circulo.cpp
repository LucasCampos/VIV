#include "Circulo.h"

Circulo::Circulo()
{

    radius = 5.0f;
    x=0.0f;
    y=0.0f;
	nVertices = 20;

}

Circulo::Circulo(GLfloat radius)
{

    Circulo();
    this -> radius = radius;
}

Circulo::Circulo(GLfloat x, GLfloat y)
{

    radius = 5.0f;
    this-> x = x;
    this-> y = y;
}

Circulo::Circulo(GLfloat x,GLfloat y, GLfloat radius)
{

    this -> x = x;
    this -> y = y;
    this -> radius = radius;
}

void Circulo::Draw()
{
    //red, green,  blue
    glColor3f(red, green,  blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);

    float deltaAngle = (2.0f*3.14159)/(nVertices);
    for (float angle = 0.0f; angle<=(2.0f*3.14159); angle+=deltaAngle)
    {
        glVertex2f(x + cosf(angle) * radius, y + sinf(angle) * radius);
    }
    glEnd();

}

void Circulo::setVertices(int n)
{

    nVertices = n;

}

void Circulo::setColor(float red, float green, float blue)
{

    this-> red = red;
    this-> blue = blue;
    this-> green = green;
}

int Circulo::getVertices()
{
    return nVertices;
}

void Circulo::addX(GLfloat add)
{
    x+=add;
}
void Circulo::addY(GLfloat add)
{
    y+=add;
}

void Circulo::setX(GLfloat newX)
{
    x=newX;
}
void Circulo::setY(GLfloat newY)
{
    y=newY;
}

GLfloat Circulo::getX()
{
    return x;
}

GLfloat Circulo::getY()
{
    return y;
}

GLfloat Circulo::getRadius()
{
    return radius;
}


void Circulo::setRadius(GLfloat val)
{
    radius = val;
}

void Circulo::printColor(){

	std::cout <<"Red: " << red << " Blue: " << blue << " Green: " << green << std::endl;

} 
