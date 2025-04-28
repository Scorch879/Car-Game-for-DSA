#include "powerups.h"

void activateShield() {
    shield = true;
}

void activateSlowmo(){
	slowmo = true;
	slowmoTicks = SLOWMO_DURATION;
}
