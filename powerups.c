#include "powerups.h"

void activateShield() {
    shield = true;
}

void activateSlowmo(){
	slowmo = true;
	slowmoTicks = SLOWMO_DURATION;
}

void reloadBomb(){
	bombAmmo += 5;
}

void activateBomb(){
	bombCar = true;
	bombTicks = BOMB_DURATION;
}
