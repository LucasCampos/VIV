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
		Circulo(GLreal radius);
		Circulo(GLreal x, GLreal y);
		Circulo(GLreal x, GLreal y, GLreal radius);
		void Draw();

};

void Circulo::Draw()
{
	//red, green,  blue
	glColor3f(red, green,  blue);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);

	real deltaAngle = (2.0f*3.14159)/(20);
	for (real angle = 0.0f; angle<=(2.0f*3.14159); angle+=deltaAngle)
	{
		glVertex2f(x + cosf(angle) * radius, y + sinf(angle) * radius);
	}
	glEnd();

}

#endif
