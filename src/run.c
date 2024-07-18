#include "func.h"
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

void showTitle(char *fn) {
  int sy;
  int sx;
  int ch;
  int score;

  getmaxyx(stdscr,sy,sx);

  while((ch = getch()) != 'q') {
    if(sx > 110) {
      mvprintw(sy/2-2,(sx-110)/2," oooooooo8  oooo   oooo      o      oooo   oooo ooooooooooo   ooooooo8       o      oooo     oooo ooooooooooo ");
      mvprintw(sy/2-1,(sx-110)/2,"888          8888o  88      888      888  o88    888    88  o888    88      888      8888o   888   888    88  ");
      mvprintw(sy/2  ,(sx-110)/2," 888oooooo   88 888o88     8  88     888888      888ooo8    888    oooo    8  88     88 888o8 88   888ooo8    ");
      mvprintw(sy/2+1,(sx-110)/2,"        888  88   8888    8oooo88    888  88o    888    oo  888o    88    8oooo88    88  888  88   888    oo  ");
      mvprintw(sy/2+2,(sx-110)/2,"o88oooo888  o88o    88  o88o  o888o o888o o888o o888ooo8888  888ooo888  o88o  o888o o88o  8  o88o o888ooo8888 ");
    } else {
      mvprintw(sy/2,(sx-9)/2,"SNAKEGAME");
    }

    mvprintw(sy/2+5,(sx-9)/2,"[s]. PLAY");
    mvprintw(sy/2+6,(sx-9)/2,"[q]. EXIT");

    if(ch == 's') {
      clear();
      score = run(sy,sx);
      clear();
      showGameover(fn,score,sy,sx);
      clear();
    }
  }
}

void showGameover(char *fn, int score, int sy, int sx) {
  FILE *fp;
  char *r;

  if((fp = fopen(fn, "r")) == NULL) {
    fprintf(stderr, "ランキングファイルがありません\n");
    exit(1);
  }

  r = (char *)malloc(sizeof(char)*15);
  fgets(r,15,fp);

  if((fp = fopen(fn, "w")) == NULL) {
    fprintf(stderr, "ランキングファイルがありません\n");
    exit(1);
  }

  mvprintw(sy/2-1,(sx-9)/2,"GAME OVER");
  mvprintw(sy/2,(sx-20)/2,"--------------------");

  if(atoi(r) < score) {
    mvprintw(sy/2+1,(sx-(17 + (int)log10(score)))/2,"NEW RECORD SCORE:%d",score);
    sprintf(r,"%d",score);
    fputs(r,fp);
    fputc('\n',fp);
  } else {
    mvprintw(sy/2+1,(sx-(13 + (int)log10(score)))/2,"RESULT SCORE:%d",score);
    fputs(r,fp);
  }

  mvprintw(sy/2+3,(sx-9)/2,"[q]. EXIT");

  fclose(fp);

  while(getch() != 'q');
}

void printWall(int sy, int sx) {
  int i = 0;
  for(i = 0;i < sy; i++) {
    mvaddch(i,sx,' ');
    mvaddch(i,sx,'|');
  }
}

int run(int sy, int sx) {
  int i;
  int ch = 0;
  int lch = 0;
  int loop = 0;
  int t1 = 0;
  int t2 = 0;
  int flag = 0;
  int goflag = 0;
  int lv = 1;
  int score = 0;
  int blink = 0;
  int feedq = sx/10;
  struct Snake *s = NULL;
  struct Feed *f = NULL;

  sx -= 14;
  printWall(sy,sx);
  srand((int)time(NULL));
  initSnake(&s);

  while(((ch = getch()) != 'q')) {
    loop++;

    if(f == NULL) for(i = 0;i < feedq;i++) makeFeed(&f,sy,sx);
    showFeed(f, s);

    if((ch != KEY_RIGHT) && (ch != KEY_LEFT) && (ch != KEY_UP) && (ch != KEY_DOWN)) {
      lch = moveSnake(s,lch,lch,sy,sx);
    } else {
      lch = moveSnake(s,ch,lch,sy,sx);
    }

    goflag = collisionDetection(s);

    if(goflag == 1) break;

    flag = eraseFeed(&f,s);

    if(flag == 1) {
      t2 = loop;
      score = score + lv + 10;
      addSnake(&s);
    } else if((((loop - t2) % 100) == 0) && ((score - 10) > 0)){
      score = score - 10;
    }

    if(score >= ((pow(lv,2) * 10))) {
      lv++;
      t1 = loop;
    }

    mvprintw(3,sx + 1,"Lv.%d",lv);
    mvprintw(4,sx + 1,"SCORE.              ");
    mvprintw(4,sx + 1,"SCORE.%d",score);
    if((t1 != 0) && ((loop - t1) != 50)) {
      if(((loop - t1) % 5) == 0){
        blink++;
      }

      if(blink%2 == 0) {
        mvprintw(6,sx + 1,"LEVELUP");
      } else {
        mvprintw(6,sx + 1,"       ");
      }
    } else {
      mvprintw(6,sx + 1,"       ");
        t1 = 0;
        blink = 0;
    }
  }

  return score;
}