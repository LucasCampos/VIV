#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H

#include "precision.h"

struct Color {
	real red, blue, green;
	Color(): red(0), blue(0), green(0) {};
	Color(real r, real g, real b): red(r), blue(b), green(g) {};
};

class ColorPicker{
	static int current;
	public:
	static Color getColor() {

		switch(current) {
			case 0:
				current++;
				return Color(1.0,0,0);
				break;
			case 1: 
				return Color(0.0,1.0,.0);
				current++;
				break;
			case 2: 
				current++;
				return Color(0.0,0.0,1.0);
				break;
			case 3: 
				current++;
				return Color(1.0,1.0,1.0);
				break;
			default: return Color (uniform(), uniform(), uniform());
		}
	}
};

int ColorPicker::current = 0;

#endif
