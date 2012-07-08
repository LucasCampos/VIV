#ifndef VECTORND_H
#define VECTORND_H

#include "precision.h"
#include <cmath>

class Vector2D
	{
	private:
		real x,y;
	public:
		Vector2D(void){x = y = 0.0;}
		Vector2D(real a,real b){x = a;y = b;}
		real norm (void) const {return(sqrt(x*x + y*y));}
		real norm2(void) const {return (x*x + y*y);}
		void scalar(real a){x*=a;y*=a;}
		real getX() const {return (x);}
		real getY() const {return (y);}
		void setX(real a){x = a;}
		void setY(real b){y = b;}
		void normalize() { real vecNorm = norm(); x/=vecNorm; y/=vecNorm;};

		void rotate (real angle);

		Vector2D operator+(const Vector2D & vecB) const;
		Vector2D operator-(const Vector2D & vecB) const;
		Vector2D & operator=(const Vector2D & vecB);
		Vector2D & operator+=(const Vector2D & vecB);
		Vector2D & operator-=(const Vector2D & vecB);
		
		friend real dotProd(Vector2D A,Vector2D B);
		friend Vector2D operator*(real a,const Vector2D & vecB);				
	};

class Vector3D
	{
	private: 
		real x,y,z;
	public:
		Vector3D(void){x=y=z=0.0;}
		Vector3D(real x1,real x2,real x3){x=x1;y=x2;z=x3;}
		real norm (void) const{return(sqrt(x*x+y*y+z*z));}
		real norm2 (void)  const{return (x*x+y*y+z*z);}
		void scalar(real a){x*=a;y*=a;z*=a;}
		real getX() const {return x;}
		real getY() const {return y;}
		real getZ() const {return z;}
		void setX(real a){x = a;}
		void setY(real b){y = b;}
		void setZ(real c){z = c;}
		Vector3D operator+(const Vector3D & vecB) const;
		Vector3D operator-(const Vector3D & vecB) const;
		Vector3D operator^(const Vector3D & vecB) const;
		Vector3D & operator=(const Vector3D & vecB);
		Vector3D & operator+=(const Vector3D & vecB);
		Vector3D & operator-=(const Vector3D & vecB);
		friend Vector3D operator*(real a,const Vector3D & vecB);
		friend real dotProd(Vector3D A,Vector3D B);
};

//=======================================================================
//Methods of the Vector2D class
//=======================================================================

void Vector2D :: rotate(real angle) {
	Vector2D R = *this;

	x = R.x*cos(angle) -R.y*sin(angle);
	y = R.y*cos(angle) +R.x*sin(angle);

}

Vector2D Vector2D ::  operator+(const Vector2D & VectorB) const
{
	Vector2D C;
	C.x = x + VectorB.x;
	C.y = y + VectorB.y;
	return (C);
}

Vector2D Vector2D :: operator-(const Vector2D & VectorB) const
{
	Vector2D C;
	C.x = x - VectorB.x;
	C.y = y - VectorB.y;
	return (C);
}

Vector2D  & Vector2D :: operator=(const Vector2D & VectorB)
{
	x = VectorB.x;
	y = VectorB.y;
	return (*this);
}

Vector2D & Vector2D :: operator+=(const Vector2D & vecB)
{
	x+=vecB.x;
	y+=vecB.y;
	return(*this);
}

Vector2D & Vector2D :: operator-=(const Vector2D & vecB)
{
	x+=vecB.x;
	y+=vecB.y;
	return(*this);
}


Vector2D operator*(real a,const Vector2D & vecB)//Method friend
{
	Vector2D T;
	T.x = a*vecB.x;
	T.y = a*vecB.y;
	return(T);
}

real dotProd (Vector2D A, Vector2D B)//Method friend
{
	real c = A.x*B.x;
	c+=(A.y*B.y);
	return (c);
}

//=============================================================
//Methods of the Vector3D CLASS
//============================================================
Vector3D Vector3D :: operator+(const Vector3D & vecB) const
{
 	Vector3D C;
	C.x = x + vecB.x;     
	C.y = y + vecB.y;
	C.z = z + vecB.z;
	return (C);
}

Vector3D Vector3D :: operator-(const Vector3D & vecB) const{
	
	Vector3D C;
	C.x = x - vecB.x;     
	C.y = y - vecB.y;
	C.z = z - vecB.z;
	return (C);
}

//Determina o produto Vetorial!!!!!!!!	
Vector3D Vector3D :: operator^(const Vector3D & vecB) const
{
	Vector3D C;
	C.x = ((y*vecB.z) - (z*vecB.y));
	C.y = ((z*vecB.x) - (x*vecB.z));
	C.z = ((x*vecB.y) - (y*vecB.x));
	return (C);
}

Vector3D  & Vector3D :: operator=(const Vector3D & VectorB)
{
	x = VectorB.x;
	y = VectorB.y;
	z = VectorB.z;
	return (*this);
}

Vector3D & Vector3D :: operator+=(const Vector3D & vecB)
{
	x+=vecB.x;
	y+=vecB.y;
	z+=vecB.z;
	return(*this);
}

Vector3D & Vector3D :: operator-=(const Vector3D & vecB)
{
	x+=vecB.x;
	y+=vecB.y;
	z+=vecB.z;
	return(*this);
}

Vector3D operator*(real a,const Vector3D & vecB)
{
	Vector3D T;
	T.x = a*vecB.x;
	T.y = a*vecB.y;
	T.z = a*vecB.z;
	return(T);
}

real dotProd (Vector3D A, Vector3D B)//Method friend
{
	real c = A.x*B.x;
	c+=(A.y*B.y);
	c+=(A.z*B.z);
	return (c);
}

#endif
