/* Definitions */

/* === Useful macros === */
#define OFFSET(x,y) (((y) << 8) - ((y) << 4) + (x)) /*y<<8 - y<<4 = (y * 240) */
#define ABS(x) (((x) >= 0) ? (x) : -(x))
#define RGB(r, g, b) ( ((b) << 10) | ((g) << 5) | (r) ) /*assumes valid parameters */
/* === GBA Constants === */
#define REG_DISPCNT (*(u16 *) 0x4000000)
#define SCANLINECTR (*(volatile u16 *) 0x4000006)
#define MODE3 3
#define BG2_ENABLE (1 << 10)
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define VIEWPORT_HEIGHT 120
#define BUTTON_ADDRESS (*(volatile u16*) 0x4000130)
/* === Button Stuff === */
#define BUTTON_PRESSED(button) (~BUTTON_ADDRESS & (button))
#define BUTTON_A 1
#define BUTTON_B (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START (1<<3)
#define BUTTON_RIGHT (1<<4)
#define BUTTON_LEFT (1<<5)
#define BUTTON_UP (1<<6)
#define BUTTON_DOWN (1<<7)
#define BUTTON_R (1<<8)
#define BUTTON_L (1<<9)
/* === Colors === */
#define BLACK 0
#define RED RGB(31, 0, 0)
#define BLUE RGB(0, 0, 31)
#define GREEN RGB(0, 31, 0)
#define GRASS RGB(0, 20, 0);
#define YELLOW RGB(31, 31, 0)
#define PURPLE RGB(31, 0, 31)
#define CYAN RGB(0, 31, 31)
#define WHITE RGB(31, 31, 31)
#define ORANGE RGB(30, 18, 0)
/* === DMA Constants === */
/* Used all of the DMA constants from Bill's code. */
#define REG_DMA0SAD         *(const volatile u32*)0x40000B0 // source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(const volatile u32*)0x40000BC // source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(const volatile u32*)0x40000C8 // source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(const volatile u32*)0x40000D4 // source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register

typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)


/* === General Programming Constants === */
#define TRUE 1
#define FALSE 0
/* === Typedefs === */
typedef unsigned short u16;
typedef unsigned char boolean;
typedef unsigned int u32;
typedef struct
{
	int x; /* Refers to upper left corner */
	int y; /* Refers to upper left corner */
	int width;
	int height;
	u16 color;

} Rectangle;

typedef struct
{
	int x1, y1, x2, y2, x3, y3;
	u16 color;

} Triangle;

typedef struct
{
	int x; /* same conventions as a rectangle */
	int y;
	int width;
	int height;
	u16 color;

} HollowRectangle;


typedef struct
{
	Rectangle *pr; /*pointer to a rectangle */
	int dx; /* movement along x */
	int dy; /* movement along y */
	int hostile; /* checks if a collision is bad */

} RectMover;

typedef struct
{
	Rectangle* rect;
	const unsigned short* img;
	int dx;
	int dy;
	int origWidth;
	int origHeight;
} ImgMover;

typedef union
{
	Rectangle r;
	HollowRectangle h;
	Triangle t;

	int type; /* 0: rectangle, 1: HollowRectangle, 2: Triangle */
} Shape;

/* === Function prototypes === */

void setPixel(int x, int y, u16 color);

void drawRectangle(Rectangle* r, u16 color);

void drawImage3(int x, int y, int width, int height, const unsigned short* img);

void drawPartialImage(int x, int y, int width, int height, const u16* img,
		int partialWidth, int partialHeight, int decreasing);

void drawTriangle(Triangle* t, u16 color);

void drawHollowRectangle(HollowRectangle h);

void drawRect(int x, int y, int width, int height, u16 color);

void drawHollowRect(int x, int y, int width, int height, u16 color);

void plotLine(int x0, int y0, int x1, int y1, u16 color);

void drawTri(int x0, int y0, int x1, int y1, int x2, int y2, u16 color);

void drawGT(int x, int y, int size, u16 color);

int intersects(Rectangle* rect1, Rectangle* rect2);

void updateRectMover(RectMover* m);

boolean updateImgMover(ImgMover* img, u16 bgColor);

Rectangle* cRectangle(Rectangle* r, int x, int y, int width, int height, u16 color);

RectMover* cRectMover(RectMover*, Rectangle*, int dx, int dy);

void waitForVBlank();


