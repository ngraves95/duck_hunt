#include "mylib.h"

/* Function definitions */

u16 *writeBuffer = (u16 *) 0x6000000;

void setPixel(int x, int y, u16 color)
{
	writeBuffer[OFFSET(x, y)] = color;
}

/*
 * Draws a solid colored rectangle
 */
void drawRect(int x, int y, int width, int height, volatile u16 color)
{
    for (int j = y; j < (height + y); j++)
    {
      DMA[3].src = &color;
      DMA[3].dst = &writeBuffer[OFFSET(x, j)];
      DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
    }

}

/*
 * Draws an image using DMA
 */
void drawImage3(int x, int y, int width, int height, const u16* img)
{
	for (int i = 0; i < height; i++)
	{
		DMA[3].src = img + (i * width);
		DMA[3].dst = &writeBuffer[OFFSET(x, y + i)];
		DMA[3].cnt = width | DMA_ON;
	}
}

void drawPartialImage(int x, int y, int width, int height, const u16* img,
		int partialWidth, int partialHeight, int increasing)
{
	if (partialHeight == height && partialWidth == width)
	{
		drawImage3(x, y, width, height, img);
	}
	else if (increasing)
	{
		for (int i = (height - partialHeight); i < height; i++)
		{
			DMA[3].src = img + (width - partialWidth) + width * i;
			DMA[3].dst = &writeBuffer[OFFSET(x, y + i - height + partialHeight)];
			DMA[3].cnt = partialWidth | DMA_ON;
		}
	}
	else
	{
		for (int i = (height - partialHeight); i < height; i++)
		{
			DMA[3].src = img + width * (i - height + partialHeight);
			DMA[3].dst = &writeBuffer[OFFSET(x, y + i - height + partialHeight)];
			DMA[3].cnt = partialWidth | DMA_ON;
		}
	}
}


void drawRectangle(Rectangle* r, u16 color)
{
	drawRect(r->x, r->y, r->width, r->height, color);
}


void plotLine(int x0, int y0, int x1, int y1, u16 color)
{
	int steepSlope = 0;

	/* if dy > dx, swap x_i and y_i to get dx > dy */
	if (ABS(y1 - y0) > ABS(x1 - x0))
	{
		/*swap x_i and y_i */
		int temp = x0;
		x0 = y0;
		y0 = temp;
		temp = x1;
		x1 = y1;
		y1 = temp;
		steepSlope = 1;
	}

	int inc = 1;
	int dx = x1 - x0;
	int dy = y1 - y0;

	/* saves the headache of coding cases for another octant */
	if(x0 > x1)
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}

	/* if negative slope, dec. y instead if inc. y */
	if(((dy < 0) && (dx > 0)) || ((dy > 0) && (dx < 0)))
	{
		inc = -1;
	}

	int D = (dy << 1) - dx;
	int y = y0;

	for (int x = x0; x < (x1 + 1); x++)
	{

	  if (D > 0)
	  {
		  y += inc;

		  if (steepSlope)
		  {
			  setPixel(y, x, color);
		  }
		  else
		  {
			  setPixel(x, y, color);
		  }

		  D = D + (ABS(dy << 1) - ABS(dx << 1));
	  }
	  else
	  {
		  if (steepSlope)
		  {
			  setPixel(y, x, color);
		  }
		  else
		  {
			  setPixel(x, y, color);
		  }
		  D = D + ABS(dy << 1);

	  }

	}

}

/* x0 == x2, y0 == y2 */
void drawTri(int x0, int y0, int x1, int y1, int x2, int y2, u16 color)
{
	for (int i = x0; i <= x2; i++)
	{
		plotLine(x1, y1, i, y2, color);
	}
}

void drawTriangle(Triangle* t, u16 color)
{
	drawTri(t->x1, t->y1, t->x2, t->y2, t->x3, t->y3, color);
}

/**
 * Checks if 2 rectangles intersect.
 * Returns 0 if no intersection, 1 if an intersection.
 */
int intersects(Rectangle* r1, Rectangle* r2)
{
	/* Check if r2 is to the left of r1 */
	if ((r2->x + r2->width) < r1->x)
	{
		return 0;
	}

	/* Check if r2 is to the right of r1 */
	if (r2->x > (r1->x + r1->width))
	{
		return 0;
	}

	/* Check if r2 is above r1 */
	if ((r2->y + r2->height) < r1->y)
	{
		return 0;
	}

	/* Check if r2 is below r1 */
	if (r2->y > (r1->y + r1->height))
	{
		return 0;
	}

	/* if r2 is not outside r1's bounds, it must be in it */
	return 1;

}


Rectangle* cRectangle(Rectangle* retval, int x, int y, int width, int height, u16 color)
{
	retval->x = x;
	retval->y = y;
	retval->width = width;
	retval->height = height;
	retval->color = color;

	return retval;
}

RectMover* cRectMover(RectMover* mover, Rectangle* pr, int dx, int dy)
{
	mover->pr = pr;
	mover->dx = dx;
	mover->dy = dy;

	return mover;

}

/**
 * m is a Mover struct. a Mover contains a rectangle, a dx, and a dy.
 * update(Mover) adjusts the Mover's Rectangle according to its dx and dy.
 * Takes in a pointer so we can adjust the actual "object"
 */
void updateRectMover(RectMover* m)
{

	int x, y, width, height, dx, dy;
	x = m->pr->x;
	y = m->pr->y;
	width = m->pr->width;
	height = m->pr->height;
	dx = m->dx;
	dy = m->dy;

	/* Add some checks for bouncing */

	if (((x + dx) < 0) || ((x + width + dx) >= SCREEN_WIDTH))
	{
		m->dx = -(m->dx);
	}

	if (((y + dy) < 0) || ((y + height + dy) >= SCREEN_HEIGHT))
	{
		m->dy = -(m->dy);
	}

	drawRectangle(m->pr, BLACK);
	m->pr->x += m->dx;
	m->pr->y += m->dy;
	drawRectangle(m->pr, m->pr->color);
}

/*
 * Updates an ImgMover. ImgMovers do not reflect off the edges of the screen,
 * but instead just disappear
 */
boolean updateImgMover(ImgMover* img, u16 bgColor)
{
	int adjX = img->rect->x + img->dx;
	int adjY = img->rect->y + img->dy;
	int adjWidth = img->rect->width;
	int adjHeight = img->rect->height;

	Rectangle r = *(img->rect);

	if ((adjX + img->rect->width) >= SCREEN_WIDTH)
	{
		adjWidth = SCREEN_WIDTH - adjX;
	}
	else if (adjX <= 0)
	{
		adjWidth += img->dx;
		adjX = 0;

	}

	if ((adjY + img->rect->height) >= VIEWPORT_HEIGHT)
	{
		adjHeight = VIEWPORT_HEIGHT - adjY;
	}
	else if (adjY <= 0)
	{
		adjHeight += img->dy;
		adjY = 0;

	}

	if (adjWidth <= 0 || adjHeight <= 0)
	{
		return TRUE;
	}

	img->rect->x = adjX;
	img->rect->y = adjY;
	waitForVBlank();
	drawRectangle(&r, bgColor);
	drawPartialImage(img->rect->x, img->rect->y, img->origWidth, img->origHeight, img->img,
			adjWidth, adjHeight, (!adjX || !adjY));
	img->rect->width = adjWidth;
	img->rect->height = adjHeight;
	return FALSE;
}

void waitForVBlank()
{
	while(SCANLINECTR >= 160);
	while(SCANLINECTR < 160);
}


