#include "func.h"
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char **argv) {
  char *fn;
  FILE *fp;

  if(argc != 2) {
    fprintf(stderr, "ランキングファイルを指定してください\n");
    exit(1);
  }

  argv++;

  if((fp = fopen(*argv, "r")) == NULL) {
    fprintf(stderr, "ランキングファイルがありません\n");
    exit(1);
  }

  fclose(fp);

  fn = *argv;

  initscr();
  noecho();
  cbreak();
  curs_set(0);
  keypad(stdscr,TRUE);
  timeout(50);
  showTitle(fn);
  flushinp();
  endwin();
  return 0;
}