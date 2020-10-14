#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "matriz.h"
/******************************************************************/

int main(void){

  Matriz* A = matriz_create();
  matriz_print(A);
  Matriz* B = matriz_create();
  matriz_print(A);
  Matriz* C = matriz_add(A,B);
  matriz_print(C);
  matriz_destroy(C);
  C = matriz_multiply(A, B); 
  matriz_print(C);
  matriz_destroy(C);
  matriz_destroy(A);
  matriz_destroy(B);

  return 0;

}
/******************************************************************/
