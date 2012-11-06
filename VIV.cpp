#include <GL/glfw.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "include/polygon.h"
#include "include/colorPicker.h"

using namespace std;

bool stopped = false;

typedef boost::shared_ptr<std::ifstream>  IFStreamPtrType;
typedef boost::shared_ptr<DrawablePolygon>  DrawablePolygonPtrType;

struct polyReader {
	int _pointsPerPoly;
	IFStreamPtrType _in;
	bool _jump;
	vector<DrawablePolygonPtrType> _poly;

	polyReader(int points, char* filename, bool jump, vector<DrawablePolygonPtrType> poly): 
		_pointsPerPoly(points), _in(new std::ifstream(filename)), _poly(poly) {
		};

	void updatePoly() {
		int size = _poly.size();
		real x,y;
		for (int i=0; i<size; i++) {
			*_in >> x >> y;
			bool isEnd = ((x == 0) && (y == 0)) || _in->eof();
			//if (((x == 0) && (y == 0)) || _in->eof()) exit(0);
			if (!isEnd){
				_poly[i]->_pos = Vector2D(x,y);
				_poly[i]->_vertex.clear();
				for (int j =0; j<_pointsPerPoly; j++) {
					*_in >> x >> y;
					isEnd = ((x == 0) && (y == 0)) || _in->eof();
					if (!isEnd) 
						_poly[i]->_vertex.push_back(Vector2D(x,y));
				}
			}
		}
	};


	void draw() {
		int size = _poly.size();
		//cout << size << endl;
		for (int i=0; i<size; i++) 
			_poly[i]->draw();
	};

	void restart(){

		_in->clear();
		_in->seekg(0, ios::beg);

		cout << "Reinicia\n";
	};
};

//Global Variables
real box = 10.0f;
real distanciaMinima = 0;
int window_width = 800;
int window_height = 800;
int timeOut = 600;
int timeZero = 0;
double delay = 0.01;
bool extendedBox = true;
int counter = 0;

vector<polyReader> poly;

//Function declaration
void Init(void);
void Shut_Down(int return_code);
void Main_Loop(void);
void Draw(void);
void GLFWCALL WindowSize( int width, int height );
void GLFWCALL keyhandler(int key, int action);
void TakeParameters(int argc, char* argv[]);
void SimpleHelp();
void LargeHelp();

int main(int argc, char* argv[])
{
	TakeParameters(argc, argv);
	timeZero = time(NULL);
	srand(time(NULL));

	Init();
	Main_Loop();
	Shut_Down(0);
}



void TakeParameters(int argc, char* argv[]){

	bool hasOne = false;

	if (argc == 1) {
		SimpleHelp();
		exit(0);
	}

	for (int i=0;i<argc; i++){
		if (string(argv[i]) == "-p"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);
			int points = atoi(argv[i+3]);

			vector<DrawablePolygonPtrType> p;
			Color c = ColorPicker::getColor();


			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new DrawablePolygon(c.red, c.green, c.blue));
				polygon->_vertex.resize(points);
				p.push_back(polygon);
			}

			poly.push_back(polyReader(points, file, true,p));

			hasOne = true;

		} else if (string(argv[i]) == "-s"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);

			vector<DrawablePolygonPtrType> p;
			Color c = ColorPicker::getColor();

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Square(c.red, c.green,c.blue));
				p.push_back(polygon);
			}

			poly.push_back(polyReader(4, file, true,p));

			hasOne = true;

		} else if (string(argv[i]) == "-t"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);

			vector<DrawablePolygonPtrType> p;
			Color c = ColorPicker::getColor();


			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Triangle(c.red, c.green, c.blue));
				p.push_back(polygon);
			}

			poly.push_back(polyReader(3, file, true,p));

			hasOne = true;

		} else if (string(argv[i]) == "-c"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);
			real radius = atof(argv[i+3]);

			vector<DrawablePolygonPtrType> p;
			Color c = ColorPicker::getColor();

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Circle(radius, c.red,c.green,c.blue));
				p.push_back(polygon);
			}

			poly.push_back(polyReader(0, file, true,p));

			hasOne = true;

		} else if (string(argv[i]) == "-d"){

			if (argc <= i+1) {
				cout << "Incomplete -d flag" << endl;
				exit(1);
			}
			delay = atof(argv[i+1]);

		} else if (string(argv[i]) == "-b") {

			if (argc <= i+1) {
				cout << "Incomplete -b flag" << endl;
				exit(1);
			}
			box = atof(argv[i+1]);	
		} else if (string(argv[i]) == "-h") {
			SimpleHelp();
			exit(0);
		} else if (string(argv[i]) == "-H") {
			LargeHelp();
			exit(0);
		} else if (string(argv[i]) == "-t") {
			timeOut = atoi(argv[i+1]);
		} else if (string(argv[i]) == "-e"){
			extendedBox = false;
		}
	}
	if (!hasOne) {
		LargeHelp();
		exit(1);
	}
}

void Init(void)
{

	if (glfwInit() != GL_TRUE)
		Shut_Down(1);
	// 800 x 800, 16 bit color, no depth, alpha or stencil buffers, windowed
	if (glfwOpenWindow(window_width, window_height, 5, 6, 5, 0, 0, 0, GLFW_WINDOW) != GL_TRUE)
		Shut_Down(1);
	glfwSetWindowTitle("VIV - VIV Is a Visualizer");
	glfwSetWindowSizeCallback( WindowSize );
	glfwSetKeyCallback( keyhandler );
	glViewport(0, 0, (GLsizei)window_width, (GLsizei)window_height);
	glLoadIdentity();

	if (extendedBox) {
		glOrtho (-box, box, -box, box, 0, 1);
	} else {
		glOrtho (0, box, 0, box, 0, 1);
	}

	glMatrixMode(GL_MODELVIEW);
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

}

void Shut_Down(int return_code)
{
	glfwTerminate();
	exit(return_code);
}

void Main_Loop(void)
{
	while (true) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		if (!stopped)
			for (int i=0; i<poly.size(); i++)
				poly[i].updatePoly();

		// clear the buffer
		// draw the figure
		if ((time(NULL) - timeZero) > timeOut) Shut_Down(0);
		Draw();
	}
}

void Draw(void)
{
	for (int i=0; i<poly.size(); i++) 
		poly[i].draw();
	// swap back and front buffers
	glFinish();
	glfwSwapBuffers();
	glfwSleep(delay);
}

void GLFWCALL keyhandler( int key, int action )
{

	if( action != GLFW_PRESS )
	{
		return;
	}

	switch (key)
	{
		case GLFW_KEY_ESC:
			Shut_Down(0);
			break;
		case 'P':
			if (stopped) {
				stopped=false;
				cout << "Continue\n";
			}else{
				stopped = true;
				cout << "Stopped\n";
			}
			break;
		case 'R':
			for (int i=0; i<poly.size(); i++)
				poly[i].restart();

	}
}


void GLFWCALL WindowSize( int width, int height ){

	window_height = height;
	window_width = width;
	glViewport(0, 0, (GLsizei)window_width, (GLsizei)window_height);	
}

void SimpleHelp() {

	cout << "\nLucas Campos, Scientific Computation Lab, Physics Department, UFPE\n";
	cout << "Version 1.02Beta\n\n";	
	std::cout << "Commands: \n\n";
	std::cout << " -h\n\tShows simple help\n";
	std::cout << " -H\n\tShows larger help\n";
	std::cout << " -E\n\tShows larger help and an example\n";
	std::cout << " -p filename Quantity nPoints\n\tAdds a new type of polygon, with Quantity polygons per frame. The first line of each polygon is its center, and the following nPoints are the vertex.\n\n";
	std::cout << " -s filename Quantity nPoints\n\tAdds a new square kind. Each square must have five lines on the file, for each frame.\n\n";
	std::cout << " -t filename Quantity nPoints\n\tAdds a new triangle kind. Each triangle must have four lines on the file, for each frame.\n\n";
	std::cout << " -c filename Quantity Radius\n\tAdds a new circle kind, all with radius Radius. Each must have one line on the file.\n\n";
	std::cout << " -d Delay\n\tChooses a delay between frames. Smaller delays offer faster visualization. Default delay is 0.001\n\n";
	std::cout << " -b Box\n\tChooses the box size. The standard it 10\n\n";
	std::cout << " -e Extended Box\n\tIf set, the coodinate system is from 0 to box, instead of -box to box\n\n";
}

void LargeHelp() {


	SimpleHelp();
	cout << "\nOn the program, at least on kind of polygon must be present.\n";
	cout << "To add a new kind, the syntax is:\n";
	cout << " -c nameFile Quantity Radius\n";
	cout << "For instance:\n";
	cout << " -c transRusso.dat 352 0.5\n";

	cout << "An example of use:\n";
	cout << "\tnine -s transannealingDuro.dat 351 0.5 -s transannealingRusso.dat 1 0.5 -b 40\n";

	cout << "\nVIV requires Boost, OpenGL and GLFW.\n";

}
