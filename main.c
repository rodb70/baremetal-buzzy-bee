// MARK WALKER
// #903112725
// markwalker@gatech.edu

#include "myLib.h"
int main() {
	REG_DISPCNT = MODE_3 | BG2_ENABLE;
	int gameState = 0;
	// Set buzz pointer pointers
	STR_BUZZ BUZZ_STRUCTURE;
	STR_BUZZ* bP;
	bP = &BUZZ_STRUCTURE;

	// Enters game loop
	while(1) {
		if(gameState==S_TITLE) {
			gameState = title();
		} else if(gameState==S_MENU) {
			gameState = menu(bP);
		} else if(gameState==S_LEVELSELECT) {
			gameState = levelSelect(bP);
		} else if(gameState==S_COUNTDOWN) {
			gameState = countdown(bP);
		} else if(gameState==S_PLAY) {
			gameState = play(bP);
		} else if(gameState==S_TUTORIAL) {
			gameState = tutorial(bP);
		} else if(gameState==S_CONTROLS) {
			gameState = controls(bP);
		} else if(gameState==S_DEATH) {
			gameState = deathAnim(bP);
		} else if(gameState==S_GAMEOVER) {
			gameState = gameOver(bP);
		}
	}
}