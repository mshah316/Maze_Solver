//#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
int debugmode=FALSE;

typedef struct node {
    char c;
    int x;
    int y;
    struct node *next;

} NODE;

typedef struct list_struct{
    NODE *front;

}STACK;

STACK *STACK_create() {
STACK *s = malloc(sizeof(STACK));

  s->front = NULL;
  return s;
}

void STACK_free(STACK *l) {
NODE *p = l->front;
NODE *pnext;

  while(p != NULL) {
    pnext = p->next;   // keeps us from de-referencing a freed ptr
    free(p);
    p = pnext;
  }
  // now free the STACK
  free(l);
}

void stk_print(STACK *l) {
NODE *p = l->front;

  printf("[");
  while(p != NULL) {
    printf("%c", p->c);
    p = p->next;
  }
  printf("]\n");
}

static void p_rev(NODE *p);
void stk_print_rev(STACK *l) {
  printf("r=row c=column[");

  p_rev(l->front);

  printf("]\n");
}

static void p_rev(NODE *p) {

  if(p==NULL)
	return;
  p_rev(p->next);
  printf(" (r:%d c:%d),", p->x,p->y);
}

void push_STACK(STACK * s, char c, int x, int y) {
NODE *p = malloc(sizeof(NODE));
    if(s->front!=NULL){
      p->c = c;
      p->x = x;
      p->y = y;
      p->next = s->front;
      s->front=p;
    }
    else{
        p->c = c;
        p->x = x;
        p->y = y;
        p->next = NULL;
        s->front=p;
    }
}

void pop_stk(STACK * s){
    NODE* temp=s->front;
    s->front=s->front->next;
    free(temp);
}

typedef struct mazeStruct
{
    char **arr;
	int xsize, ysize;
	int xstart, ystart;
	int xend, yend;


} maze;

void allocarr(maze  *m){
    int r = m->xsize+2, c = m->ysize+2, i;

    char **arr = (char **)malloc(r * sizeof(char *));
    for (i=0; i<r; i++)
         arr[i] = (char *)malloc((c) * sizeof(char));
   (m->arr)=arr;
}

void freearr(maze  *m){
    int r = m->xsize+2, c = m->ysize+2, i;

    for (i=0; i<r; i++){free(m->arr[i]);}
   free(m->arr);
}


maze *maze_create() {
maze *m = malloc(sizeof(maze));
  m->xend=0;
  m->xsize=0;
  m->xstart=0;
  m->yend=0;
  m->ysize=0;
  m->ystart=0;
  return m;
}

void checkneighbor(STACK*s,maze *m){

        if(m->arr[s->front->x][s->front->y+1]!='*'&&m->arr[s->front->x][s->front->y+1]!='V'){push_STACK(s,'V',(s->front->x),(s->front->y+1));}
        else if(m->arr[s->front->x][s->front->y-1]!='*'&&m->arr[s->front->x][s->front->y-1]!='V'){push_STACK(s,'V',s->front->x,s->front->y-1);}
        else if(m->arr[s->front->x+1][s->front->y]!='*'&&m->arr[s->front->x+1][s->front->y]!='V'){push_STACK(s,'V',s->front->x+1,s->front->y);}
        else if(m->arr[s->front->x-1][s->front->y]!='*'&&m->arr[s->front->x-1][s->front->y]!='V'){push_STACK(s,'V',s->front->x-1,s->front->y);}
        else{if(debugmode == TRUE ){printf (" Debugging Information \n");
        printf("pop on stack row x %d column y %d\n",s->front->x,s->front->y);
        }pop_stk(s);m->arr[s->front->x][s->front->y]='V';
    }
}
int main(int argc, char **argv)
{

	maze *m=maze_create();
	/*
    m->xend=3;
    m->xsize=10;
    m->xstart=1;
    m->yend=5;
    m->ysize=10;
    m->ystart=1;
    */
	STACK *s=STACK_create();

	int xpos, ypos;
	int i, j,counter=0;
	FILE *src;

	/* verify the proper number of command line arguments were given

	if (argc < 2) {
		printf("Usage: %s <input file name> only can take one file input\n", argv[0]);
		exit(-1);
	}*/

    printf("will only execute on text file which was entered last\n");
    for (i = 0; i < argc; i++){
            if(strcmp(argv[i],"-d")==0)
                { debugmode= TRUE;}
            else{
                        src = fopen(argv[i], "r");
            }
            //if(strcmp(argv[2],"-d")==0){ debugmode= TRUE;}
            }


	/* Try to open the input file.*
	if ((src = fopen(argv[1], "r")) == NULL)
	{
		printf("Can't open input file: %s", argv[1]);
		exit(-1);
	}

	/* read in the size, starting and ending positions in the maze*/

	while(counter!=3){
            int x,y=0;
        fscanf(src, "%d %d", &x, &y);
        //printf("x:%d y:%d counter:%d\n",x,y,counter);

        if(x<1&&y<1 && counter==0){printf("Array bounds Invalid\n");}
        if(counter==2 && x<=m->xsize&&y<=m->ysize){m->xend=x; m->yend=y; counter++;}
        if(counter==1 && x<=m->xsize&&y<=m->ysize){m->xstart=x; m->ystart=y; counter++;}
        if(counter==0 && ((x>0&&y>0))){m->xsize=x; m->ysize=y; counter++;}

	}


	/* print them out to verify the input */
	printf("size: %d, %d\n", m->xsize, m->ysize);
	printf("start: %d, %d\n", m->xstart, m->ystart);
	printf("end: %d, %d\n", m->xend, m->yend);
    allocarr(m);
	/* initialize the maze to empty */
	for (i = 0; i < m->xsize + 2; i++)
		for (j = 0; j < m->ysize + 2; j++)
			m->arr[i][j] = '.';


	/* mark the starting and ending positions in the maze */
	m->arr[m->xstart][m->ystart] = 's';
	m->arr[m->xend][m->yend] = 'e';

	/* mark the blocked positions in the maze with *'s*/
	while (fscanf(src, "%d %d", &xpos, &ypos) != EOF)
	{   //printf("x:%d,y:%d\n",xpos,ypos);
        if(xpos>1 && ypos>1 && xpos<=m->xsize && ypos<=m->ysize && xpos!=m->xstart && ypos!=m->ystart){
            m->arr[xpos][ypos] = '*';
        }
	}
	/* making neighbors of start and end unblocked*/
    m->arr[m->xstart+1][m->ystart] = '.';
    m->arr[m->xstart-1][m->ystart] = '.';
    m->arr[m->xstart][m->ystart+1] = '.';
    m->arr[m->xstart][m->ystart-1] = '.';
	m->arr[m->xend+1][m->yend] = '.';
    m->arr[m->xend-1][m->yend] = '.';
    m->arr[m->xend][m->yend+1] = '.';
    m->arr[m->xend][m->yend-1] = '.';
    /* mark the borders of the maze with *'s */
	for (i = 0; i < m->xsize + 2; i++)
	{
		m->arr[i][0] = '*';
		m->arr[i][m->ysize + 1] = '*';
	}
	for (i = 0; i < m->ysize + 2; i++)
	{
		m->arr[0][i] = '*';
		m->arr[m->xsize + 1][i] = '*';
	}


	m->arr[m->xstart][m->ystart] = 'V';
	m->arr[m->xend][m->yend] = 'e';

	/* print out the initial maze */
	for (i = 0; i < m->xsize + 2; i++)
	{
		for (j = 0; j < m->ysize + 2; j++)
			printf("%c", m->arr[i][j]);
		printf("\n");
	}
	push_STACK(s,'V',m->xstart,m->ystart);
    /*push's quordinates on to stack if stack is not empty or end has been found*/
    while(1){
        checkneighbor(s,m);
        if(s->front->x==m->xend && s->front->y==m->yend){break;}
        if(s->front->x==m->xstart && s->front->y==m->ystart){break;}
        {m->arr[s->front->x][s->front->y]='V';}
    if(debugmode == TRUE ){printf (" Debugging Information \n");
        printf("push on stack row x %d column y %d\n",s->front->x,s->front->y);
    }
    }
    /*returns result after loop has done executing*/
    if(s->front->x==m->xend && s->front->y==m->yend){printf("solution:");stk_print_rev(s);}
    else if(s->front->x==m->xstart && s->front->y==m->ystart){printf("No solution to maze\n");}
    STACK_free(s);
    freearr(m);

}
