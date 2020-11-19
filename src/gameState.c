// MARK WALKER
// #903112725
// markwalker@gatech.edu

#include <stdio.h>
#include "images.h"
#include "mylib.h"

// Global data
char text[41];
int count;
int PILDIST; // Amount of pixels between each pillar
int delay;
int maxDel;
int lives;
int speed;

PILLAR pilA;
PILLAR pilB;
PILLAR *A;
PILLAR *B;

PROJECTILE projectile;
PROJECTILE *proj;

// 0: TITLE PAGE
int title(void)
{
    // Draws initial screen
    drawImage( 0, 0, TITLE_WIDTH, TITLE_HEIGHT, title_data );
    count = 0;
    drawRectangle( 140, 0, 240, 20, BLACK );
    sprintf( text, "PRESS START TO PLAY!" );
    while( !KEY_DOWN_NOW( BUTTON_START ) )
    {
        if( count == 0 )
        {
            drawString( 148, 60, text, YELLOW );
        }
        else if( count == 40 )
        {
            drawRectangle( 140, 0, 240, 20, BLACK );
        }
        waitForVblank();
        // Count is used to implement a flashing text
        if( count == 80 )
        {
            count = 0;
        }
        else
        {
            count++;
        }
    }
    // when player hits start, move to MENU
    return S_MENU;
}
// 1: MENU
int menu(STR_BUZZ *bP)
{
    int gameState = S_MENU;
    count = 1;
    // Create projectile pointer
    proj = &projectile;
    removeProjectile( proj );
    // Create first instance of buzz object
    createBuzz( bP );
    // Draw initial game frame;
    drawRectangle( 0, 0, 240, 140, SKY );
    drawRectangle( 140, 0, 240, 20, BLACK );
    sprintf( text, "[A] FIRE AT CHOICE" );
    drawString( 148, 10, text, YELLOW );
    sprintf( text, "[SELECT] EXIT" );
    drawString( 148, 150, text, YELLOW );
    while( gameState == S_MENU )
    {
        // Draws buzz at the new location
        drawBuzz( bP->y, bP->x, buzz_data );
        if( count < 35 )
        {
            // Slides out & grows option boxes
            drawRectangle( 25, 240 - count * 2, count * 2, 20, BLACK );
            drawRectangle( 55, 240 - count * 2, count * 2, 20, BLACK );
            drawRectangle( 85, 240 - count * 2, count * 2, 20, BLACK );
        }
        else if( count < 50 )
        {
            // Slides out option boxes
            drawRectangle( 25, 240 - count * 2, 70, 20, BLACK );
            drawRectangle( 55, 240 - count * 2, 70, 20, BLACK );
            drawRectangle( 85, 240 - count * 2, 70, 20, BLACK );
        }
        else if( count == 50 )
        {
            // Adds text to option boxes
            sprintf( text, "Play" );
            drawString( 32, 145, text, YELLOW );
            sprintf( text, "Tutorial" );
            drawString( 62, 145, text, YELLOW );
            sprintf( text, "Controls" );
            drawString( 92, 145, text, YELLOW );
        }
        //if projectile active, draw it
        if( proj->active == 1 )
        {
            drawRectangle( proj->y, proj->x, proj->length, 2, BLACK );
        }
        waitForVblank();
        // Undraws buzz at the old location
        drawRectangle( bP->y, bP->x, 20, 20, SKY );
        if( count < 49 )
        {
            // Cleans up after the shifting option boxes
            drawRectangle( 25, 240 - count * 2, 70, 20, SKY );
            drawRectangle( 55, 240 - count * 2, 70, 20, SKY );
            drawRectangle( 85, 240 - count * 2, 70, 20, SKY );
        }
        // Draws a projectile after it's been fired
        if( proj->active == 1 )
        {
            drawRectangle( proj->y, proj->x, proj->length, 2, SKY );
        }
        // Pull info from user input
        if( KEY_DOWN_NOW( BUTTON_UP ) )
        {
            bP->y -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_DOWN ) )
        {
            bP->y += movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_LEFT ) )
        {
            bP->x -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_RIGHT ) )
        {
            bP->x += movespeed;
        }
        // Don't let buzz leave the desired area
        // Gave it a nice *bouncy* feel to borders
        if( bP->y < 0 )
        {
            bP->y += movespeed + 1;
        }
        else if( bP->y > 120 )
        {
            bP->y -= movespeed + 1;
        }
        if( bP->x < 0 )
        {
            bP->x += movespeed + 1;
        }
        else if( bP->x > 110 )
        {
            bP->x -= movespeed + 1;
        }
        if( KEY_DOWN_NOW(BUTTON_A) && proj->active == 0 )
        {
            createProjectile( proj, bP->x, bP->y );
        }
        // Shift projectile
        if( proj->active == 1 && (proj->x + proj->length < 220) )
        {
            proj->x += 4;
            if( proj->length < 12 )
            {
                proj->length++;
            }
        }
        else
        {
            removeProjectile( proj );
        }
        // Check where the player shoots
        if( count > 50 && ((proj->x) > 205) )
        {
            if( ((proj->y) > 25) && ((proj->y) < 45) )
            {
                // go to level select screen
                gameState = S_LEVELSELECT;
            }
            else if( ((proj->y) > 55) && ((proj->y) < 75) )
            {
                gameState = S_TUTORIAL;
            }
            else if( ((proj->y) > 85) && ((proj->y) < 105) )
            {
                gameState = S_CONTROLS;
            }
        }
        else
        {
            count++;
        }
        if( KEY_DOWN_NOW(BUTTON_SELECT) && count > 30 )
        {
            // Select: returns you to title
            gameState = S_TITLE;
        }
    }
    return gameState;
}
// 2: LEVELSELECT
int levelSelect(STR_BUZZ *bP)
{
    int gameState = S_LEVELSELECT;
    count = 1;
    // Create pillar
    A = &pilA;
    B = &pilB;
    // Create projectile
    proj = &projectile;
    removeProjectile( proj );
    // Draw initial game frame
    sprintf( text, "[A] SELECT DIFFICULTY" );
    drawRectangle( 0, 0, 240, 140, SKY );
    drawRectangle( 140, 0, 240, 20, BLACK );
    drawString( 148, 10, text, YELLOW );
    sprintf( text, "[SELECT] EXIT" );
    drawString( 148, 150, text, YELLOW );
    while( gameState == S_LEVELSELECT )
    {
        // Draw buzz at his new location
        drawBuzz( bP->y, bP->x, buzz_data );
        if( count < 35 )
        {
            // Draw the expanding choice boxes
            drawRectangle( 15, 240 - count * 2, count * 2, 20, BLACK );
            drawRectangle( 45, 240 - count * 2, count * 2, 20, BLACK );
            drawRectangle( 75, 240 - count * 2, count * 2, 20, BLACK );
            drawRectangle( 105, 240 - count * 2, count * 2, 20, BLACK );
        }
        else if( count < 50 )
        {
            // Draw the sliding choice boxes
            drawRectangle( 15, 240 - count * 2, 70, 20, BLACK );
            drawRectangle( 45, 240 - count * 2, 70, 20, BLACK );
            drawRectangle( 75, 240 - count * 2, 70, 20, BLACK );
            drawRectangle( 105, 240 - count * 2, 70, 20, BLACK );
        }
        else if( count == 50 )
        {
            // Add text to the choice boxes
            sprintf( text, "Easy" );
            drawString( 22, 145, text, YELLOW );
            sprintf( text, "Medium" );
            drawString( 52, 145, text, YELLOW );
            sprintf( text, "Hard" );
            drawString( 82, 145, text, YELLOW );
            sprintf( text, "Impossible" );
            drawString( 112, 145, text, YELLOW );
        }
        //if projectile active, draw it
        if( proj->active == 1 )
        {
            drawRectangle( proj->y, proj->x, proj->length, 2, BLACK );
        }
        waitForVblank();
        // Undraw buzz at the old location
        drawRectangle( bP->y, bP->x, bP->w, bP->h, SKY );
        if( count < 49 )
        {
            // Clean up after shifting choice boxes
            drawRectangle( 15, 240 - count * 2, 70, 20, SKY );
            drawRectangle( 45, 240 - count * 2, 70, 20, SKY );
            drawRectangle( 75, 240 - count * 2, 70, 20, SKY );
            drawRectangle( 105, 240 - count * 2, 70, 20, SKY );
        }
        // Draw projectile after it's been fired
        if( proj->active == 1 )
        {
            drawRectangle( proj->y, proj->x, proj->length, 2, SKY );
        }
        // Use info from user input
        if( KEY_DOWN_NOW( BUTTON_UP ) )
        {
            bP->y -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_DOWN ) )
        {
            bP->y += movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_LEFT ) )
        {
            bP->x -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_RIGHT ) )
        {
            bP->x += movespeed;
        }
        // Don't let buzz leave the screen/area
        // BTW, the shortened range is intentional
        //  in the menu. He can move wherever during
        //  actual gameplay
        if( bP->y < 0 )
        {
            bP->y += movespeed + 1;
        }
        else if( bP->y > 120 )
        {
            bP->y -= movespeed + 1;
        }
        if( bP->x < 0 )
        {
            bP->x += movespeed + 1;
        }
        else if( bP->x > 110 )
        {
            bP->x -= movespeed + 1;
        }
        // Creates a projectile if there isn't already one
        if( KEY_DOWN_NOW(BUTTON_A) && proj->active == 0 )
        {
            createProjectile( proj, bP->x, bP->y );
        }
        // Shift projectile
        if( proj->active == 1 && (proj->x + proj->length < 220) )
        {
            proj->x += 4;
            if( proj->length < 12 )
            {
                proj->length++;
            }
        }
        else
        {
            removeProjectile( proj );
        }
        // See what the player chooses and update gameState
        if( ((proj->active) == 1) && count > 50 && ((proj->x) > 205) )
        {
            if( ((proj->y) > 15) && ((proj->y) < 35) )
            {
                A->speed = 1;
                B->speed = 1;
                gameState = S_COUNTDOWN;
            }
            else if( ((proj->y) > 45) && ((proj->y) < 65) )
            {
                A->speed = 2;
                B->speed = 2;
                gameState = S_COUNTDOWN;
            }
            else if( ((proj->y) > 75) && ((proj->y) < 95) )
            {
                A->speed = 3;
                B->speed = 3;
                gameState = S_COUNTDOWN;
            }
            else if( ((proj->y) > 105) && ((proj->y) < 125) )
            {
                A->speed = 5;
                B->speed = 5;
                gameState = S_COUNTDOWN;
            }
        }
        else
        {
            count++;
        }
        if( KEY_DOWN_NOW(BUTTON_SELECT) && count > 30 )
        {
            // if player hits select, go to menu
            gameState = S_MENU;
        }
    }
    return gameState;
}
// 3: COUNTDOWN
int countdown(STR_BUZZ *bP)
{
    int gameState = S_COUNTDOWN;
    count = 0;
    // Draw initial game frame
    drawRectangle( 0, 0, 240, 140, SKY );
    drawRectangle( 140, 0, 240, 20, BLACK );
    sprintf( text, "[SELECT] MENU" );
    drawString( 148, 150, text, YELLOW );
    while( gameState == S_COUNTDOWN && count < 120 )
    {
        // Draw buzz at his new location
        drawBuzz( bP->y, bP->x, buzz_data );
        // Huge if/else statement which updates an animated game countdown
        if( count == 0 )
        {
            sprintf( text, "Get Ready" );
            drawString( 148, 10, text, YELLOW );
        }
        else if( count == 30 )
        {
            sprintf( text, "5." );
            drawRectangle( 140, 5, 80, 20, BLACK );
            drawString( 148, 10, text, YELLOW );
        }
        else if( count == 35 )
        {
            sprintf( text, "5.." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, YELLOW );
        }
        else if( count == 40 )
        {
            sprintf( text, "5..." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, YELLOW );
        }
        else if( count == 45 )
        {
            sprintf( text, "4." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, YELORA );
        }
        else if( count == 50 )
        {
            sprintf( text, "4.." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, YELORA );
        }
        else if( count == 55 )
        {
            sprintf( text, "4..." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, YELORA );
        }
        else if( count == 60 )
        {
            sprintf( text, "3." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, ORANGE );
        }
        else if( count == 65 )
        {
            sprintf( text, "3.." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, YELORA );
        }
        else if( count == 70 )
        {
            sprintf( text, "3..." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, YELORA );
        }
        else if( count == 75 )
        {
            sprintf( text, "2." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, REDORA );
        }
        else if( count == 80 )
        {
            sprintf( text, "2.." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, REDORA );
        }
        else if( count == 85 )
        {
            sprintf( text, "2..." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, REDORA );
        }
        else if( count == 90 )
        {
            sprintf( text, "1." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, RED );
        }
        else if( count == 95 )
        {
            sprintf( text, "1.." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, RED );
        }
        else if( count == 100 )
        {
            sprintf( text, "1..." );
            drawRectangle( 140, 5, 30, 20, BLACK );
            drawString( 148, 10, text, RED );
        }
        else if( count == 105 )
        {
            gameState = S_PLAY;
        }
        waitForVblank();
        // Undraw buzz at his old location
        drawRectangle( bP->y, bP->x, bP->w, bP->h, SKY );
        // Use info from user input
        if( KEY_DOWN_NOW( BUTTON_UP ) )
        {
            bP->y -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_DOWN ) )
        {
            bP->y += movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_LEFT ) )
        {
            bP->x -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_RIGHT ) )
        {
            bP->x += movespeed;
        }
        // Don't let user leave the screen
        if( bP->y < 0 )
        {
            bP->y += movespeed + 1;
        }
        else if( bP->y > 120 )
        {
            bP->y -= movespeed + 1;
        }
        if( bP->x < 0 )
        {
            bP->x += movespeed + 1;
        }
        else if( bP->x > 220 )
        {
            bP->x -= movespeed + 1;
        }
        if( count > 30 && KEY_DOWN_NOW( BUTTON_SELECT ) )
        {
            // Select returns you to level select
            gameState = S_LEVELSELECT;
        }
        count++;
    }
    return gameState;
}
// 4: PLAY
int play(STR_BUZZ *bP)
{
    // Initialize a few important variables for the method
    int gameState = S_PLAY;
    int checkCol;
    lives = 1;
    delay = 0;
    bP->score = 0;
    speed = A->speed;
    maxDel = 120 / (A->speed);
    // Create Pillar Pointers & objects
    A = &pilA;
    B = &pilB;
    createPillar( A, speed );
    createPillar( B, speed );
    // Create Projectile pointer
    proj = &projectile;
    removeProjectile( proj );
    // Draw initial game frame
    drawRectangle( 0, 0, 240, 140, SKY );
    drawRectangle( 140, 0, 240, 20, BLACK );
    sprintf( text, "SCORE: %d", bP->score );
    drawString( 148, 10, text, YELLOW );
    sprintf( text, "LIVES: %d", lives );
    drawString( 148, 80, text, YELLOW );
    sprintf( text, "[SELECT] BACK" );
    drawString( 148, 150, text, YELLOW );
    while( gameState == S_PLAY && lives > 0 )
    {
        // Update and draw buzz, pillars,
        //  projectile(if there is one active),
        //  and active game text if it changes
        updatePillar( A, speed, BLUE );
        if( delay == maxDel )
        {
            updatePillar( B, speed, BLUE );
        }
        drawBuzz( bP->y, bP->x, buzz_data );
        if( proj->active == 1 )
        {
            drawRectangle( proj->y, proj->x, proj->length, 2, BLACK );
        }
        if( A->passed == 1 && A->gain == 0 )
        {
            A->gain = 1;
            bP->score += speed;
            drawRectangle( 140, 0, 80, 20, BLACK );
            sprintf( text, "SCORE: %d", bP->score );
            drawString( 148, 10, text, YELLOW );
        }
        else if( delay == maxDel && B->passed == 1 && B->gain == 0 )
        {
            B->gain = 1;
            bP->score += speed;
            drawRectangle( 140, 0, 80, 20, BLACK );
            sprintf( text, "SCORE: %d", bP->score );
            drawString( 148, 10, text, YELLOW );
        }
        waitForVblank();
        // Erase current buzz, pillar, and projectile (if it's active)
        updatePillar( A, speed, SKY );
        if( delay == maxDel )
        {
            updatePillar( B, speed, SKY );
        }
        drawRectangle( bP->y, bP->x, bP->w, bP->h, SKY );
        if( proj->active == 1 )
        {
            drawRectangle( proj->y, proj->x, proj->length, 2, SKY );
        }
        // Shift pillars
        if( (A->x < speed) && (A->width == speed) )
        {
            createPillar( A, speed );
            A->width = speed;
        }
        else if( A->x < speed )
        {
            A->width -= speed;
        }
        else if( A->x > 220 )
        {
            A->x -= speed;
            A->width += speed;
        }
        else
        {
            A->x -= speed;
        }
        if( delay == maxDel )
        {
            if( (B->x < speed) && (B->width == speed) )
            {
                createPillar( B, speed );
                B->width = speed;
            }
            else if( B->x == 0 )
            {
                B->width -= speed;
            }
            else if( B->x > 220 )
            {
                B->x -= speed;
                B->width += speed;
            }
            else
            {
                B->x -= speed;
            }
        }
        // Shift projectile (if active)
        if( proj->active == 1 && (proj->x + proj->length < 240) )
        {
            proj->x += 4;
            if( proj->length < 12 )
            {
                proj->length++;
            }
        }
        else
        {
            removeProjectile( proj );
        }
        // Resets game when player hits select
        if( KEY_DOWN_NOW( BUTTON_SELECT ) )
        {
            gameState = S_LEVELSELECT;
        }
        // Use info from user input
        if( KEY_DOWN_NOW( BUTTON_UP ) )
        {
            bP->y -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_DOWN ) )
        {
            bP->y += movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_LEFT ) )
        {
            bP->x -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_RIGHT ) )
        {
            bP->x += movespeed;
        }
        // Don't let buzz leave the screen
        if( bP->y < 0 )
        {
            bP->y += movespeed + 1;
        }
        else if( bP->y > 120 )
        {
            bP->y -= movespeed + 1;
        }
        if( bP->x < 0 )
        {
            bP->x += movespeed + 1;
        }
        else if( bP->x > 220 )
        {
            bP->x -= movespeed + 1;
        }
        // Create a projectile to draw if there isn't
        //  already one and a player hits [A]
        if( KEY_DOWN_NOW(BUTTON_A) && proj->active == 0 )
        {
            createProjectile( proj, bP->x, bP->y );
        }
        // Check for projectile collision with pillar A
        if( proj->active == 1 )
        {
            checkCol = projCollision( (proj->x + proj->length), proj->y, speed, A );
            if( checkCol == 1 )
            {
                removeProjectile( proj );
            }
            else if( checkCol == 2 )
            {
                removeProjectile( proj );
                A->active = 0;
            }
        }
        // Check for projectile collision with pillar B
        if( ((proj->active) == 1) && delay == maxDel )
        {
            checkCol = projCollision( (proj->x + proj->length), proj->y, speed, B );
            if( checkCol == 1 )
            {
                removeProjectile( proj );
            }
            else if( checkCol == 2 )
            {
                removeProjectile( proj );
                B->active = 0;
            }
        }
        // Check for buzz collision (all four corners)
        // 	& Checks if buzz has passed pillar A
        checkCol = buzzCollision( bP->x, bP->y, A );
        if( checkCol == 1 )
        {
            lives--;
            drawRectangle( 140, 80, 40, 20, BLACK );
            sprintf( text, "LIVES: %d", lives );
            drawString( 148, 80, text, YELLOW );
        }
        if( A->passed == 0 && (bP->x > (A->x + A->width)) )
        {
            A->passed = 1;
        }
        // Does the same checks with pillar B
        if( delay == maxDel )
        {
            checkCol = buzzCollision( bP->x, bP->y, B );
            if( checkCol == 1 )
            {
                lives--;
                drawRectangle( 140, 80, 40, 20, BLACK );
                sprintf( text, "LIVES: %d", lives );
                drawString( 148, 80, text, YELLOW );
            }
            if( A->passed == 0 && (bP->x > (B->x + B->width)) )
            {
                B->passed = 1;
            }
        }
        // Once you're out of lives, death
        //  animation plays
        if( lives == 0 )
        {
            // If you're out of lives, go to death animation
            gameState = S_DEATH;
        }
        // Increment delay until second pillar is drawn
        if( delay < maxDel )
        {
            delay++;
        }
    }
    return gameState;
}
// 5: TUTORIAL
int tutorial(STR_BUZZ *bP)
{
    int count = 0;
    int checkCol;
    // Create Pillar Pointers
    A = &pilA;
    B = &pilB;
    // Create Projectile Pointer
    proj = &projectile;
    removeProjectile( proj );
    // Create tutorial pillar object w/ a speed of 1
    createPillar( A, 1 );
    // Draw initial game frame
    drawRectangle( 0, 0, 240, 140, SKY );
    drawRectangle( 140, 0, 240, 20, BLACK );
    sprintf( text, "Tutorial:" );
    drawRectangle( 0, 75, 90, 30, BLACK );
    drawString( 5, 95, text, WHITE );
    while( count < 1100 )
    {
        // Tutorial instructions
        if( count == 0 )
        {
            sprintf( text, "Use Directional Buttons to move!" );
            drawString( 148, 20, text, YELLOW );
        }
        else if( count == 50 )
        {
            sprintf( text, "ENTER to skip" );
            drawString( 18, 81, text, GRAY );
        }
        else if( count == 200 )
        {
            sprintf( text, "Press A to shoot!" );
            drawRectangle( 140, 0, 240, 20, BLACK );
            drawString( 148, 70, text, YELLOW );
        }
        else if( count == 400 )
        {
            sprintf( text, "Shoot the weak spots on the pillars!" );
            drawRectangle( 140, 0, 240, 20, BLACK );
            drawString( 148, 10, text, YELLOW );
        }
        else if( count == 700 )
        {
            sprintf( text, "You get a point for each pillar!" );
            drawRectangle( 140, 0, 240, 20, BLACK );
            drawString( 148, 20, text, YELLOW );
        }
        else if( count == 900 )
        {
            sprintf( text, "You can reset at any time with SELECT!" );
            drawRectangle( 140, 0, 240, 20, BLACK );
            drawString( 148, 5, text, YELLOW );
        }
        // Draw buzz, a pillar (if active), and
        //  a projectile (if active)
        if( count > 400 )
        {
            updatePillar( A, 1, BLUE );
        }
        if( proj->active == 1 )
        {
            drawRectangle( proj->y, proj->x, proj->length, 2, BLACK );
        }
        drawBuzz( bP->y, bP->x, buzz_data );

        waitForVblank();
        // Undraw buzz, pillar, and projectile at
        //  their respective old locations
        drawRectangle( bP->y, bP->x, 20, 20, SKY );
        if( count > 400 )
        {
            updatePillar( A, 1, SKY );
        }
        if( proj->active == 1 )
        {
            drawRectangle( proj->y, proj->x, proj->length, 2, SKY );
        }
        // Get user input
        if( KEY_DOWN_NOW( BUTTON_UP ) )
        {
            bP->y -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_DOWN ) )
        {
            bP->y += movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_LEFT ) )
        {
            bP->x -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_RIGHT ) )
        {
            bP->x += movespeed;
        }
        // Don't let buzz leave the screen
        if( bP->y < 0 )
        {
            bP->y += movespeed + 1;
        }
        else if( bP->y > 120 )
        {
            bP->y -= movespeed + 1;
        }
        if( bP->x < 0 )
        {
            bP->x += movespeed + 1;
        }
        else if( bP->x > 220 )
        {
            bP->x -= movespeed + 1;
        }
        if( count > 30 && (KEY_DOWN_NOW(BUTTON_START) || KEY_DOWN_NOW( BUTTON_SELECT )) )
        {
            count = 1100;
        }
        // Creates a new projectile
        if( KEY_DOWN_NOW(BUTTON_A) && proj->active == 0 )
        {
            createProjectile( proj, bP->x, bP->y );
        }
        // Shift pillar
        if( count > 400 )
        {
            if( (A->x < 1) && (A->width == 1) )
            {
            }
            else if( A->x < 1 )
            {
                A->width -= 1;
            }
            else if( A->x > 220 )
            {
                A->x -= 1;
                A->width += 1;
            }
            else
            {
                A->x -= 1;
            }
        }
        // Shift projectile
        if( proj->active == 1 && (proj->x + proj->length < 236) )
        {
            proj->x += 4;
            if( proj->length < 12 )
            {
                proj->length++;
            }
        }
        else
        {
            removeProjectile( proj );
        }
        // Check for projectile collision and
        //  displays a successful message if
        //  they shoot the correct spot
        if( proj->active == 1 )
        {
            checkCol = projCollision( (proj->x + proj->length), proj->y, 1, A );
            if( checkCol == 1 )
            {
                removeProjectile( proj );
            }
            else if( checkCol == 2 )
            {
                removeProjectile( proj );
                A->active = 0;
                sprintf( text, "Good! Now pass through the opening!" );
                drawRectangle( 140, 0, 240, 20, BLACK );
                drawString( 148, 10, text, YELLOW );
            }
        }
        // Checks for projectile collision and
        //  displays an unsuccessful message if
        //  player collides
        checkCol = buzzCollision( bP->x, bP->y, A );
        if( checkCol == 1 )
        {
            sprintf( text, "Try not to hit the pillar next time..." );
            drawRectangle( 140, 0, 240, 20, BLACK );
            drawString( 148, 10, text, YELLOW );
        }
        // Increment tutorial counter
        count++;
    }
    return S_MENU;
}
// 6: CONTROLS
int controls(STR_BUZZ *bP)
{
    int gameState = S_LEVELSELECT;
    int count = 1;
    drawBuzz( bP->y, bP->x, buzz_data );
    // Create Projectile Pointer
    proj = &projectile;
    removeProjectile( proj );
    // Draw starting frame
    drawRectangle( 0, 0, 240, 140, SKY );
    drawRectangle( 140, 0, 240, 20, BLACK );
    sprintf( text, "CONTROLS" );
    drawString( 148, 90, text, YELLOW );
    while( gameState == S_LEVELSELECT )
    {
        drawBuzz( bP->y, bP->x, buzz_data );
        if( count < 55 )
        {
            // You get the idea with the shifting boxes
            drawRectangle( 15, 240 - (count * 2), count * 2, 20, BLACK );
            drawRectangle( 45, 240 - (count * 2), count * 2, 20, BLACK );
            drawRectangle( 75, 240 - (count * 2), count * 2, 20, BLACK );
            drawRectangle( 105, 240 - (count * 2), count * 2, 20, BLACK );
        }
        else if( count < 65 )
        {
            drawRectangle( 15, 240 - (count * 2), 110, 20, BLACK );
            drawRectangle( 45, 240 - (count * 2), 110, 20, BLACK );
            drawRectangle( 75, 240 - (count * 2), 110, 20, BLACK );
            drawRectangle( 105, 240 - (count * 2), 110, 20, BLACK );
        }
        else if( count == 65 )
        {
            sprintf( text, "[A] FIRE" );
            drawString( 22, 115, text, YELLOW );
            sprintf( text, "[<][>] X-MOVEMENT" );
            drawString( 52, 115, text, YELLOW );
            sprintf( text, "[^][v] Y-MOVEMENT" );
            drawString( 82, 115, text, YELLOW );
            sprintf( text, "[SELECT} BACK" );
            drawString( 112, 115, text, YELLOW );
        }
        //if projectile active, draw it
        if( proj->active == 1 )
        {
            drawRectangle( proj->y, proj->x, proj->length, 2, BLACK );
        }

        waitForVblank();
        // Undraw buzz & projectile
        drawRectangle( bP->y, bP->x, bP->w, bP->h, SKY );
        if( count < 64 )
        {
            drawRectangle( 15, 240 - count * 2, 110, 20, SKY );
            drawRectangle( 45, 240 - count * 2, 110, 20, SKY );
            drawRectangle( 75, 240 - count * 2, 110, 20, SKY );
            drawRectangle( 105, 240 - count * 2, 110, 20, SKY );
        }
        if( proj->active == 1 )
        {
            drawRectangle( proj->y, proj->x, proj->length, 2, SKY );
        }
        // Get user input
        if( KEY_DOWN_NOW( BUTTON_UP ) )
        {
            bP->y -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_DOWN ) )
        {
            bP->y += movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_LEFT ) )
        {
            bP->x -= movespeed;
        }
        if( KEY_DOWN_NOW( BUTTON_RIGHT ) )
        {
            bP->x += movespeed;
        }
        // Don't let buzz leave screen
        if( bP->y < 0 )
        {
            bP->y += movespeed + 1;
        }
        else if( bP->y > 120 )
        {
            bP->y -= movespeed + 1;
        }
        if( bP->x < 0 )
        {
            bP->x += movespeed + 1;
        }
        else if( bP->x > 90 )
        {
            bP->x -= movespeed + 1;
        }
        // Create a new projectile
        if( KEY_DOWN_NOW(BUTTON_A) && proj->active == 0 )
        {
            createProjectile( proj, bP->x, bP->y );
        }
        // Shift projectile
        if( proj->active == 1 && (proj->x + proj->length < 90) )
        {
            proj->x += 4;
            if( proj->length < 12 )
            {
                proj->length++;
            }
        }
        else
        {
            removeProjectile( proj );
        }
        if( count < 66 )
        {
            count++;
        }
        if( KEY_DOWN_NOW(BUTTON_SELECT) && count > 20 )
        {
            // if player hits select, go to menu
            gameState = S_MENU;
        }
    }
    return gameState;
}
// 7: DEATH
int deathAnim(STR_BUZZ *bP)
{
    int death[21] = { 0, 0, -1, -1, -2, -2, -3, -2, -2, -1, -1, 0, 0, 0, 1, 1, 2, 2, 3, 4, 5 };
    count = 0;
    while( count < 40 )
    {
        // Draw buzz at current location and text
        drawBuzz( bP->y, bP->x, buzz_data );
        drawRectangle( 140, 0, 240, 20, BLACK );
        sprintf( text, "SCORE: %d", bP->score );
        drawString( 148, 10, text, YELLOW );
        sprintf( text, "LIVES: 0" );
        drawString( 148, 80, text, YELLOW );
        waitForVblank();
        // Undraw buzz & don't let him loop to
        //  the top of the screen
        if( bP->y < 140 )
        {
            drawRectangle( bP->y, bP->x, bP->w, bP->h, SKY );
        }
        if( count < 21 )
        {
            (bP->y) += death[count];
        }
        else if( bP->y > 140 )
        {
            bP->y = 140;
        }
        else
        {
            (bP->y) += 10;
        }
        count++;
    }
    return S_GAMEOVER;
}
// 8: GAMEOVER
int gameOver(STR_BUZZ *bP)
{
    int gameState = S_GAMEOVER;
    // Draw initial game frame
    drawImage( 0, 0, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, gameover_data );
    sprintf( text, "YOUR SCORE: %d", bP->score );
    // if statement centers text
    if( bP->score < 10 )
    {
        count = 80;
    }
    else if( bP->score < 100 )
    {
        count = 78;
    }
    else
    {
        count = 76;
    }
    drawString( 65, count, text, YELLOW );
    count = 0;
    sprintf( text, "PRESS ENTER OR START TO RETRY" );
    while( gameState == S_GAMEOVER )
    {
        if( KEY_DOWN_NOW(BUTTON_SELECT) || KEY_DOWN_NOW( BUTTON_START ) )
        {
            // Recreate buzz at initial coordinates and
            // replay the game
            createBuzz( bP );
            gameState = S_COUNTDOWN;
        }
        // Code for flashing text
        if( count == 0 )
        {
            drawString( 145, 40, text, YELLOW );
        }
        else if( count == 40 )
        {
            drawRectangle( 140, 0, 240, 20, BLACK );
        }
        waitForVblank();

        // loops from 0-80 to make text flash
        if( count == 80 )
        {
            count = 0;
        }
        else
        {
            count++;
        }
    }
    return gameState;
}
