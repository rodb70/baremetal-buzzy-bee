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
#ifndef MYLIB_H
#define MYLIB_H
#include <stdint.h>
#include "hardware.h"

// *** General ======================================================
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
#define OFFSET(r, c, rowlen) ((c)*(rowlen)+(r))

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
typedef struct
{
    int x;
    int y;
    int w;
    int h;
    int score;
} STR_BUZZ;

typedef struct
{
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

typedef struct
{
    int x;
    int y;
    int length;
    int active;
} PROJECTILE;

extern PILLAR pilA;
extern PILLAR pilB;
extern PILLAR *A;
extern PILLAR *B;

extern PROJECTILE projectile;
extern PROJECTILE *proj;

#define COLOR(r,g,b) ((r) + ((g) << 5) + ((b) << 10))

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

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

// GAME STATE PROTOTYPES
int title(void); // 0
int menu(STR_BUZZ *bP); // 1
int levelSelect(STR_BUZZ *bP); // 2
int countdown(STR_BUZZ *bP); // 3
int play(STR_BUZZ *bP); // 4
int tutorial(STR_BUZZ *bP); // 5
int controls(STR_BUZZ *bP); // 6
int deathAnim(STR_BUZZ *bP); // 7
int gameOver(STR_BUZZ *bP); // 8

// STRUCTURE CREATION PROTOTYPES
void createBuzz(STR_BUZZ *bP);
void createPillar(PILLAR *p, int speed);
void createProjectile(PROJECTILE *p, int x, int y);
void removeProjectile(PROJECTILE *p);

// DRAWING PROTOTYPES
void setPixel(int row, int col, unsigned short color);
void drawChar(int row, int col, char ch, unsigned short color);
void drawString(int col, int row, char str[], unsigned short color);
void drawBuzz(int y, int x, const uint16_t *buzz);
void drawRectangle(int col, int row, int width, int height, unsigned short color);
void drawImage(int row, int col, int width, int height, const unsigned short *image);

// OTHER PROTOTYPES
void waitForVblank(void);
void updatePillar(PILLAR *p, int speed, volatile uint16_t color);
int projCollision(int x, int y, int speed, PILLAR *p);
int buzzCollision(int x, int y, PILLAR *p);

#endif
