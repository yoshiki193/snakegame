#include "func.h"
#include <ncurses.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  char *fn;
  FILE *fp;

  if(argc != 2) {
    fprintf(stderr, "スコアファイルを指定してください\n");
    exit(1);
  }

  argv++;

  if((fp = fopen(*argv, "r")) == NULL) {
    fprintf(stderr, "スコアファイルがありません\n");
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