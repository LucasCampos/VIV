#ifndef PRECISION_H
#define PRECISION_H
#include <GLFW/glfw3.h>

#include <cstdlib>
#define real GLfloat

const real pi = 3.141592;

real uniform() { return rand()/(real)RAND_MAX;}
real uniformD() { return 2.0*(rand()/(real)RAND_MAX-0.5);}

#endif
