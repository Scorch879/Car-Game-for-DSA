#include "powerups.h"

void activatePowerUp() {
    int power = rand() % 2;
    if (power == 0) shield = true;
    else {
        slowmo = true;
        slowmoTicks = SLOWMO_DURATION;
    }
}
