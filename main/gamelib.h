#include "mylib.h"
#include "../images/duck_hunt_right_32x23.h"
#include "../images/duck_hunt_left_32x23.h"
#include "../images/boom_32x23.h"
#include "../images/bullet_9x21.h"

#define NO_MOVE 0
#define RIGHT 1
#define UP 2
#define LEFT 3
#define DOWN 4
#define BG_COLOR RGB(9, 22, 31)
#define DUCK_WIDTH 32
#define DUCK_HEIGHT 23

typedef struct Player {
	Rectangle* rect; /*player representation */
	int speed; /* controls how fast the player moves in each direction */
	int lastDirection; /* 1: right, 2:up, 3:left, 4:down 0:no movement  */
} Player;


typedef struct {
	int x;
	int y;
	Rectangle* vertical;
	Rectangle* horizontal;
	int x0; /* center x */
	int y0; /* center y*/
	int width;
	int length;
	int color;
	int speed;
} Crosshair;


Player* cPlayer(Player*, Rectangle*, int);

void updatePlayer(Player* player);

int isYCollision(Player*, RectMover*);

void reflectRectangle(Player*, RectMover*);

void moveRandom(RectMover*);

void drawStartingScreen(void);

void updateCrosshair(Crosshair*, u16);

ImgMover* createDuck(ImgMover*, Rectangle*);
