#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
int eraseFeed(struct Feed **f, struct Snake *s);
void showFeed(struct Feed *f);
void makeSnake(struct Snake **s, int y, int x);
int moveSnake(struct Snake *s, int ch, int lch, int sy, int sx);
void addSnake(struct Snake **s, int lch);
void initSnake(struct Snake **s);
void printWall(int sy, int sx);
int collisionDetection(struct Snake *s);

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

void showFeed(struct Feed *f) {
  int i = 0;
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
  int y = 0;
  int x = 0;

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

void printWall(int sy, int sx) {
  int i = 0;
  for(i = 0;i < sy; i++) {
    mvaddch(i,sx,' ');
    mvaddch(i,sx,'|');
  }
}

void run() {
  int i;
  int sy;
  int sx;
  int ch = 0;
  int lch = 0;
  int loop = 0;
  int flag = 0;
  int goflag = 0;
  int lv = 1;
  int score = 0;
  struct Snake *s = NULL;
  struct Feed *f = NULL;

  getmaxyx(stdscr,sy,sx);
  sx -= 14;
  printWall(sy,sx);
  timeout(50);
  srand((int)time(NULL));
  initSnake(&s);
  for(i = 0;i < 5;i++) makeFeed(&f,sy,sx);

  while(((ch = getch()) != 'q')) {
    loop++;

    showFeed(f);

    if((ch != KEY_RIGHT) && (ch != KEY_LEFT) && (ch != KEY_UP) && (ch != KEY_DOWN)) {
      lch = moveSnake(s,lch,lch,sy,sx);
    } else {
      lch = moveSnake(s,ch,lch,sy,sx);
    }

    goflag = collisionDetection(s);
    if(goflag == 1) break;

    flag = eraseFeed(&f,s);

    if(flag == 1) {
      score = score + lv + 10;
      addSnake(&s, lch);
    }

    if(score >= ((round(pow(lv,2)) * 10))) {
      lv++;
    }

    mvprintw(3,sx + 1,"Lv.%d",lv);
    mvprintw(4,sx + 1,"SCORE.%d",score);

    if(f == NULL) for(i = 0;i < 5;i++) makeFeed(&f,sy,sx);
  }
}