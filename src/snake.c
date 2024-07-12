#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

struct Snake {
  int x;
  int y;
  int component;
  int df;
  int uf;
  int length;
  struct Snake *next;
};

void run();
void makeSnake(struct Snake **s, int y, int x, int l);
void initSnake(struct Snake **s);
int showSnake(struct Snake *s, int ch, int lch);
void moveSnake(struct Snake *s, int  d);

int main(int argc, char **argv) {
  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr,TRUE);
  run();
  endwin();
  return 0;
}

void makeSnake(struct Snake **s, int y, int x, int l) {
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
  new->df = 0;
  new->uf = 0;
  new->length = l;
  new->component = '#';
  new->next = NULL;
}

void initSnake(struct Snake **s) {
  makeSnake(s,0,0,4);
  makeSnake(s,0,-1,4);
  makeSnake(s,0,-2,4);
  makeSnake(s,0,-3,4);
  makeSnake(s,0,-4,4);
}

int showSnake(struct Snake *s, int ch, int lch) {
  struct Snake *ptr;
  ptr = s;

  if((ch != KEY_LEFT) && (ch != KEY_RIGHT) && (ch != KEY_DOWN) && (ch != KEY_UP)) {
    switch(lch) {
      case KEY_LEFT:
        break;
      case KEY_RIGHT:
        break;
      default:
        break;
    }
  } else {
    switch(ch) {
      case KEY_LEFT:
        if(lch != KEY_RIGHT) {
          moveSnake(ptr,1);
          lch = ch;
        }
        break;
      case KEY_RIGHT:
        if(lch != KEY_LEFT) {
          moveSnake(ptr,2);
          lch = ch;
        }
        break;
      case KEY_DOWN:
        if(lch != KEY_UP) {
          moveSnake(ptr,3);
          lch = ch;
        }
        break;
      default:
        break;
    }
  }

  return lch;
}

void moveSnake(struct Snake *s,int d) {
  int i = 0;
  int j = 0;
  int k = 0;
  switch(d) {
    case 1:
      k = s->df;
      while(s != NULL) {
        mvaddch(s->y,s->x, ' ');
        if(s->df == 0) {
          s->x--;
        } else if(j < k) {
          s->y++;
          s->df++;
          j++;
        } else {
          s->x++;
        }
        mvaddch(s->y,s->x,s->component);
        s = s->next;
        i++;
      }
      break;
    case 2:
      while(s != NULL) {
        mvaddch(s->y,s->x, ' ');
        s->x++;
        mvaddch(s->y,s->x,s->component);
        s = s->next;
      }
      break;
    case 3:
      i = s->df;
      j = 2;
      while(s != NULL) {
        mvaddch(s->y,s->x, ' ');
        if(i >= 0) {
          s->y++;
          if(s->df != s->length) s->df++;
        } else {
          s->x++;
        }
        mvaddch(s->y,s->x,s->component);
        i--;
        s = s->next;
      }
      break;
    default:
      break;
  }
}

void run() {
  int ch = 0,lch,loop = 0;
  struct Snake *s = NULL;
  timeout(-1);
  initSnake(&s);
  while((ch = getch()) != 'q') {
    loop++;
    lch = showSnake(s, ch, lch);
  }
}