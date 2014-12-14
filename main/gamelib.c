/*
 * gamelib.c
 *
 *  Created on: Oct 22, 2014
 *      Author: ngraves3
 */

#include "gamelib.h"
#include <stdlib.h>


void updateCrosshair(Crosshair* c, u16 bgColor)
{
	drawRectangle(c->horizontal, c->horizontal->color);
	drawRectangle(c->vertical, c->vertical->color);

	if (BUTTON_PRESSED(BUTTON_UP))
	{
		/* c is at the top of the screen and shouldnt move up */
		if ((c->vertical->y - c->speed) >= 0)
		{
			drawRectangle(c->vertical, bgColor);
			drawRectangle(c->horizontal, bgColor);
			c->vertical->y -= c->speed;
			c->horizontal->y -= c->speed;
			drawRectangle(c->vertical, c->vertical->color);
			drawRectangle(c->horizontal, c->horizontal->color);

		}
	}

	if (BUTTON_PRESSED(BUTTON_DOWN))
	{
		if ((c->vertical->y + c->vertical->height + c->speed) < VIEWPORT_HEIGHT)
		{
			drawRectangle(c->vertical, bgColor);
			drawRectangle(c->horizontal, bgColor);
			c->vertical->y += c->speed;
			c->horizontal->y += c->speed;
			drawRectangle(c->vertical, c->vertical->color);
			drawRectangle(c->horizontal, c->horizontal->color);

		}
	}

	if (BUTTON_PRESSED(BUTTON_LEFT))
	{
		if ((c->horizontal->x - c->speed) >= 0)
		{
			drawRectangle(c->horizontal, bgColor);
			drawRectangle(c->vertical, bgColor);
			c->horizontal->x -= c->speed;
			c->vertical->x -= c->speed;
			drawRectangle(c->horizontal, c->horizontal->color);
			drawRectangle(c->vertical, c->vertical->color);

		}
	}

	if (BUTTON_PRESSED(BUTTON_RIGHT))
	{
		if ((c->horizontal->x + c->horizontal->width + c->speed) < SCREEN_WIDTH)
		{	Rectangle h = *(c->horizontal);
			Rectangle v = *(c->vertical);
			c->horizontal->x += c->speed;
			c->vertical->x += c->speed;
			drawRectangle(&h, bgColor);
			drawRectangle(&v, bgColor);
			//c->horizontal->x += c->speed;
			//c->vertical->x += c->speed;
			drawRectangle(c->horizontal, c->horizontal->color);
			drawRectangle(c->vertical, c->vertical->color);

		}
	}
}

ImgMover* createDuck(ImgMover* retval, Rectangle* r)
{
	r->x = (rand() % 190) + 10;
	r->y = (rand() % 20) + 75;
	r->width = DUCK_WIDTH;
	r->height = DUCK_HEIGHT;
	r->color = BG_COLOR;
	retval->rect = r;
	retval->dx = (rand() % 7) - 3;
	retval->dy = (rand() % 5) - 4;

	if (!retval->dx && !retval->dy)
	{
		retval->dx = 1;
	}

	if (retval->dx >= 0)
	{
		retval->img = duck_hunt_right_32x23;
	}
	else
	{
		retval->img = duck_hunt_left_32x23;
	}

	retval->origWidth = retval->rect->width;
	retval->origHeight = retval->rect->height;

	return retval;
}


