#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#define NUM_PONTOS 4

typedef struct aresta{
    float y_max;
    float x_in_ymin;
    float mx;
    aresta *prox;
};

typedef struct et{
    float y_min;
    et *prox_y;
    aresta *prox_aresta;
};

struct pontos{
    float x;
    float y;
}pontos[NUM_PONTOS];

et *listaET = NULL;
aresta *listaAET = NULL;


float x1,x2,x3,x4,y1,y2,y3,y4;

void draw_pixel(int x,int y)
{
    glColor3f(0.0,1.0,1.0);
    glPointSize(1.0);
    glBegin(GL_POINTS);
    glVertex2i(x,y);
    glEnd();
}

void edgedetect(float x1,float y1,float x2,float y2,int *le,int *re)
{
    float temp,x,mx;
    int i;

    if(y1>y2)
    {
        temp=x1,x1=x2,x2=temp;
        temp=y1,y1=y2,y2=temp;
    }

    if(y1==y2)
        mx=x2-x1;
    else
        mx=(x2-x1)/(y2-y1);

    x=x1;

    for(i=int(y1);i<=(int)y2;i++)
    {
        if(x<(float)le[i]) le[i]=(int)x;
        if(x>(float)re[i]) re[i]=(int)x;
        x+=mx;
    }
}

void scanfill(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
    int le[500],re[500],i,j;

    for(i=0;i<500;i++)
        le[i]=500,re[i]=0;

    edgedetect(x1,y1,x2,y2,le,re);
    edgedetect(x2,y2,x3,y3,le,re);
    edgedetect(x3,y3,x4,y4,le,re);
    edgedetect(x4,y4,x1,y1,le,re);

    for(j=0;j<500;j++)
    {
        if(le[j]<=re[j])
            for(i=le[j];i<re[j];i++)
                draw_pixel(i,j);
    }
}


void display()
{
    x1=250.0;y1=200.0;x2=150.0;y2=300.0;x3=250.0;
    y3=400.0;x4=350.0;y4=300.0;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glVertex2f(x3,y3);
    glVertex2f(x4,y4);
    glEnd();

    scanfill(x1,y1,x2,y2,x3,y3,x4,y4);

    glFlush();
}


void init()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,499.0,0.0,499.0);
}

/*void ordenaPontos()
{
    int a, j, passo, troca = 1, trocas = 0;

	for (passo = 0; passo < tam-1 && troca; passo++)
	{
      troca = 0;
      for (j = 0; j < tam-passo-1; j++)
        if (pontos[j].y > pontos[j+1].y)
        {
            troca = 1;
            a = aux[j];
            aux[j] = aux[j+1];
            aux[j+1] = a;
            trocas++;
        }
	}
}*/

void definePontos()
{
  printf("Entre com os pontos do polígono:\n");
  for(int i = 0; i < NUM_PONTOS; i++)
  {
     printf("\nPonto %d: \n", i+1);
     printf("\nx%d:  ", i+1);
     scanf("%f", &pontos[i].x);
     printf("\ny%d:  ", i+1);
     scanf("%f", &pontos[i].y);
  }
}

void montaListaET(float x1, float y1, float x2, float y2)
{
    et *aux, *reg;
    aresta *p, *q;
    float y_max, x_in_ymin, mx, y_min;
    int achou = 0;

    if (y1 < y2)
    {
      y_min = y1;
      x_in_ymin = x1;
      y_max = y2;
    }
    else
    {
      y_min = y2;
      x_in_ymin = x2;
      y_max = y1;
    }
    mx = 1/((y2-y1)/(x2-x1));
    aux = listaET;
    for(aux; aux->prox_y != NULL; aux = aux->prox_y){
      if (aux)
      {
         if (aux->y_min == y_min)
         {
             q = aux->prox_aresta;
             for(q; q->prox != NULL; q = q->prox){
               if (q->prox == NULL)
               {
                   p = (aresta *)malloc(sizeof(aresta));
                   p->y_max = y_max;
                   p->x_in_ymin = x_in_ymin;
                   p->mx = mx;
                   p->prox = NULL;
                   q->prox = p;
               }
             }
         }
         else if(aux->prox_y == NULL)
         {
            reg = (et *)malloc(sizeof(et));
            reg->y_min = y_min;
            reg->prox_y = NULL;
            p = (aresta *)malloc(sizeof(aresta));
            p->y_max = y_max;
            p->x_in_ymin = x_in_ymin;
            p->mx = mx;
            p->prox = NULL;
            reg->prox_aresta = p;
         }
      }
      else
      {
            reg = (et *)malloc(sizeof(et));
            reg->y_min = y_min;
            reg->prox_y = NULL;
            p = (aresta *)malloc(sizeof(aresta));
            p->y_max = y_max;
            p->x_in_ymin = x_in_ymin;
            p->mx = mx;
            p->prox = NULL;
            reg->prox_aresta = p;
      }
    }
}

void montaAET()
{
    aresta *q, *novo;
    et *p;
    q = listaAET;
    p = listaET;
    novo = p->prox_aresta;
    for(p; p->prox_y != NULL; p = p->prox_y)
    {

    }
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    definePontos();
    for(int i = 0; i < NUM_PONTOS; i++){
        if (i == (NUM_PONTOS - 1))
          montaListaET(pontos[i].x, pontos[i].y, pontos[0].x, pontos[0].y)
        else
          montaListaET(pontos[i].x, pontos[i].y, pontos[i+1].x, pontos[i+1].y);
    }
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(500,500);

    glutCreateWindow("scanline");
    glutDisplayFunc(display);

    init();
    glutMainLoop();

    return 0;
}
