#include "func.h"
#include <ncurses.h>
#include <stdlib.h>

void makeFeed(struct Feed **f, int sy, int sx) {
  struct Feed *ptr,*new;
  int x;
  int y;
  
  x = rand()%(sx);
  y = rand()%(sy);

  new = (struct Feed *)malloc(sizeof(struct Feed));

  if(*f == NULL) {
    *f = new;
  }else {
    ptr = *f;
    while(ptr->next != NULL) {
      ptr = ptr->next;
    }
    ptr->next = new;
  }

  new->y = y;
  new->x = x;
  new->component = '@';
  new->next = NULL;
}

int eraseFeed(struct Feed **f, struct Snake *s) {
  struct Feed *ptr,*pre;
  int flag = 0;

  ptr = *f;
  pre = NULL;

  while(ptr != NULL) {
    if((s->y == ptr->y) && (s->x == ptr->x)) {
      if(ptr->next == NULL && pre != NULL) {
        pre->next = NULL;
      } else if(pre == NULL){
        *f = ptr->next;
      } else {
        pre->next = ptr->next;
      }
      free(ptr);
      flag = 1;
      break;
    } else {
      pre = ptr;
      ptr = ptr->next;
    }
  }

  return flag;
}

void showFeed(struct Feed *f, struct Snake *s) {
  struct Snake *tmp;
  int flag = 0;

  while(f != NULL) {
    tmp = s;

    while(tmp != NULL) {
      if((tmp->y == f->y) && (tmp->x == f->x)) flag = 1;
      tmp = tmp->next;
    }

    if(flag == 0) mvaddch(f->y,f->x,f->component);

    flag = 0;
    f = f->next;
  }
}