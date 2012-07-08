#include <GL/glfw.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "Circulo.h"

using namespace std;

int Ntotal = 0, Ntotalconstantes = 0;
vector <int> N;
vector <int> Nconstantes;
vector <float> raio; 
vector <float> raioconstantes;
int nVertices = 20;
float box = 10.0f;
float distanciaMinima = 0;
Circulo* circulos;
Circulo* constantes;
int window_width = 800;
int window_height = 800;
ifstream entradas[10]; 
ifstream entradasConstantes[10]; 
int timeOut = 600;
int timeZero = 0;
double delay = 0.01;
bool parar = false;
bool jump = true;

bool extendedBox = false;
bool rewinding = false;
//freetype::font_data our_font;
/*
char *filename;
bool plotafinal;
*/
int counter = 0;
 
void Init(void);
void Shut_Down(int return_code);
void Main_Loop(void);
void Draw(void);
void DrawConnections(float param);
inline void Ler(void);
void GLFWCALL TamanhoJanela( int width, int height );
void GLFWCALL keyhandler(int key, int action);
void pegaParametros(int argc, char* argv[]);
void Plota();
inline float Uni();
void leConstantes();
void Reinicia();
void AjudaSimples();
void AjudaGrande();
void AjudaGrandeComExemplo();
void deOnde();

int main(int argc, char* argv[])
{
	pegaParametros(argc, argv);

	timeZero = time(NULL);

	srand(time(NULL));
	int Npercorrido = 0;
	circulos = new Circulo[Ntotal];

	cout << timeOut << endl;

	//for (int i=0; i<argc; i++) cout << string(argv[i]) << endl;	
 
	for (int i=0; i<N.size(); i++) {
		switch(i){
			case 0:
				for (int j=0; j<N[i]; j++) 
					circulos[Npercorrido+j].setColor(1.0f,0.0f,0.0f);
				break;
			case 1:
				for (int j=0; j<N[i]; j++) 
					circulos[Npercorrido+j].setColor(0.0f,1.0f,0.0f);
				break;

			case 2:
				for (int j=0; j<N[i]; j++) 
					circulos[Npercorrido+j].setColor(0.0f,0.0f,1.0f);
				break;

			case 3:
				for (int j=0; j<N[i]; j++) 
					circulos[Npercorrido+j].setColor(.30f,0.30f,0.0f);
				break;
			default:
				float red = Uni(), green = Uni(), blue = Uni();
				for (int j=0; j<N[i]; j++) 
					circulos[Npercorrido+j].setColor(red,green,blue);
				break;
		}
		for (int j=0; j<N[i]; j++){		
			circulos[Npercorrido + j].setVertices(nVertices);
			circulos[Npercorrido + j].setRadius(raio[i]);
		}
		Npercorrido +=N[i];
	}
	Npercorrido = 0;
	constantes = new Circulo[Ntotalconstantes];	
 
	for (int i=0; i<Nconstantes.size(); i++) {
		switch(i){
			case 0:
				for (int j=0; j<Nconstantes[i]; j++) 
					constantes[i*Npercorrido+j].setColor(0.2f,0.50f,.30f);
				break;
			case 1:
				for (int j=0; j<Nconstantes[i]; j++) 
					constantes[i*Npercorrido+j].setColor(0.30f,.0f,0.30f);
				break;
			default:
				float red = Uni(), green = Uni(), blue = Uni();
				for (int j=0; j<Nconstantes[i]; j++) 
					constantes[i*Npercorrido+j].setColor(red,green,blue);
				break;

		}
		for (int j=0; j<Nconstantes[i]; j++) {
			constantes[Npercorrido + j].setVertices(nVertices);
			constantes[Npercorrido + j].setRadius(raioconstantes[i]);
		}
			
		Npercorrido +=Nconstantes[i];
	}
	
    //our_font.init("Test.ttf", 12);

	if (Nconstantes.size() > 0) leConstantes();
 	Init();
  	Main_Loop();
  	Shut_Down(0);
}

inline float Uni() {
	return (float)rand()/RAND_MAX;
}

void pegaParametros(int argc, char* argv[]){

	bool temUm = false;

	if (argc == 1) {
		AjudaSimples();
		exit(0);
	}

	for (int i=0;i<argc; i++){
		if (string(argv[i]) == "-a"){
			if (argc <= (i+3)) {
				cout << "Parametros -a incompletos\n";
				exit(1);
			} 
			entradas[N.size()].open(argv[i+1]);
			Ntotal+=atoi(argv[i+2]);
			N.push_back(atoi(argv[i+2]));
			raio.push_back(atof(argv[i+3]));
			temUm = true;

		} else if (string(argv[i]) == "-c") {

			if (argc <= (i+3)) {
				cout << "Parametros do -c incompletos\n";
				exit(1);
			} 
			entradasConstantes[Nconstantes.size()].open(argv[i+1]);
			Nconstantes.push_back(atoi(argv[i+2]));
			Ntotalconstantes+=atoi(argv[i+2]);
			raioconstantes.push_back(atof(argv[i+3]));

		} else if (string(argv[i]) == "-d"){

			if (argc <= i+1) {
				cout << "Parametros -d incompletos\n";
				exit(1);
			}
			delay = atof(argv[i+1]);

		} else if (string(argv[i]) == "-v") {

			if (argc <= i+1) {
				cout << "Parametros -h incompletos\n";
				exit(1);
			}
			nVertices = atoi(argv[i+1]);
		} else if (string(argv[i]) == "-r") {

			if (argc <= i+1) {
				cout << "Parametros -r incompletos\n";
				exit(1);
			}
			distanciaMinima = atof(argv[i+1]);	
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
		} else if (string(argv[i]) == "-D"){
			deOnde();
			exit(0);
		} else if (string(argv[i]) == "-j"){
			jump = false;
		} else if (string(argv[i]) == "-e"){
			extendedBox = true;
		}/*else if (string(argv[i]) == "-p"){
			plotafinal = true;
			window_width = atof(argv[i+1]);
			window_height = atof(argv[i+2]);
			filename = (char*) malloc (sizeof(argv[i+3]));
			filename = argv[i+3];
		}*/
	}
	if (!temUm) {
		AjudaGrande();
		exit(1);
	}
}

void leConstantes() {
	float x, y;
	char charLeitor[100], partex[50],partey[50];

	int Npercorrido = 0;

	for (int i=0; i< Nconstantes.size(); i++){
	  	for (int j=0; j< Nconstantes[i]; j++) {	
			entradasConstantes[i].getline(charLeitor,100);
			sscanf(charLeitor,"%s%s",partex,partey);
			x = atof(partex);
			y = atof(partey);
			//cout << partex << "   " << partey << endl;
			constantes[Npercorrido + j].setX(x);
			constantes[Npercorrido + j].setY(y);
	  	}
		Npercorrido += Nconstantes[i];
	}
}
	

inline void Ler(void){	
	float x, y;
	char charLeitor[100], partex[50],partey[50];

	int Npercorrido = 0;

	for (int i=0; i< N.size(); i++){
	  	for (int j=0; j< N[i]; j++) {	
            /*Essa parte � necess�ria para que se pare ao t�rmino da simula��o
            Ao inv�s de fechar*/
			if (entradas[i].eof())
			  break;
			entradas[i].getline(charLeitor,100);
			sscanf(charLeitor,"%s%s",partex,partey);
			x = atof(partex);
			y = atof(partey);
			circulos[Npercorrido + j].setX(x);
			circulos[Npercorrido + j].setY(y);
	  	}
		if (jump && !entradas[i].eof()) entradas[i].getline(charLeitor,100);
		Npercorrido += N[i];
	}
	
	if (rewinding) {
	  for (int i=0; i< N.size(); i++){
	    entradas[i].seekg(-2*(N[i]+jump), ios::cur);
	  }
	}
}
 
void Init(void)
{
 
  if (glfwInit() != GL_TRUE)
    Shut_Down(1);
  // 800 x 600, 16 bit color, no depth, alpha or stencil buffers, windowed
  if (glfwOpenWindow(window_width, window_height, 5, 6, 5,
                     0, 0, 0, GLFW_WINDOW) != GL_TRUE)
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
  // the time of the previous frame
  //double old_time = glfwGetTime();
  // this just loops as long as the program runs
	//for (int i=0; i<Ntotal; i++){
	//	cout << circulos[i].getX() << "   " << circulos[i].getY()  << "   " << circulos[i].getRadius() << endl;
	//
        for (int i=0; i<N.size(); i++) {
			entradas[i].seekg(0, ios::beg);
		}
	while (true) {
	{
			if (glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
			  Shut_Down(0);
			if (!parar) Ler();
			glfwSleep(delay);
			// clear the buffer
				glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
			glClear(GL_COLOR_BUFFER_BIT);
			// draw the figure
			if ((time(NULL) - timeZero) > timeOut) Shut_Down(0);
				glLoadIdentity();
			//freetype::print(our_font, (int)(0.1*window_width), (int)(0.1*window_height), 	"t = %7.3f", box);
				glPopMatrix();
			Draw();
	}
}
}
 
void Draw(void)
{
  // reset view matrix
	glLoadIdentity();
	glfwSetWindowSize( window_width, window_width );
	if (Nconstantes.size() > 0) 
		for (int i=0; i<Ntotalconstantes; i++) {
			constantes[i].Draw();
			//cout << constantes[i].getX() << "   " << constantes[i].getY() << "   " << constantes[i].getRadius() << "   " << constantes[i].getVertices() << endl;
			//constantes[i].printColor();
		};


	for (int i=0; i<Ntotal; i++) circulos[i].Draw();
    // swap back and front buffers
	DrawConnections(distanciaMinima);
	glFinish();
	//if (plotafinal) 
	//	Plota(); 
    glfwSwapBuffers();
}

void DrawConnections (float param){

	float param2=param*param;
    glColor3f(1.0f,1.0f,1.0f);		
	for (int i=0; i<Ntotal; i++){
		for (int j=i+1; j<Ntotal; j++){
			float delx = circulos[i].getX() - circulos[j].getX();
			float dely = circulos[i].getY() - circulos[j].getY();
			float r2 = delx*delx + dely*dely;
			if (r2 <= param2) {
				glBegin(GL_LINES);
				glVertex2f(circulos[i].getX(), circulos[i].getY());
				glVertex2f(circulos[j].getX(), circulos[j].getY());
				glEnd();
			}
		}
	}
}

void Reinicia(){
  
        for (int i=0; i<N.size(); i++) {
			entradas[i].clear();
			entradas[i].seekg(0, ios::beg);
		}
		
	cout << "Reinicia\n";
}

void GLFWCALL keyhandler( int key, int action )
{

    if( action != GLFW_PRESS )
    {
        return;
    }

    switch (key)
    {
    case 'P':
        if (parar) {
	  parar=false;
	  cout << "Continua\n";
	}else{
	  parar = true;
	  cout << "Parado\n";
	}
        break;
    case 'V':
        if (rewinding) {
	  rewinding=false;
	  cout << "Indo\n";
	}else{
	  rewinding = true;
	  cout << "Voltando\n";
	}
        break;
    case 'R':
      Reinicia();

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
	cout << "Versão 0.3Beta\n\n";	
	std::cout << "Comandos: \n\n";
	std::cout << " -h\n\tMostra o diálogo de ajuda\n\n"; 
	std::cout << " -H\n\tMostra o diálogo de ajuda extenso\n\n";
	std::cout << " -E\n\tMostra o diálogo de ajuda extenso com um exemplo de arquivo\n\n";
	std::cout << " -a NomeDoArquivo Quantidade Raio \n\tAdiciona um novo tipo que se move (ex: Partículas de Yukawa)\n\n";
	std::cout << " -c NomeDoArquivo Quantidade Raio \n\tAdiciona um novo tipo que não de move (ex: Pinnings)\n\n";
	std::cout << " -d Delay\n\tEscolhe um delay. Delays menores oferecem visualizações mais rápidas. Por padrão é 0.001\n\n";
	std::cout << " -b Box\n\tEscolhe o tamanho da caixa. Por padrão é 10\n\n";
	std::cout << " -v Vertices\n\tEscolhe o número de vértices por partícula. Por padrão, 20\n\n";
	std::cout << " -e Extended Box\n\tA caixa vai de -box a box, ao invés de 0 a box. Por padrão, false\n\n";
	std::cout << " -j Jump\n\tSe true, pula uma linha entre os frames. Por padrão, true\n\n";
	std::cout << " -r Distancia Minima\n\tDesenha liga��es caso as part�culas tenha distancia menor que a distancia minimia\n\n";
}

void AjudaGrande() {
	
	
	AjudaSimples();
	cout << "\nO programa deve ter ao menos um tipo móvel\n";
	cout << "Para adicionar um novo tipo, a sintaxe é:\n";
	cout << " -a NomeDoArquivo Quantidade Raio\nou\n";
	cout << " -c NomeDoArquivo Quantidade Raio\n\n";
	cout << "Por Exemplo:\n";
	cout << " -a transRusso.dat 352 0.5\n";
	cout << " -c pinnings.dat 64 0.3\n\n";

	cout << "Um exemplo de uso:\n";
	cout << "\tnine -a transannealingDuro.dat 351 0.5 -a transannealingRusso.dat 1 0.5 -b 40\n";

	cout << "\nNINE requer que o OpenGL e o GLFW estejam instalados.\n";

}

void AjudaGrandeComExemplo() {

	AjudaGrande();
	cout << "\nOs arquivos devem estar organizado em duas colunas, x e y.\n";
	cout << "Caso -j não esteja definido, deven haver uma linha em branco entre os as coordenadas de cada frame.\n\n";
	
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

void deOnde() {

	cout << "VIV - VIV Is a Visualizer\n";
	cout << "NINE - NINE In Not ELEVEN\n";
	cout << "V + IV = NINE\n";
	cout << "Thus NINE Is a Visualizer\n";
	cout << "V + I + V = ELEVEN\n\n";

}
