#include "gamelib.h"
#include "../text/textlib.h"
#include "../images/start_screen.h"
#include "../images/end_screen.h"

#define TEXT_START 5
#define TEXT_COLOR RGB(25, 25, 25)
#define DARKER_TEXT RGB(14, 14, 12)
#define SHOT_IMG_START 85
#define SHOT_IMG_Y 140
#define BUSH_DIST 150

int main(void)
{
    	while (1) // main game loop
	{
	    REG_DISPCNT = MODE3 | BG2_ENABLE;

	    u16 shotsLeft = 3;
	    u16 bgColor = BG_COLOR;
	    u16 grass = GRASS;
	    char* shotString = "Shots left:";
	    char* ducksShotString = "Ducks left:";
	    boolean started = FALSE;
	    boolean shouldCreate = FALSE;
	    boolean fired = FALSE;
	    int reloading = 3;
	    int ducksToShoot = 9;

	    //Code for the cross hairs
	    Crosshair ch;
	    ch.x = 75;
	    ch.y = 75;
	    ch.x0 = 76;
	    ch.y0 = 76;
	    ch.speed = 15;
	    ch.width = 1;
	    ch.length= 3;
	    ch.color = RED;
	    Rectangle h = { ch.x, ch.y0, ch.length, ch.width, ch.color };
	    Rectangle v = { ch.x0, ch.y, ch.width, ch.length, ch.color };
	    ch.horizontal = &h;
	    ch.vertical = &v;

	    ImgMover curDuck;
	    Rectangle backing;
	    createDuck(&curDuck, &backing);

	    Rectangle foliage[6];
	    foliage[0] = *cRectangle(&foliage[0], 170, 110, 40, 10, RGB(0, 25, 0));
	    foliage[1] = *cRectangle(&foliage[1], 160, 115, 10, 5, RGB(0, 23, 0));
	    foliage[2] = *cRectangle(&foliage[2], 201, 117, 20, 3, RGB(0, 24, 0));
	    foliage[3] = foliage[0];
	    foliage[3].x -= BUSH_DIST;
	    foliage[4] = foliage[1];
	    foliage[4].x -= BUSH_DIST;
	    foliage[5] = foliage[2];
	    foliage[5].x -= BUSH_DIST;

	    Rectangle lastFiring;
	    lastFiring.x = ch.x - (DUCK_WIDTH / 2);
	    lastFiring.y = ch.y - (DUCK_HEIGHT / 2);
	    lastFiring.width = DUCK_WIDTH;
	    lastFiring.height = DUCK_HEIGHT;

	    drawImage3(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, start_screen);

	    /* Draw starting screen with explanatory text */
	    drawString("Press A to start!", TEXT_START, 140, TEXT_COLOR);
	    while (!started)
	    {
		if (BUTTON_PRESSED(BUTTON_A)){
		    started = TRUE;
		    drawRect(0,0, SCREEN_WIDTH, VIEWPORT_HEIGHT, bgColor);
		    drawRect(0, VIEWPORT_HEIGHT, SCREEN_WIDTH, (SCREEN_HEIGHT - VIEWPORT_HEIGHT), grass);
		}
	    }

	    for (int i = 0; i < (sizeof(foliage) / sizeof(Rectangle)); i++)
	    {
		drawRectangle(&foliage[i], foliage[i].color);
	    }

	    while(shotsLeft && ducksToShoot)
	    {
		updateCrosshair(&ch, BG_COLOR);

		if (reloading)
		{
		    reloading--;
		}
		else if (shouldCreate)
		{
		    createDuck(&curDuck, &backing);
		    shouldCreate = FALSE;
		}

		if (fired)
		{
		    drawRectangle(&lastFiring, BG_COLOR);
		    fired = FALSE;
		}

		if (!reloading && BUTTON_PRESSED(BUTTON_A))
		{
		    shotsLeft--;
		    int start = SHOT_IMG_START + (shotsLeft * BULLET_9X21_WIDTH);
		    drawRect(start, SHOT_IMG_Y, BULLET_9X21_WIDTH, BULLET_9X21_HEIGHT, grass);

		    Rectangle r = {ch.vertical->x, ch.horizontal->y, 1, 1, RED};
		    if (intersects(&r, curDuck.rect))
		    {
			/* remove the shot image from the movers */
			drawRectangle(curDuck.rect, BG_COLOR);
			drawChar(ducksToShoot + '0', 200, 150, grass);
			ducksToShoot--;
			shotsLeft = 3;
			shouldCreate = TRUE;
		    }

		    lastFiring.x = ch.horizontal->x - (DUCK_WIDTH / 2);
		    lastFiring.y = ch.vertical->y - (DUCK_HEIGHT / 2);
		    drawImage3(lastFiring.x, lastFiring.y, DUCK_WIDTH, DUCK_HEIGHT, boom_32x23);
		    fired = TRUE;
		    reloading = 10;
		}

		waitForVBlank();

		if (!shouldCreate)
		{
		    shouldCreate = updateImgMover(&curDuck, BG_COLOR);
		}

		waitForVBlank();

		drawString(shotString, 5, 150, TEXT_COLOR);

		for(int i = 0; i < shotsLeft; i++)
		{
		    int start = SHOT_IMG_START + (i * BULLET_9X21_WIDTH);
		    drawImage3(start, SHOT_IMG_Y, BULLET_9X21_WIDTH, BULLET_9X21_HEIGHT, bullet_9x21);
		}

		drawString(ducksShotString, 120, 150, TEXT_COLOR);
		drawChar((ducksToShoot + '0'), 200, 150, TEXT_COLOR);

	    }

	    drawImage3(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, end_screen);

	    char* message;
	    if (!ducksToShoot)
	    {
		message = "You win!";
	    }
	    else
	    {
	        message = "Game over!";
	    }

	    drawString(message, 15, 60, DARKER_TEXT);
	    drawString("Press select", 15, 80, DARKER_TEXT);
	    drawString("to play again!", 15, 90, DARKER_TEXT);

	    while (!BUTTON_PRESSED(BUTTON_SELECT))
	    {
		// Sit idly until the select button is pressed to restart the game
	    }
	}
}
