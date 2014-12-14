#define FONT_WIDTH 6
#define FONT_HEIGHT 8
#define CHAR_SPACING 1
#define FONT_AREA ((FONT_WIDTH) * (FONT_HEIGHT))
#define FONT_PIX(ascii, x, y) ((ascii) * (FONT_AREA) + (x) + (y) * (FONT_WIDTH))

extern unsigned char fontdata_6x8[];

//typedef unsigned char uchar;

void drawChar(char c, int x, int y, unsigned short color);
void drawString(char* string, int x, int y, unsigned short color);
