#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Snake {
  int x;
  int y;
  int nx;
  int ny;
  int component;
  struct Snake *next;
};

struct Feed {
  int x;
  int y;
  int component;
  struct Feed *next;
};

void run();
void makeFeed(struct Feed **f, int sy, int sx);
void showFeed(struct Feed *f);
void makeSnake(struct Snake **s, int y, int x);
void initSnake(struct Snake **s);
int moveSnake(struct Snake *s, int ch, int lch, int sy, int sx);
void addSnake(struct Snake **s, int lch);

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

void showFeed(struct Feed *f) {
  while(f != NULL) {
    mvaddch(f->y,f->x,f->component);
    f = f->next;
  }
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
  int i;

  for(i = 5;i >= 0;i--) makeSnake(s,0,i);

  ptr = *s;

  while(ptr != NULL) {
    mvaddch(ptr->y,ptr->x,ptr->component);
    ptr = ptr->next;
  }
}

int moveSnake(struct Snake *s, int ch, int lch, int sy, int sx) {
  int lx = s->x;
  int ly = s->y;
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

  if(s->x >= sx) s->x -= sx;
  if(s->x < 0) s->x += sx;
  if(s->y >= sy) s->y -= sy;
  if(s->y < 0) s->y += sy;

  mvaddch(s->y,s->x,s->component);

  while(s->next != NULL) {
    s = s->next;
    tmpx = s->x;
    tmpy = s->y;
    s->x = lx;
    s->y = ly;
    lx = tmpx;
    ly = tmpy;
    s->nx = lx;
    s->ny = ly;
  }

  mvaddch(ly,lx,' ');

  return lch;
}

void addSnake(struct Snake **s, int lch) {
  int x = 0;
  int y = 0;
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
  new->component = '#';
  new->next = NULL;
}

void run() {
  int i;
  int sx;
  int sy;
  int ch = 0;
  int lch = 0;
  int loop = 0;
  struct Snake *s = NULL;
  struct Feed *f = NULL;

  getmaxyx(stdscr,sy,sx);
  timeout(50);
  srand((int)time(NULL));
  initSnake(&s);
  for(i = 0;i < 3;i++) makeFeed(&f,sy,sx);
  showFeed(f);

  while(((ch = getch()) != 'q')) {
    loop++;
    if((ch != KEY_RIGHT) && (ch != KEY_LEFT) && (ch != KEY_UP) && (ch != KEY_DOWN)) {
      lch = moveSnake(s,lch,lch,sy,sx);
    } else {
      lch = moveSnake(s,ch,lch,sy,sx);
    }
    showFeed(f);
  }
}