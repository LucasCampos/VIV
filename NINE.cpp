#include <GL/glfw.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "include/polygon.h"

using namespace std;

bool parar = false;

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
			bool notEnd = (x != 0) && (y != 0);
			if (notEnd) {
				_poly[i]->_pos = Vector2D(x,y);
				_poly[i]->_vertex.clear();
			}
			for (int j =0; j<_pointsPerPoly; j++) {
				*_in >> x >> y;
				if(notEnd) 
					_poly[i]->_vertex.push_back(Vector2D(x,y));
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

//Variaveis Globais
real box = 10.0f;
real distanciaMinima = 0;
int window_width = 800;
int window_height = 800;
int timeOut = 600;
int timeZero = 0;
double delay = 0.01;
bool jump = true;
bool extendedBox = true;
int counter = 0;

vector<polyReader> poly;

//Declaracao de Funcoes
void Init(void);
void Shut_Down(int return_code);
void Main_Loop(void);
void Draw(void);
void GLFWCALL TamanhoJanela( int width, int height );
void GLFWCALL keyhandler(int key, int action);
void pegaParametros(int argc, char* argv[]);
void Plota();
void AjudaSimples();
void AjudaGrande();
void AjudaGrandeComExemplo();

int main(int argc, char* argv[])
{
	pegaParametros(argc, argv);
	timeZero = time(NULL);
	srand(time(NULL));

	Init();
	Main_Loop();
	Shut_Down(0);
}



void pegaParametros(int argc, char* argv[]){

	bool temUm = false;

	if (argc == 1) {
		AjudaSimples();
		exit(0);
	}

	for (int i=0;i<argc; i++){
		if (string(argv[i]) == "-p"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);
			int points = atoi(argv[i+3]);

			vector<DrawablePolygonPtrType> p;

			for (int j=0; j<N; j++) {
				//DrawablePolygonPtrType polygon(new DrawablePolygon(uniform(), uniform(), uniform()));
				DrawablePolygonPtrType polygon(new DrawablePolygon(1.0, 1.0, 1.0));
				polygon->_vertex.resize(points);
				p.push_back(polygon);
			}

			poly.push_back(polyReader(points, file, true,p));

			temUm = true;

		} else if (string(argv[i]) == "-s"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);

			vector<DrawablePolygonPtrType> p;

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Square(1.0, .0,.0));
				//DrawablePolygonPtrType polygon(new Square(uniform(), uniform(), uniform()));
				p.push_back(polygon);
			}

			poly.push_back(polyReader(4, file, true,p));

			temUm = true;

		} else if (string(argv[i]) == "-t"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);

			vector<DrawablePolygonPtrType> p;

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Triangle(0, 1.0, 1.0));
				p.push_back(polygon);
			}

			poly.push_back(polyReader(3, file, true,p));

			temUm = true;

		} else if (string(argv[i]) == "-c"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);
			real radius = atof(argv[i+3]);

			vector<DrawablePolygonPtrType> p;

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Circle(radius, 1.0,.0,.0));
				p.push_back(polygon);
			}

			poly.push_back(polyReader(0, file, true,p));
	
			temUm = true;

		} else if (string(argv[i]) == "-d"){

			if (argc <= i+1) {
				cout << "Parametros -d incompletos\n";
				exit(1);
			}
			delay = atof(argv[i+1]);

		} else if (string(argv[i]) == "-b") {

			if (argc <= i+1) {
				cout << "Parametros -b incompletos\n";
				exit(1);
			}
			box = atof(argv[i+1]);	
		} else if (string(argv[i]) == "-h") {
			AjudaSimples();
			exit(0);
		} else if (string(argv[i]) == "-H") {
			AjudaGrande();
			exit(0);
		} else if (string(argv[i]) == "-E") {
			AjudaGrandeComExemplo();
			exit(0);
		} else if (string(argv[i]) == "-t") {
			timeOut = atoi(argv[i+1]);
		} else if (string(argv[i]) == "-j"){
			jump = false;
		} else if (string(argv[i]) == "-e"){
			extendedBox = false;
		}
	}
	if (!temUm) {
		AjudaGrande();
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
	glfwSetWindowSizeCallback( TamanhoJanela );
	glMatrixMode(GL_PROJECTION);
	glfwSetKeyCallback( keyhandler );
	glViewport(0, 0, (GLsizei)window_width, (GLsizei)window_height);
	glLoadIdentity();

	if (extendedBox) {
		glOrtho (-box, box, -box, box, 0, 1);
	} else {
		glOrtho (0, box, 0, box, 0, 1);
	}

	glMatrixMode(GL_MODELVIEW);
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
		if (!parar)
			for (int i=0; i<poly.size(); i++)
				poly[i].updatePoly();

		glfwSleep(delay);
		// clear the buffer
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear(GL_COLOR_BUFFER_BIT);
		// draw the figure
		if ((time(NULL) - timeZero) > timeOut) Shut_Down(0);
		glLoadIdentity();
		Draw();
	}
}

void Draw(void)
{
	for (int i=0; i<poly.size(); i++) 
		poly[i].draw();
	// swap back and front buffers
	//DrawConnections(distanciaMinima);
	glFinish();
	glfwSwapBuffers();
}
/*
void DrawConnections (real param){

	real param2=param*param;
	glColor3f(1.0f,1.0f,1.0f);		
	for (int i=0; i<Ntotal; i++){
		for (int j=i+1; j<Ntotal; j++){
			real delx = circulos[i].getX() - circulos[j].getX();
			real dely = circulos[i].getY() - circulos[j].getY();
			real r2 = delx*delx + dely*dely;
			if (r2 <= param2) {
				glBegin(GL_LINES);
				glVertex2f(circulos[i].getX(), circulos[i].getY());
				glVertex2f(circulos[j].getX(), circulos[j].getY());
				glEnd();
			}
		}
	}
}
*/

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
			if (parar) {
				parar=false;
				cout << "Continua\n";
			}else{
				parar = true;
				cout << "Parado\n";
			}
			break;
		case 'R':
			for (int i=0; i<poly.size(); i++)
				poly[i].restart();

	}
}


void GLFWCALL TamanhoJanela( int width, int height ){

	window_height = height;
	window_width = width;
	glViewport(0, 0, (GLsizei)window_width, (GLsizei)window_height);	
}

void AjudaSimples() {

	cout << "\nLucas Campos, Laboratório de Computação Científica, Departamento de Física, UFPE\n";
	cout << "Em caso de bugs, por favor mande um email para lqcc@df.ufpe.br\n\n";  
	cout << "Versao 1.0Beta\n\n";	
	std::cout << "Comandos: \n\n";
	std::cout << " -h\n\tMostra o diálogo de ajuda\n\n"; 
	std::cout << " -H\n\tMostra o diálogo de ajuda extenso\n\n";
	std::cout << " -E\n\tMostra o diálogo de ajuda extenso com um exemplo de arquivo\n\n";
	std::cout << " -p filename Quantidade Pontos\n\tAdiciona um novo tipo de poligono, com Quantidade poligonos por frame e Pontos vertices por poligono. A primeira linha de cada poligono sera o centr. O arquivo deve ter entao Pontos+1 linhas por poligono\n\n";
	std::cout << " -s filename Quantidade\n\tAdiciona um novo tipo de quadrado, com Quantidade quadrados por frame. A cada quadrado deve ser associado 5 linhas do arquivo\n\n";
	std::cout << " -t filename Quantidade\n\tAdiciona um novo tipo de triangulo, com Quantidade triangulos por frame. A cada triangulo deve ser associado 4 linhas do arquivo\n\n";
	std::cout << " -s filename Quantidade Raio\nAdiciona um novo tipo de circulo, com Quantidade circulos por frame. A cada circulo deve ser associado 1 linha do arquivo\t\n\n";
	std::cout << " -d Delay\n\tEscolhe um delay. Delays menores oferecem visualizações mais rápidas. Por padrão é 0.001\n\n";
	std::cout << " -b Box\n\tEscolhe o tamanho da caixa. Por padrao e 10\n\n";
	std::cout << " -e Extended Box\n\tA caixa vai de 0 a box, ao inves de -box a box.\n\n";
	std::cout << " -j Jump\n\tAdicione caso nao seja necessario pular uma linha entre os frames.\n\n";
}

void AjudaGrande() {


	AjudaSimples();
	cout << "\nO programa deve ter ao menos um tipo de poligono\n";
	cout << "Para adicionar um novo tipo, a sintaxe e:\n";
	cout << " -s NomeDoArquivo Quantidade Raio\nou\n";
	cout << "Por Exemplo:\n";
	cout << " -s transRusso.dat 352 0.5\n";

	cout << "Um exemplo de uso:\n";
	cout << "\tnine -s transannealingDuro.dat 351 0.5 -s transannealingRusso.dat 1 0.5 -b 40\n";

	cout << "\nNINE requer que o Boost, OpenGL e o GLFW estejam instalados.\n";

}

void AjudaGrandeComExemplo() {

	AjudaGrande();
	cout << "\nOs arquivos devem estar organizado em duas colunas, x e y.\n";
	cout << "Caso -j nao esteja definido, deve haver uma linha em branco entre os as coordenadas de cada frame.\n\n";

	cout << "2.5219482421875e+01   3.6102264404297e+01      \\\n";  
	cout << "1.5872397422791e+01   3.0927572250366e+01      |\n";
	cout << "3.2085777282715e+01   3.5613407135010e+01      |---- 1º Frame\n";
	cout << "3.2100597381592e+01   2.1326478958130e+01      |\n";
	cout << "3.2701339721680e+01   1.1126631498337e+00      /\n\n";

	cout << "2.4824047088623e+01   3.6052524566650e+01      \\\n";       
	cout << "1.5643037796021e+01   3.1283767700195e+01      |\n";        
	cout << "3.2052001953125e+01   3.5472850799561e+01      |---- 2º Frame\n";        
	cout << "3.1981531143188e+01   2.1602472305298e+01      |\n";        
	cout << "3.2481254577637e+01   1.2583947181702e+00      /\n\n";

}    
