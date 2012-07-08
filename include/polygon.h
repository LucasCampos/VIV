#ifndef POLYGON_H
#define POLYGON_H

#include "vectorND.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <GL/glfw.h>
#include <cmath>
#include <cstdlib>

class Polygon {

	public:
		Vector2D _pos;
		std::vector<Vector2D> _vertex;

		Polygon(Vector2D pos): _pos(pos){};
		Polygon(){};
		void addVertex(Vector2D vertex){
			_vertex.push_back(vertex);
		}

		void translate(const Vector2D& T) {
			_pos += T;
		}

		void writeOnFile(std::ofstream& file) const {
			const int nVertex =_vertex.size(); 
			for(int i=0; i<nVertex; i++) 
				file << _vertex[i].getX() + _pos.getX() << "   " << _vertex[i].getY() + _pos.getY() << std::endl;
		}

		void rotate(real angle){
			const int nVertex =_vertex.size(); 
			for(int i=0; i<nVertex; i++) 
				_vertex[i].rotate(angle);
		}


};

class DrawablePolygon: public Polygon {

	public:

		real _red, _green, _blue;

		DrawablePolygon(real red, real green, real blue): _red(red), _green(green), _blue(blue) {};
		DrawablePolygon(): _red(1.0), _green(1.0), _blue(1.0) {};

		virtual void draw() {
			
			int size = _vertex.size();
			
			glColor3f(_red, _green, _blue);
			glBegin(GL_POLYGON);

				for (int i=0; i<size; i++) {

					const Vector2D pV = _pos + _vertex[i];
					glVertex2f(pV.getX(), pV.getY());
				}

			glEnd();
		};
};

class Square: public DrawablePolygon {

	public:

		Square(real red, real green, real blue): DrawablePolygon (red,green,blue){

			addVertex(Vector2D(-.5,-.5));
			addVertex(Vector2D(-.5, .5));
			addVertex(Vector2D( .5, .5));
			addVertex(Vector2D( .5,-.5));
		}

		Square(real size,real red, real green, real blue): DrawablePolygon (red,green,blue) {
			addVertex(Vector2D(-size,-size));
			addVertex(Vector2D(-size, size));
			addVertex(Vector2D( size, size));
			addVertex(Vector2D( size,-size));
		}

		void draw() {
			
			int size = _vertex.size();
			
			glColor3f(_red, _green, _blue);
			glBegin(GL_QUADS);

				for (int i=0; i<size; i++) {

					const Vector2D pV = _pos + _vertex[i];
					glVertex2f(pV.getX(), pV.getY());
				}

			glEnd();
		};

};

class Triangle: public DrawablePolygon{

	public:
		Triangle(real red, real green, real blue): DrawablePolygon (red,green,blue) {
			Vector2D T(0,sqrt(3.0)/3.0);
			addVertex(T);
			T.rotate(2*pi/3.0);
			addVertex(T);
			T.rotate(2*pi/3.0);
			addVertex(T);
		}

		Triangle(real size,real red, real green, real blue): DrawablePolygon (red,green,blue) {
			Vector2D T(0,sqrt(3.0)/3.0*size);
			addVertex(T);
			T.rotate(2*pi/3.0);
			addVertex(T);
			T.rotate(2*pi/3.0);
			addVertex(T);
		}
		
		void draw() {
			
			int size = _vertex.size();
			
			glColor3f(_red, _green, _blue);
			glBegin(GL_TRIANGLES);

				for (int i=0; i<size; i++) {

					const Vector2D pV = _pos + _vertex[i];
					glVertex2f(pV.getX(), pV.getY());
				}

			glEnd();
		};

};

class Circulo: public DrawablePolygon {

	public:
		real _radius;

		Circulo(real radius,real red, real green, real blue): DrawablePolygon (red,green,blue), _radius(radius) {};
		void draw(){
			//red, green,  blue
			glColor3f(_red, _green,  _blue);
			glBegin(GL_TRIANGLE_FAN);
				glVertex2f(_pos.getX(), _pos.getY());

				const real deltaAngle = (2.0f*3.14159)/(20);
				for (real angle = 0.0f; angle<=(2.0f*3.14159); angle+=deltaAngle)
				{
					glVertex2f(_pos.getX() + cosf(angle) * _radius, _pos.getY() + sinf(angle) * _radius);
				}
			glEnd();

		};
};

#endif
