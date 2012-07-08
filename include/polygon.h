#ifndef POLYGON_H
#define POLYGON_H

#include "vectorND.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

struct int2{
	int i,j;
	int2(): i(0), j(0){};
	int2(int i1, int i2): i(i1), j(i2){};
};


class Polygon {

	public:
		Vector2D _pos;
		std::vector<Vector2D> _vertex;
		std::vector<Vector2D> _edges;

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

		void generateEdges() {
			_edges.clear();
			const int nVertex =_vertex.size(); 
			for(int i=0; i<nVertex; i++) {
				Vector2D T = _vertex[(i+1)%nVertex] - _vertex[i];
				_edges.push_back(T);
			}
		}




};

class Square: public Polygon {

	public:

		Square(){

			addVertex(Vector2D(-.5,-.5));
			addVertex(Vector2D(-.5, .5));
			addVertex(Vector2D( .5, .5));
			addVertex(Vector2D( .5,-.5));
		}

		Square(real size) {
			addVertex(Vector2D(-size,-size));
			addVertex(Vector2D(-size, size));
			addVertex(Vector2D( size, size));
			addVertex(Vector2D( size,-size));
		}

		Square(Vector2D pos, real size): Polygon(pos) {
			addVertex(Vector2D(-size,-size));
			addVertex(Vector2D(-size, size));
			addVertex(Vector2D( size, size));
			addVertex(Vector2D( size,-size));
		}


};

class Triangle: public Polygon{

	public:
		Triangle() { 
			Vector2D T(0,sqrt(3.0)/3.0);
			addVertex(T);
			T.rotate(2*pi/3.0);
			addVertex(T);
			T.rotate(2*pi/3.0);
			addVertex(T);
		}

		Triangle(real size) { 
			Vector2D T(0,sqrt(3.0)/3.0*size);
			addVertex(T);
			T.rotate(2*pi/3.0);
			addVertex(T);
			T.rotate(2*pi/3.0);
			addVertex(T);
		}

		Triangle(Vector2D pos, real size): Polygon(pos) { 
			Vector2D T(0,sqrt(3.0)/3.0*size);
			addVertex(T);
			T.rotate(2*pi/3.0);
			addVertex(T);
			T.rotate(2*pi/3.0);
			addVertex(T);
		}
};


//////////////////////////////////DECLARACAO DAS FUNCOES AUXILIARES////////////////////////////////////////
void projectPolygon(const Vector2D& axis, const Polygon& polygon, real* min, real* max);
bool isOverlapping(const Polygon& poly1, const Polygon& poly2);
real intervalDistance(real minA, real maxA, real minB, real maxB);

///////////////////////////////FUNCOES DE AJUDA////////////////////////////
inline void projectPolygon(const Vector2D& axis, const Polygon& polygon, real* min, real* max) {
	// To project a point on an axis use the dot product
	double dot = dotProd(polygon._pos + polygon._vertex[0],axis);
	*min = dot;
	*max = dot;

	const int nVertex = polygon._vertex.size();

	for (int i = 1; i < nVertex; i++) {
		dot = dotProd(polygon._pos+polygon._vertex[i],axis);
		if (dot < *min) {
			*min = dot;
		} else if (dot> *max) {
			*max = dot;
		}

	}
}

// Calculate the distance between [minA, maxA] and [minB, maxB]
// The distance will be negative if the intervals overlap
real intervalDistance(real minA, real maxA, real minB, real maxB) {

	if (minA < minB) {
		return minB - maxA;
	} else {
		return minA - maxB;
	}
}

bool isOverlapping(const Polygon& poly1, const Polygon& poly2) {

	const bool overlap = true;
	const bool notOverlap = false;

	//std::cout << "Entrou em overlaps\n";

	const int nEdges = poly1._edges.size();
	for (int i=0; i<nEdges; i++) {

		Vector2D axis = poly1._edges[i];
		axis.normalize();

		real max1,min1;
		projectPolygon(axis, poly1, &min1, &max1);

		real max2,min2;
		projectPolygon(axis, poly2, &min2, &max2);

		const real interval = intervalDistance(min1,max1,min2,max2);

	//	std::cout << "Interval = " << interval << std::endl;
		if (interval>=0) return notOverlap;
	}
	return overlap;
}

#endif
