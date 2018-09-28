// MARK WALKER
// #903112725
// markwalker@gatech.edu

//////////////////////////////////////////////////////////////////////
//
//  INDEX
// 
//////////////////////////////////////////////////////////////////////
/* 
        General
        Game Options
        Video
        DMA
        Input

====================================================================*/

// *** General ======================================================

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef char s8;
typedef short s16;
typedef int s32;

#define TRUE 1
#define FALSE 0


// *** Game Options ================================================

#define S_TITLE 		0
#define S_MENU			1
#define S_LEVELSELECT	2
#define S_COUNTDOWN		3
#define S_PLAY			4
#define S_TUTORIAL		5
#define S_CONTROLS		6
#define S_DEATH			7
#define S_GAMEOVER		8

#define movespeed 		3 // How many pixels/cycle Buzz can move

extern char text[41];
extern int count;
extern int delay;
extern int maxDel; 
extern int lives;
extern int speed;

// GAME STRUCTURES
typedef struct {
	int x;
	int y;
	int w;
	int h;
	int score;
} STR_BUZZ;

typedef struct {
	// Shared characteristics
	int speed;
	int x;
	int width;
	int passed;
	int gain;
	// Upper Section
	int yUp;
	int hUp;
	//Middle Section
	int yMid;
	int hMid;
	int active;
	//Lower Section
	int yDown;
	int hDown;
} PILLAR;

typedef struct {
	int x;
	int y;
	int length;
	int active;
} PROJECTILE;

PILLAR pilA;
PILLAR pilB;
PILLAR* A;
PILLAR* B;

PROJECTILE projectile;
PROJECTILE* proj;
// *** Video =======================================================

/* Mode 3 */
extern u16* videoBuffer;               // 0x6000000
// Need: unsigned short *videoBuffer = (unsigned short *)0x6000000;
#define REG_DISPCNT  *(u16*) 0x4000000
#define OFFSET(r, c, rowlen) ((r)*(rowlen)+(c))
#define SCANLINECOUNTER *(unsigned short *)0x4000006

#define COLOR(r,g,b) ((r) + ((g) << 5) + ((b) << 10))

//Background Enables
#define BG0_ENABLE  (1<<8)
#define BG1_ENABLE  (1<<9)
#define BG2_ENABLE  (1<<10)
#define BG3_ENABLE  (1<<11)

#define BLACK   COLOR(0,0,0)
#define WHITE   COLOR(31,31,31)
#define GRAY    COLOR(20,20,20)
#define RED     COLOR(31,0,0)
#define GREEN   COLOR(0,31,0)
#define BLUE    COLOR(0,0,20)
#define LTBLUE  COLOR(10,10,20)
#define SKY    	COLOR(9,24,25)
#define YELLOW  COLOR(31,31,0)
#define YELORA	COLOR(31,24,0)
#define ORANGE	COLOR(31,16,0)
#define REDORA	COLOR(31,8,0)

extern const unsigned char fontdata_6x8[12288];

#define MODE_0 0
#define MODE_1 1
#define MODE_2 2
#define MODE_3 3
#define MODE_4 4
#define MODE_5 5
#define MODE_6 6

// *** DMA =======================================================

typedef struct{
	const volatile void *src;
	const volatile void *dst;
	u32                  cnt;
} DMA_CONTROLLER;
#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
// DMA channels
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

// Mostly just used for sound.
#define DMA_REPEAT (1 << 25)

// DMA copies 16 bits at a time normally (1 pixel), 
// but can be set to copy 32 bits at a time to be 
// even faster for mutliples of 32 bits.
#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

// Don't worry about these, the default DMA_NOW is good enough
// for 2110, if you want to go beyond the scope of the class you
// can ask a TA or the internet about the other options.
#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)


// *** Input =========================================================

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define BUTTONS (*(unsigned int *)0x4000130)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

// GAME STATE PROTOTYPES
int title();					// 0
int menu(STR_BUZZ *bP);			// 1
int levelSelect(STR_BUZZ *bP);	// 2
int countdown(STR_BUZZ *bP);	// 3
int play(STR_BUZZ *bP);			// 4
int tutorial(STR_BUZZ *bP);		// 5
int controls(STR_BUZZ *bP);		// 6
int deathAnim(STR_BUZZ *bP);	// 7
int gameOver(STR_BUZZ *bP);		// 8

// STRUCTURE CREATION PROTOTYPES
void createBuzz(STR_BUZZ* bP);
void createPillar(PILLAR* p, int speed);
void createProjectile(PROJECTILE *p, int x, int y);
void removeProjectile(PROJECTILE *p);

// DRAWING PROTOTYPES
void setPixel(int row, int col, unsigned short color);
void drawChar(int row, int col, char ch, unsigned short color);
void drawString(int row, int col, char str[], unsigned short color);
void drawBuzz(int x, int y, const u16* buzz);
void drawRectangle(int row, int col, int width, int height, volatile u16 color);
void drawImage(int row, int col, int width, int height, const u16* image);

// OTHER PROTOTYPES
void waitForVblank();
void updatePillar(PILLAR* p, int speed, volatile u16 color);
int projCollision(int x, int y, int speed, PILLAR* p);
int buzzCollision(int x, int y, PILLAR* p);