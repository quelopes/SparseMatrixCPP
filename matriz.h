#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*PROGRAMA PARA CRIAR UMA MATRIZ ESPARSA BIDIMENSIONAL ATRAVES DE LISTA ENCADEADA*/
/* author: Raquel Lopes Costa */

struct matriz{
  struct matriz* right;
  struct matriz* below;
  int line;
  int column;
  float info;
};
typedef struct matriz Matriz;

/******************************************************************/
Matriz* create_empty(int L, int C);
Matriz* matriz_create(void);
/* FORMA AS CELULAS LINHAS E COLUNAS*/
Matriz* forma(Matriz* p, int m, int n);
/*FORNECE O ELEMENTO CONTIDO NAS COORDENADAS X, Y*/
float getElem(Matriz* m, int x, int y);
/*ADICIONA UM ELEMENTO NA MATRIZ*/
void setElem (Matriz* p, int x, int y, float elem);
/*conta o numero de linhas e colunas*/
void get_nLineCols(Matriz* p, int &nline, int &ncols);
/*IMPRIME A MATRIZ*/
void matriz_print (Matriz* p);
/*SOMA DUAS MATRIZES*/
Matriz* matriz_add (Matriz* m, Matriz* n);
/*MULTIPLICA DUAS MATRIZES*/
Matriz* matriz_multiply(Matriz* m, Matriz* n);
/*DESTROI UMA MATRIZ*/
void matriz_destroy(Matriz* m);
static void destroy_line(Matriz* x);

/******************************************************************/
/* FUNCAO PARA CRIAR MATRIZ */

Matriz* matriz_create(void){
  int x, y, L, C;
  float v;
  int l, c;
  scanf("%d, %d",&L, &C);
  Matriz* p;
  p = create_empty(L, C);
  do {
    scanf("%d, %d, %f",&x, &y, &v);
    if (x==0 && y==0 && v==0)
    break;
    setElem(p,x,y,v);
  }while (1);

  return p;

}
/******************************************************************/
/* FUNCAO PARA CRIAR O NO CABECA E INSERIR VALORES
line(-1) E column (-1) */

Matriz* create_empty(int L, int C){

  /*cria no cabeca*/
  Matriz* novo;
  novo = (Matriz*)malloc(sizeof(Matriz));
  novo->line = -1;
  novo->column = -1;
  novo->right = novo;
  novo->below = novo;

  /*retorna o ponteiro novo atualizado com os nos cabeca de linhas(-1) e colunas (-1)*/
  novo = forma(novo, L, C);
  return novo;

}
/******************************************************************/
/* FUNCAO AUXILIAR PARA CRIAR lista encadeada com nos cabeca*/

Matriz* forma(Matriz* p, int L, int C){
  /*lista encadeada para referencia de linhas*/
  Matriz* r;
  r = p;
  for (int i =0; i<L; i++){
    Matriz* novo = (Matriz*)malloc(sizeof(Matriz));
    r->below = novo;
    novo->line= -1;
    novo->right = novo;
    r=novo;
  }
  r->below = p;
  /*lista encadeada para referencia de colunas*/
  Matriz* s;
  s = p;
  for (int j=0;j<C; j++){
    Matriz* novo = (Matriz*)malloc(sizeof(Matriz));
    s->right = novo;
    novo->column =-1;
    novo->below = novo;
    s=novo;
  }
  s->right = p;
  return p;
}
/******************************************************************/
/* FUNCAO PARA CONTAR NUMERO DE LINHAS E COLUNAS EM UMA MATRIZ*/

void get_nLineCols(Matriz* p, int &nlines, int &ncols){
  Matriz* m;
  Matriz* n;
  if (p == p->below){
    nlines=0;
    printf("Não exite a matriz");
  }
  nlines=0;
  for (m = p->below; m!=p; m= m->below){
    nlines++;
  }
  if (p==p->right){
    ncols =0;
    printf("Não exite a matriz");
  }
  ncols =0;
  for (n= p->right; n!=p; n= n->right){
    ncols++;
  }
}
/******************************************************************/
/* Funcao para imprimir uma matriz*/

void matriz_print (Matriz* p){
  Matriz* m;
  Matriz* n;
  for (m= p->below; m!=p; m =m->below){
    for (n=m->right; n!=m; n = n->right){
      printf("\n%d, %d, %f \n", n->line, n->column, n->info);
    }
  }
}
/******************************************************************/
/* Funcao para inserir elemento */

void setElem(Matriz* p, int x, int y, float elem){
  /*faz o ponteiro LH alcancar linha certa */
  Matriz* lineH;
  Matriz* colsH;
  Matriz* novo;
  novo = NULL;
  int l = 1;
  int c = 1; /* contadores para linha e coluna respectivamente*/
  lineH = p->below;
  colsH = p->right;

  /*faz o ponteiro lineH alcancar a linha correta*/
  while (l!= x){
    if (lineH == p){
      printf("o valor da linha nao existe para a matriz\n");
      exit(1);
    }
    l++;
    lineH = lineH->below;
  }
  /*faz o ponteiro colsH alcancar coluna correta*/
  while (c!=y){
    if (colsH == p){
      printf("o valor da coluna nao existe para a matriz\n");
      exit(1);
    }
    c++;
    colsH = colsH->right;
  }

  /* acertar linha*/
  Matriz* test;
  test = lineH->right;
  if (test ==lineH){ /* elemento novo na linha, insere no inicio */
    novo= (Matriz*)malloc(sizeof(Matriz));
    novo->line = x;
    novo->column = y;
    novo->info = elem;

    lineH->right =novo;
    novo->right = lineH;
  }
  else if (y< (test->column)){ /*inserir no meio */
    novo= (Matriz*)malloc(sizeof(Matriz));
    novo->line = x;
    novo->column = y;
    novo->info = elem;
    novo->right = test;
    lineH->right = novo;
  }
  else if(y>(test->column)){
    while (test->right != lineH){
      if (y<(test->right->column))
      break;

      else
      test = test->right;
    }
    novo= (Matriz*)malloc(sizeof(Matriz));
    novo->line = x;
    novo->column = y;
    novo->info = elem;
    novo->right = test->right;
    test->right = novo;
  }


  /* acertar coluna*/
  Matriz* testC;
  testC= colsH->below;
  if (testC->below == colsH){ /*nao existe elemento na coluna mas ja existe o no!!*/
    colsH->below = novo;
    novo->below = colsH;
  }
  else if (x< (testC->line)){
    novo->below = testC;
    colsH->below = novo;
  }
  else if (x>(testC->line)){
    while (testC->below != colsH && x<(testC->below->line)){
      testC = testC->below;
    }
    novo->right = test->right;
    test->right = novo;
  }

}
/******************************************************************/
/* FUNCAO PARA BUSCAR UM ELEMENTO EM UMA MATRIZ ESPARSA*/

float getElem(Matriz* m, int x, int y){
  Matriz* p = m;
  Matriz* lineH;
  Matriz* colsH;
  int i, j;
  float v;
  i=j=1;
  lineH = p->below;
  while(i!=x){
    if (lineH==p){
      printf("Nao exite esta linha!");
      exit (1);
    }
    lineH = lineH->below;
    i++;
  }
  Matriz* test;
  test = lineH->right;
  /*nao existe elemento, pois a linha esta vazia*/
  if(test==lineH)
  return 0;
  else if (y<test->column)
  return 0;

  while(y != test->column){
    test=test->right;
    if((test==lineH)) return 0.0;
  }

  return test->info;
}
/******************************************************************/
/* FUNCAO PARA DESTRUIR UMA MATRIZ*/

void matriz_destroy(Matriz* m) {
  if(m== NULL) /*verifica se a matriz ja esta vazia*/
  return;
  Matriz *p, *t;
  p = m->below;
  destroy_line(m);
  while(p != m){
    t = p->below;
    destroy_line(p);
    p = t;
  }
}
/*****************************************************************/
/* FUNCAO PARA DESTRUIR UMA LINHA INTEIRA, AUXILIAR DA DESTROY*/

static void destroy_line(Matriz* x){
  Matriz *ant, *prox;
  ant = x->right;
  free(x);
  while(ant != x){
    prox = ant->right;
    free(ant);
    ant = prox;
  }
}
/******************************************************************/
/* FUNCAO PARA SOMAR DUAS MATRIZES*/

Matriz* matriz_add(Matriz* m, Matriz* n){
  float soma= 0;
  int i, j;

  int nlineA= 0;
  int nlineB= 0;
  int ncolsA= 0;
  int ncolsB= 0;
  get_nLineCols(m,nlineA,ncolsA);
  get_nLineCols(n,nlineB,ncolsB);

  if(nlineA != nlineB || ncolsA != ncolsB){
    printf("As matrizes devem ter numeros de linhas e colunas iguais!");
    exit(1);
  }

  Matriz* C;
  C= create_empty(nlineA, ncolsA);
  printf("Soma de duas matrizes esparsas");
  for(i=1; i<=nlineA; i++){
    for(j=1; j<=ncolsA; j++){
      soma = getElem(m,i,j) + getElem(n,i,j);
      if (soma!= 0)
      setElem(C,i,j,soma);
      soma= 0;
    }
  }

  return C;
}
/******************************************************************/
/* FUNCAO PARA MULTIPLICAR DUAS MATRIZES*/

Matriz* matriz_multiply(Matriz* m, Matriz* n){
  Matriz* D;

  int nlineA= 0;
  int nlineB= 0;
  int ncolsA= 0;
  int ncolsB= 0;
  int i, j, k;
  float prod;

  /*passagem por referencia, retorna os valores de linha e coluna das matrizes*/
  get_nLineCols(m,nlineA,ncolsA);
  get_nLineCols(n,nlineB,ncolsB);
  /*verifica se e permitido a multiplicacao*/
  if (ncolsA != nlineB){
    printf("ERRO!O # coluna da 1º matriz diferente do # linhas da 2º matriz");
    exit (1);
  }
  D= create_empty(nlineA, ncolsB);
  printf("Multiplicacao de duas matrizes esparsas");
  /*MULTIPLICAÇÃO E ALOCAÇÃO DOS ELEMENTOS*/
  float nA, nB; /*recebe os valores da matriz A e B respectivamente*/
  for(i=1; i<=nlineA;i++){
    for(j=1;j<=ncolsB; j++){
      prod= 0.0;
      for(k=1; k<=nlineA;k++){
        nA= getElem(m,i,k);
        nB= getElem(n,k,j);
        prod= prod+ (nA*nB);

      }
      if(prod!=0.0)
      setElem(D,i,j,prod);

    }
  }

  return D;
}
/******************************************************************/
