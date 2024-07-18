#include "func.h"
#include <ncurses.h>
#include <stdlib.h>

void makeSnake(struct Snake **s, int y, int x) {
  struct Snake *ptr,*new;

  new = (struct Snake *)malloc(sizeof(struct Snake));

  if(*s == NULL) {
    *s = new;
  }else {
    ptr = *s;
    while(ptr->next != NULL) {
      ptr = ptr->next;
    }
    ptr->next = new;
  }

  new->y = y;
  new->x = x;
  new->component = 'o';
  new->next = NULL;
}

int moveSnake(struct Snake *s, int ch, int lch, int sy, int sx) {
  int ly = s->y;
  int lx = s->x;
  int tmpx = 0;
  int tmpy = 0;

  switch(ch) {
    case KEY_LEFT:
      if(lch == KEY_RIGHT) {
        s->x++;
        break;
      } else {
        lch = ch;
        s->x--;
        break;
      }
    case KEY_RIGHT:
      if(lch == KEY_LEFT) {
        s->x--;
        break;
      } else {
        lch = ch;
        s->x++;
        break;
      }
    case KEY_DOWN:
      if(lch == KEY_UP) {
        s->y--;
        break;
      } else {
        lch = ch;
        s->y++;
        break;
      }
    case KEY_UP:
      if(lch == KEY_DOWN) {
        s->y++;
        break;
      } else {
        lch = ch;
        s->y--;
        break;
      }
    default:
      return lch;
      break;
  }

  if(s->y >= sy) s->y -= sy;
  if(s->y < 0) s->y += sy;
  if(s->x >= sx) s->x -= sx;
  if(s->x < 0) s->x += sx;

  mvaddch(s->y,s->x,s->component);

  while(s->next != NULL) {
    s = s->next;
    tmpy = s->y;
    tmpx = s->x;
    s->y = ly;
    s->x = lx;
    ly = tmpy;
    lx = tmpx;
    s->ny = ly;
    s->nx = lx;
  }

  mvaddch(ly,lx,' ');

  return lch;
}

void addSnake(struct Snake **s, int lch) {
  struct Snake *ptr,*new;

  new = (struct Snake *)malloc(sizeof(struct Snake));

  if(*s == NULL) {
    *s = new;
  }else {
    ptr = *s;
    while(ptr->next != NULL) {
      ptr = ptr->next;
    }
    ptr->next = new;
  }

  new->y = ptr->ny;
  new->x = ptr->nx;
  new->component = 'o';
  new->next = NULL;
}

void initSnake(struct Snake **s) {
  struct Snake *ptr;
  int i;

  for(i = 5;i >= 0;i--) makeSnake(s,0,i);

  ptr = *s;

  while(ptr != NULL) {
    mvaddch(ptr->y,ptr->x,ptr->component);
    ptr = ptr->next;
  }
}

int collisionDetection(struct Snake *s) {
  int hy = s->y;
  int hx = s->x;
  int flag = 0;

  s = s->next;
  while(s != NULL) {
    if((s->y == hy) && (s->x == hx)) {
      flag = 1;
    }
    s = s->next;
  }

  return flag;
}