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

int run(int sy, int sx);
void makeFeed(struct Feed **f, int sy, int sx);
int eraseFeed(struct Feed **f, struct Snake *s);
void showFeed(struct Feed *f, struct Snake *s);
void makeSnake(struct Snake **s, int y, int x);
int moveSnake(struct Snake *s, int ch, int lch, int sy, int sx);
void addSnake(struct Snake **s, int lch);
void initSnake(struct Snake **s);
int collisionDetection(struct Snake *s);
void printWall(int sy, int sx);
void showTitle(char *fn);
void showGameover(char *fn, int score, int sy, int sx);