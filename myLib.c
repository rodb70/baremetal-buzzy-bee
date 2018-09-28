// MARK WALKER
// #903112725
// markwalker@gatech.edu

#include <stdlib.h>
#include "myLib.h"

u16* videoBuffer = (u16*)0x6000000;


// ***=========================================================


void setPixel(int row, int col, u16 color) {
    videoBuffer[row * 240 + col] = color;
}
void drawChar(int row, int col, char ch, unsigned short color) {
    for(int r = 0; r< 8; r++) {
        for(int c=0; c< 6; c++) {
            if( fontdata_6x8[OFFSET(r, c, 6) + ch*48] == 1) {
                setPixel(r+row, c+col, color);
            }
        }

    }
}

void drawString(int row, int col, char str[], unsigned short color) {
    while(*str) {
        drawChar(row, col, *str++, color);
        col += 6;
    }
}

void waitForVblank() {
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 161);
}

void drawBuzz(int x, int y, const u16* buzz) {
    for (int r = 0; r < 20; r++) {
        DMA[3].src = buzz + (r*20);
        DMA[3].dst = (videoBuffer + OFFSET(x+r, y, 240));
        DMA[3].cnt = (20) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
    }
}
void drawRectangle(int row, int col, int width, int height, volatile u16 color) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)];
        DMA[3].cnt = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
    }
}

void drawImage(int row, int col, int width, int height, const u16* image) {
    for (int r = 0; r < height; r++) {
        // pointer to first pixel of image + (position of each row)
        DMA[3].src = image + (r * width);
        DMA[3].dst = (videoBuffer + OFFSET(row+r, col, 240));
        DMA[3].cnt = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT;
    }
}

void createBuzz(STR_BUZZ* bP) {
    bP->x = 50;
    bP->y = 50;
    bP->w = 20;
    bP->h = 20;
    bP->score = 0;
}

void createPillar(PILLAR* p, int speed) {
    // Gets a random number between 0 and 70
    int random = rand()%81;
    // Shared characteristics
    p->width = speed;
    p->x = 240-speed;
    p->passed = 0;
    p->gain = 0;
    // Upper section
    p->yUp = 0;
    p->hUp = (5 + random);
    // Middle section
    p->yMid = 5 + random;
    p->hMid = 50;
    p->active = 1;
    // Lower section
    p->yDown = 55 + random;
    p->hDown = 85 - random;
}

void createProjectile(PROJECTILE *p, int x, int y) {
    p->x = x + 21;
    p->y = y + 8;
    p->length = 1;
    p->active = 1;
}

void removeProjectile(PROJECTILE *p) {
    p->x = 0;
    p->y = 0;
    p->length = 0;
    p->active = 0;
}

void updatePillar(PILLAR* p, int speed, volatile u16 color) {
    int x = p->x;
    u16 mid = LTBLUE;
    if(color == SKY) {
        // erase: x = rightmost col of pillar
        x = x + p->width;
        mid = SKY;
    }
    if(p->active == 1) {
        drawRectangle(p->yMid, x, speed, p->hMid, mid);
    }
    drawRectangle(0, x, speed, p->hUp, color);
    drawRectangle(p->yDown, x, speed, p->hDown, color);
}

int projCollision(int x, int y, int speed, PILLAR* p) {
    int result = 0;
    // Check if it hits pillar
    if((x > (p->x)) && (x < ((p->x) + (p->width)))) {
        result++;
        // Check if it hits the center part of the pillar & its active
        if((y > p->yMid) && (y < ((p->yMid) + (p->hMid))) && ((p->active) == 1)) {
            result++;
            drawRectangle(p->yMid, p->x, (p->width) + speed, p->hMid, SKY);
        } else if((y > (p->yMid)) && (y < ((p->yMid) + (p->hMid)))) {
            result = 0;
        }
    }
    return result;
}

int buzzCollision(int x, int y, PILLAR* p) {
    // Checks Top Right corner
    if((x+20 > (p->x)) && (x+20 < ((p->x) + (p->width)))) {
        if((y < p->yMid) || (y > ((p->yMid) + (p->hMid))) || ((p->active) == 1)) {
            return 1;
        }
    }
    // Checks Bottom Right corner
    if((x+20 > (p->x)) && (x+20 < ((p->x) + (p->width)))) {
        if((y+20 < p->yMid) || (y+20 > ((p->yMid) + (p->hMid))) || ((p->active) == 1)) {
            return 1;
        }
    }
    // Checks Top Left corner
    if((x > (p->x)) && (x < ((p->x) + (p->width)))) {
        if((y+20 < p->yMid) || (y+20 > ((p->yMid) + (p->hMid))) || ((p->active) == 1)) {
            return 1;
        }
    }
    // Checks Bottom Left corner
   if((x > (p->x)) && (x < ((p->x) + (p->width)))) {
        if((y < p->yMid) || (y > ((p->yMid) + (p->hMid))) || ((p->active) == 1)) {
            return 1;
        }
    }
    return 0;

    // Increments counter if successfully passes
    if(x == p->x + p->width) {
        return 2;
    }
}