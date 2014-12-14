#include "textlib.h"
#include "../main/mylib.h"

void drawChar(char c, int x, int y, u16 color)
{
	for (int i = 0; i < FONT_WIDTH; i++)
	{
		for(int j = 0; j < FONT_HEIGHT; j++)
		{
			if (fontdata_6x8[FONT_PIX(c, i, j)])
			{
				setPixel(x + i, y + j, color);
			}

		}
	}
}


void drawString(char* string, int x, int y, u16 color)
{
	int origX = x;
	int i = 0;
	while (string[i])
	{
		drawChar(string[i], x, y, color);
		x += FONT_WIDTH + CHAR_SPACING;
		if (x > SCREEN_WIDTH)
		{
			x = origX;
			y += FONT_HEIGHT + CHAR_SPACING;

		}
		++i;
	}
}
