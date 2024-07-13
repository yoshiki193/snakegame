#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

struct Snake {
  int x;
  int y;
  int component;
  struct Snake *next;
};

void run();
void makeSnake(struct Snake **s, int y, int x);
void initSnake(struct Snake **s);
int showSnake(struct Snake *s, int ch, int lch);
void moveSnake(struct Snake *s, int d);

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
  new->component = '#';
  new->next = NULL;
}

void initSnake(struct Snake **s) {
  struct Snake *ptr;
  makeSnake(s,0,5);
  makeSnake(s,0,4);
  makeSnake(s,0,3);
  makeSnake(s,0,2);
  makeSnake(s,0,1);
  makeSnake(s,0,0);
  ptr = *s;
  while(ptr != NULL) {
    mvaddch(ptr->y,ptr->x,ptr->component);
    ptr = ptr->next;
  }
}

int showSnake(struct Snake *s, int ch, int lch) {
  if((ch != KEY_LEFT) && (ch != KEY_RIGHT) && (ch != KEY_DOWN) && (ch != KEY_UP)) {
    switch(lch) {
      case KEY_LEFT:
        if(lch != KEY_RIGHT) {
          moveSnake(s,1);
        }
        break;
      case KEY_RIGHT:
        if(lch != KEY_LEFT) {
          moveSnake(s,2);
        }
        break;
      case KEY_DOWN:
        if(lch != KEY_UP) {
          moveSnake(s,3);
        }
        break;
      case KEY_UP:
        if(lch != KEY_DOWN) {
          moveSnake(s,4);
        }
      default:
        break;
    }
  } else {
    switch(ch) {
      case KEY_LEFT:
        if(lch != KEY_RIGHT) {
          moveSnake(s,1);
          lch = ch;
        }
        break;
      case KEY_RIGHT:
        if(lch != KEY_LEFT) {
          moveSnake(s,2);
          lch = ch;
        }
        break;
      case KEY_DOWN:
        if(lch != KEY_UP) {
          moveSnake(s,3);
          lch = ch;
        }
        break;
      case KEY_UP:
        if(lch != KEY_DOWN) {
          moveSnake(s,4);
          lch = ch;
        }
      default:
        break;
    }
  }
  return lch;
}

void moveSnake(struct Snake *s, int d) {
  int lx = 0;
  int ly = 0;
  int tmpx = 0;
  int tmpy = 0;
  lx = s->x;
  ly = s->y;
  switch(d) {
    case 1:
      s->x--;
      break;
    case 2:
      s->x++;
      break;
    case 3:
      s->y++;
      break;
    case 4:
      s->y--;
      break;
  }
  mvaddch(s->y,s->x,s->component);
  while(s->next != NULL) {
    s = s->next;
    tmpx = s->x;
    tmpy = s->y;
    s->x = lx;
    s->y = ly;
    lx = tmpx;
    ly = tmpy;
  }
  mvaddch(ly,lx,' ');
}

void run() {
  int ch = 0,lch,loop = 0;
  struct Snake *s = NULL;
  timeout(50);
  initSnake(&s);
  while((ch = getch()) != 'q') {
    loop++;
    lch = showSnake(s, ch, lch);
  }
}