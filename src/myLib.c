// MARK WALKER
// #903112725
// markwalker@gatech.edu

#include <stdlib.h>

#include "mylib.h"

void drawChar(int row, int col, char ch, unsigned short color)
{
    for( int c = 0; c < 8; c++ )
    {
        for( int r = 0; r < 6; r++ )
        {
            if( fontdata_6x8[OFFSET(r, c, 6) + ch * 48] == 1 )
            {
                setPixel( r + row, c + col, color );
            }
        }

    }
}

void drawString(int col, int row, char str[], unsigned short color)
{
    while( *str )
    {
        drawChar( row, col, *str++, color );
        row += 6;
    }
}

void createBuzz(STR_BUZZ *bP)
{
    bP->x = 50;
    bP->y = 50;
    bP->w = 20;
    bP->h = 20;
    bP->score = 0;
}

void createPillar(PILLAR *p, int speed)
{
    // Gets a random number between 0 and 70
    int random = rand() % 81;
    // Shared characteristics
    p->width = speed;
    p->x = 240 - speed;
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

void createProjectile(PROJECTILE *p, int x, int y)
{
    p->x = x + 21;
    p->y = y + 8;
    p->length = 1;
    p->active = 1;
}

void removeProjectile(PROJECTILE *p)
{
    p->x = 0;
    p->y = 0;
    p->length = 0;
    p->active = 0;
}

void updatePillar(PILLAR *p, int speed, volatile uint16_t color)
{
    int x = p->x;
    uint16_t mid = LTBLUE;
    if( color == SKY )
    {
        // erase: x = rightmost col of pillar
        x = x + p->width;
        mid = SKY;
    }
    if( p->active == 1 )
    {
        drawRectangle( p->yMid, x, speed, p->hMid, mid );
    }
    drawRectangle( 0, x, speed, p->hUp, color );
    drawRectangle( p->yDown, x, speed, p->hDown, color );
}

int projCollision(int x, int y, int speed, PILLAR *p)
{
    int result = 0;
    // Check if it hits pillar
    if( (x > (p->x)) && (x < ((p->x) + (p->width))) )
    {
        result++;
        // Check if it hits the center part of the pillar & its active
        if( (y > p->yMid) && (y < ((p->yMid) + (p->hMid))) && ((p->active) == 1) )
        {
            result++;
            drawRectangle( p->yMid, p->x, (p->width) + speed, p->hMid, SKY );
        }
        else if( (y > (p->yMid)) && (y < ((p->yMid) + (p->hMid))) )
        {
            result = 0;
        }
    }
    return result;
}

int buzzCollision(int x, int y, PILLAR *p)
{
    // Checks Top Right corner
    if( (x + 20 > (p->x)) && (x + 20 < ((p->x) + (p->width))) )
    {
        if( (y < p->yMid) || (y > ((p->yMid) + (p->hMid))) || ((p->active) == 1) )
        {
            return 1;
        }
    }
    // Checks Bottom Right corner
    if( (x + 20 > (p->x)) && (x + 20 < ((p->x) + (p->width))) )
    {
        if( (y + 20 < p->yMid) || (y + 20 > ((p->yMid) + (p->hMid))) || ((p->active) == 1) )
        {
            return 1;
        }
    }
    // Checks Top Left corner
    if( (x > (p->x)) && (x < ((p->x) + (p->width))) )
    {
        if( (y + 20 < p->yMid) || (y + 20 > ((p->yMid) + (p->hMid))) || ((p->active) == 1) )
        {
            return 1;
        }
    }
    // Checks Bottom Left corner
    if( (x > (p->x)) && (x < ((p->x) + (p->width))) )
    {
        if( (y < p->yMid) || (y > ((p->yMid) + (p->hMid))) || ((p->active) == 1) )
        {
            return 1;
        }
    }
    return 0;

    // Increments counter if successfully passes
    if( x == p->x + p->width )
    {
        return 2;
    }
}
