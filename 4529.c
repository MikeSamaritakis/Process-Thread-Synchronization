/* Michail Samaritakis csd4529*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>  
#include <pthread.h>

#define EAST 0
#define WEST 1

#define RED 0
#define BLUE 1

#define SIDEWALK 0
#define STREET 1

pthread_mutex_t mutex;

struct pedestrian {   /* Pedestrian's attributes (single struct size = 16bytes)*/
  int direction;      /* 0 = heading right/east, 1 = heading left/west */
  int  id;            /* id for each pedestrian, varies from 0-9 */ 
  int color;          /* 0 = red clothes , 1 = blue clothes */
  /* int position; */ /* 0 = sidewalk, 1 = street */
}; 

int arraysize;
struct pedestrian *str;
struct pedestrian *sdl;


void red () {
printf("\033[1;31m");
}

void blue () {
printf("\033[0;34m");
}

void reset () {
printf("\033[0m");
}

void color(int c){
if (c)blue();
else red();
}

void print_streetside(){ /* Prints the 'street'/'sidewalk', to visualize the thread movement in the terminal. */
  printf("Street:   ");

  for (size_t i = 0; i < arraysize; i++)
  {
    if (str[i].color == RED){
      color(RED);
      if (str[i].direction == EAST){
        printf("%d >", str[i].id);
      }
      else if (str[i].direction == WEST){
        printf("%d <", str[i].id);
      }
      reset();
    }
    else if (str[i].color == BLUE){
      color(BLUE);
      if (str[i].direction == EAST){
        printf("%d >", str[i].id);
      }
      else if (str[i].direction == WEST){
        printf("%d <", str[i].id);
      }
      reset();
    }
    printf("||   ");
  }
  printf("\n");

  return;
}

void print_sidewalkside(){
  printf("Sidewalk: ");

  for (size_t i = 0; i < arraysize; i++)
  {
    if (sdl[i].color == RED){
      color(RED);
      if (sdl[i].direction == EAST){
        printf("%d >", sdl[i].id);
      }
      else if (sdl[i].direction == WEST){
        printf("%d <", sdl[i].id);
      }
      reset();
    }
    else if (sdl[i].color == BLUE){
      color(BLUE);
      if (sdl[i].direction == EAST){
        printf("%d >", sdl[i].id);
      }
      else if (sdl[i].direction == WEST){
        printf("%d <", sdl[i].id);
      }
      reset();
    }
    printf("||  ");
  }
  printf("\n");


}

int find_dom_color(){ /* Return the dominant color. */
  int red = 0, blue = 0;
  for (size_t i = 0; i < arraysize; i++)
  {
    if (str[i].color == RED){
      red++;
    }
    else if (str[i].color == BLUE){
      blue++;
    }
  }
  if (red >= blue) return RED;
  if (blue > red) return BLUE;
  return -1;
}

int find_dom_direction(){ /* Return the dominant direction. */
  int west = 0, east = 0;
  for (size_t i = 0; i < arraysize; i++)
  {
    if (str[i].direction == WEST){
      west++;
    }
    else if (str[i].direction == EAST){
      east++;
    }
  }
  if (west >= east) return WEST;
  if (east > west) return EAST;
  return -1;
}

int find_dom_direction_sidewalk(){ /* Return the dominant direction of the sidewalk. */
  int west = 0, east = 0;
  for (size_t i = 0; i < arraysize; i++)
  {
    if (sdl[i].direction == WEST){
      west++;
    }
    else if (sdl[i].direction == EAST){
      east++;
    }
  }
  if (west >= east) return WEST;
  if (east > west) return EAST;
  return -1;
}

void segragate_bluered(int domcolor){
  if (domcolor == RED)
  {
    for (size_t i = 0; i < arraysize; i++)
    {
      if (str[i].color == BLUE){
        if (sdl[i].id == -1){
          sdl[i].color = str[i].color;
          sdl[i].direction = str[i].direction;
          sdl[i].id = str[i].id;

          str[i].color = -1;
          str[i].direction = -1;
          str[i].id = -1;
        }
      }
    }
  }
  else if (domcolor == BLUE)
  {
    for (size_t i = 0; i < arraysize; i++)
    {
      if (str[i].color == RED){
        if (sdl[i].id == -1){
          sdl[i].color = str[i].color;
          sdl[i].direction = str[i].direction;
          sdl[i].id = str[i].id;

          str[i].color = -1;
          str[i].direction = -1;
          str[i].id = -1;

        }
      }
    }
  }
  return;
}

void segragate_direction(){
  int domdirection = find_dom_direction(str, arraysize);
  if (domdirection == EAST)
  {
    for (size_t i = 0; i < arraysize; i++)
    {
      if (str[i].direction == WEST){
        if (sdl[i].direction == -1){
          sdl[i].color = str[i].color;
          sdl[i].direction = str[i].direction;
          sdl[i].id = str[i].id;

          str[i].color = -1;
          str[i].direction = -1;
          str[i].id = -1;
        }
      }
    }
    
  }
  else if (domdirection == WEST)
  {
    for (size_t i = 0; i < arraysize; i++)
    {
      if (str[i].direction == EAST){
        if (sdl[i].direction == -1){
          sdl[i].color = str[i].color;
          sdl[i].direction = str[i].direction;
          sdl[i].id = str[i].id;

          str[i].color = -1;
          str[i].direction = -1;
          str[i].id = -1;
        }
      }
    }
  }
}

void segragate_all(int domcolor){
  segragate_bluered(domcolor);
  segragate_direction();
  return;
}

void print_all(){ /* Prints both street and sidewalk one under the other. */
  print_streetside(); 
  print_sidewalkside(); 
  printf("\n");
  return;
}

int empty(){ /* Checks if the street is empty, if it is empty() returns 1, else it returns -1. */
  int check = 0;
  size_t i = 0;
  while (check == 0)
  {
    if(str[i].id != -1) return -1;
    i++;
  }
  return 1;
}

void backtostreet_bycolor(int domcolor){ /* Brings the remainings of the dominant color from the sidewalk back to the street. */
  if(empty(str) == 1){
    for (size_t i = 0; i < arraysize; i++){  
      if (str[i].id == -1){
        for (size_t i = 0; i < arraysize; i++){
          if (str[i].id == -1){
            if (domcolor == sdl[i].color){
              str[i].id = sdl[i].id;
              str[i].direction = sdl[i].direction;
              str[i].color = sdl[i].color;

              sdl[i].id = -1;
              sdl[i].direction = -1;
              sdl[i].color = -1;
            }
          }
        }
      }
    }
  }
  return;
}

void backtostreet_bydirection(int domcolor, int domdirection){ /* Brings the remainings of the dominant side from the sidewalk back to the street. */
  if(empty(str) == 1){
    for (size_t i = 0; i < arraysize; i++){  
      if (str[i].id == -1){
        for (size_t i = 0; i < arraysize; i++)
        {
          if (str[i].id == -1){
            if (domdirection == sdl[i].direction && domcolor == sdl[i].color){
              str[i].id = sdl[i].id;
              str[i].direction = sdl[i].direction;
              str[i].color = sdl[i].color;

              sdl[i].id = -1;
              sdl[i].direction = -1;
              sdl[i].color = -1;
            }
          }
        }
      }
    }
  }
  return;
}

void* move(void *position) {
  int pos = *(int *)(position);
  if (str[pos].direction == EAST)
  {
    while(1)
    {
      pthread_mutex_lock(&mutex);
      if(pos == arraysize-1)
      {
        str[pos].id = -1;
        str[pos].color = -1;
        str[pos].direction = -1;
        print_all();
        pthread_mutex_unlock(&mutex);
        break;
      }
      else 
      {
        if (str[pos+1].id == -1)
        {
          str[pos+1].id = str[pos].id;
          str[pos+1].color = str[pos].color;
          str[pos+1].direction = str[pos].direction;
          str[pos].id = -1;
          str[pos].color = -1;
          str[pos].direction = -1;
          pos++;
          print_all();
        }
      }
      pthread_mutex_unlock(&mutex);
    }
  }  
  else if (str[pos].direction == WEST)
  {
    while(1)
    {
      pthread_mutex_lock(&mutex);
      if(pos == 0)
      {
        str[pos].id = -1;
        str[pos].color = -1;
        str[pos].direction = -1;
        print_all();
        pthread_mutex_unlock(&mutex);
        break;
      }
      else 
      {
        if (str[pos-1].id == -1)
        {
          str[pos-1] = str[pos];
          str[pos].id = -1;
          str[pos].color = -1;
          str[pos].direction = -1;
          pos--;
          print_all();
        }
      }
      if (pos < 0) break;
      pthread_mutex_unlock(&mutex);
    }
  }
  return NULL;
}

void threads(int count){
  int c = 0;
  pthread_t *thda;
  thda = (pthread_t)malloc(sizeof(pthread_t)*count); /* Allocating memory for array of threads size. */
  pthread_mutex_init(&mutex, NULL);
  for (int j = 0; j < arraysize; j++)
  {
    if (str[j].id != -1){
      int *arguement = (int*)malloc(sizeof(int));
      if (arguement == NULL) {
        perror("Memory allocation error.\n");
        exit(1);
      }
      *arguement = j;
      if (pthread_create(&thda[c] , NULL, &move, arguement) != 0) {
            perror("Failed to create thread");
            exit(1);
      }
      if (str[j].id != -1) c++;
    }
  }


  for (int k = 0; k < count; k++)
  {
    if (pthread_join(thda[k], NULL) != 0) {
           printf("Failed to join %d threads.\n", k);
        }
  }
  return;
}

int main(int argc, char * argv[] ){

  int num, begin = 0, end = atoi(argv[2])-1;
  time_t t;
  size_t i;
  arraysize = atoi(argv[2]);
    
  /* Arguement quantity check. */
  if (argc<2) {
		printf("Not enough parmeters\n");
		exit(1);
	}

  str = (struct pedestrian*)malloc(sizeof(struct pedestrian)*arraysize); /* Allocating memory for street size. */
  /* Malloc NULL check. */
  if (str == NULL)
  {
    perror("Memory Issue.\n");
    exit(1);
  }

  sdl = (struct pedestrian*)malloc(sizeof(struct pedestrian)*arraysize); /* Allocating memory for sidewalk size. */
  /* Malloc NULL check. */
  if (sdl == NULL)
  {
    perror("Memory Issue.\n");
    exit(1);
  }

  srand((unsigned)time(&t)); /* Seed for random number generator. */

  /* Initializing pedestrians and their attributes on the street, as well as threads with corelation to it's id. */
  for (i = 0; i < arraysize; i++)
  {
    num = rand() % 2;

    if (num == 0){
      str[begin].direction = EAST;

      str[begin].color = rand() % 2;
      if (str[begin].color == 0){
        str[begin].color = RED;
      }
      else if (str[begin].color == 1){
        str[begin].color = BLUE;
      }

      str[begin].id = i;
      begin++;
    }
    else if (num == 1){
      str[end].direction = WEST;

      str[end].color = rand() % 2;
      if (str[end].color == 0){
        str[end].color = RED;
      }
      else if (str[end].color == 1){
        str[end].color = BLUE;
      }

      str[end].id = i;
      end--;
    }
  }
  
  /* Where the value is equal to -1 there is no value(=NULL). */
  for (i = 0; i < arraysize; i++) /* Initilization of sidewalk */
  {
    sdl[i].id = -1;
    sdl[i].color = -1;
    sdl[i].direction = -1;
  }

  int domcolor = find_dom_color(); /* Sets the dominant color. */

  print_all();
  segragate_bluered(domcolor);
  print_all();
  segragate_direction();
  print_all();
  
  int count = 0; /* Threads needed for the remaining unmoved pedestrians. */
  for (int i = 0; i < arraysize; i++)
  {
    if (str[i].id != -1) count++;
  }

  threads(count);

  for (size_t i = 0; i < arraysize; i++)
  {
    if (domcolor == sdl[i].color && sdl[i].id != -1 && str[i].id == -1)
    {
      str[i] = sdl[i];
      sdl[i].id = -1;
      sdl[i].direction = -1;
      sdl[i].color = -1;
    }
    
  }

  print_all();
  
  count = 0;
  for (int i = 0; i < arraysize; i++)
  {
    if (str[i].id != -1) count++;
  }
  
  threads(count);

  print_all();

  segragate_direction();

  threads(count);

  int domdirection = find_dom_direction_sidewalk();

  for (size_t i = 0; i < arraysize; i++)
  {
    if (sdl[i].direction == domdirection)
    {
      str[i] = sdl[i];
      sdl[i].id = -1;
      sdl[i].direction = -1;
      sdl[i].color = -1;
    }
  }

  print_all();

  threads(count);

    for (size_t i = 0; i < arraysize; i++)
  {
      str[i] = sdl[i];
      sdl[i].id = -1;
      sdl[i].direction = -1;
      sdl[i].color = -1;
  }

  print_all();

  threads(count);

  pthread_mutex_destroy(&mutex); /* Destroy created mutex. */
  free(str); /* Frees allocated memory for str array. */
  free(sdl); /* Frees allocated memory for sdl array. */
  return 0;
}