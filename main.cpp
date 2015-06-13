#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define NUM_PONTOS 5

typedef struct tAresta{
  int y_max;
  float x_in_ymin, mx;
  struct tAresta *prox;
} Aresta;

typedef struct tPontos{
  int x;
  int y;
} Pontos;

Pontos points[NUM_PONTOS];

void insereAresta(Aresta *lista, Aresta *aresta)
{
  Aresta *p, *q = lista;

  p = q->prox;
  while (p)
  {
    if (aresta->x_in_ymin < p->x_in_ymin)
      p = NULL;
    else
    {
      q = p;
      p = p->prox;
    }
  }
  aresta->prox = q->prox;
  q->prox = aresta;
}

int proxY(int i, int cont, Pontos *pts)
{
  int j;

  if ((i + 1) > (cont - 1))
    j = 0;
  else
    j = i + 1;
  while (pts[i].y == pts[j].y)
    if ((j + 1) > (cont - 1))
      j = 0;
    else
      j++;
  return pts[j].y;
}

void inicializaAresta(Pontos menor, Pontos maior, int yComp, Aresta *aresta, Aresta *arestas[])
{
  aresta->mx = (float) (maior.x - menor.x) / (maior.y - menor.y);
  aresta->x_in_ymin = menor.x;
  if (maior.y < yComp)
    aresta->y_max = maior.y  - 1;
  else
    aresta->y_max = maior.y;
  insereAresta(arestas[menor.y], aresta);
}

void montaET(int cont, Pontos *pts, Aresta *arestas[])
{
  Aresta *aresta;
  Pontos P1, P2;
  int i, yAnt = pts[cont - 2].y;

  P1.x = pts[cont - 1].x;
  P1.y = pts[cont - 1].y;
  for(i = 0; i < cont; i++)
  {
    P2 = pts[i];
    if (P1.y != P2.y)
    {
      aresta = (Aresta *)malloc(sizeof (Aresta));
      if (P1.y < P2.y)
        inicializaAresta(P1, P2, proxY(i, cont, pts), aresta, arestas);
      else
        inicializaAresta(P2, P1, yAnt, aresta, arestas);
    }
    yAnt = P1.y;
    P1 = P2;
  }
}

void montaAET(int varredura, Aresta *ativa, Aresta *arestas[])
{
  Aresta *p, *q;

  p = arestas[varredura]->prox;
  while(p)
  {
    q = p->prox;
    insereAresta(ativa, p);
    p = q;
  }
}

void preencheLinha(int varredura, Aresta *ativa)
{
  Aresta *p1, *p2;
  int i;

  p1 = ativa->prox;
  while(p1)
  {
    p2 = p1->prox;
    for (i = p1->x_in_ymin; i < p2->x_in_ymin; i++)
      glVertex2i((int)i, varredura);
    p1 = p2->prox;
  }
}

void deleta(Aresta *q)
{
  Aresta *p = q->prox;

  q->prox = p->prox;
  free(p);
}

void atualizaAET(int varredura, Aresta *ativa)
{
  Aresta *q = ativa, *p = ativa->prox;

  while(p)
  {
    if (varredura >= p->y_max)
    {
      p = p->prox;
      deleta(q);
    }
    else
    {
      p->x_in_ymin = p->x_in_ymin + p->mx;
      q = p;
      p = p->prox;
    }
  }
}

void ordenaAET(Aresta *ativa)
{
  Aresta *q, *p = ativa->prox;

  ativa->prox = NULL;
  while(p)
  {
    q = p->prox;
    insereAresta(ativa, p);
    p = q;
  }
}

void scanLine(int cont, Pontos *pts)
{
  Aresta *arestas[500], *ativa;
  int i, varredura;

  for (i = 0; i < 500; i++)
  {
    arestas[i] = (Aresta *)malloc(sizeof (Aresta));
    arestas[i]-> prox = NULL;
  }
  montaET(cont, pts, arestas);
  ativa = (Aresta *)malloc(sizeof (Aresta));
  ativa->prox = NULL;

  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POINTS);
  for(varredura = 0; varredura < 500; varredura++)
  {
    montaAET(varredura, ativa, arestas);
    if (ativa->prox)
    {
      preencheLinha(varredura, ativa);
      atualizaAET(varredura, ativa);
      ordenaAET(ativa);
    }
  }
  glEnd();
  glFlush();
}

void inicializaOpenGL()
{
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //Define o modo inicial de apresentação
  glutInitWindowPosition(0,0); // Define a posição inicial da tela a ser apresentada
  glutInitWindowSize(500, 500); // Define o tamanho da tela a ser apresentada
  glutCreateWindow("Scan Line");
}

void inicializaCores()
{
  glClearColor(1.0,1.0,1.0,0);// define a cor de fundo da janela (no caso, branco)
  glColor3f(1.0,0.0,0.0);// define cor do traço (no caso, vermelho)
  glPointSize(1.0);// define espessura do traço
  gluOrtho2D(0 , 500 , 0 , 500);
}

void definePontos()
{
  int i;
  points[0].x = 30; points[0].y = 40;
  points[1].x = 50; points[1].y = 60;
  points[2].x = 90; points[2].y = 50;
  points[3].x = 120; points[3].y = 80;
  points[4].x = 50; points[4].y = 110;
}

void chamaScanLine()
{
  scanLine(NUM_PONTOS, points);
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    definePontos();
    inicializaOpenGL();
    inicializaCores();
    glutDisplayFunc(chamaScanLine);
    glutMainLoop();

    return 0;
}
