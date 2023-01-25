#include <stdlib.h>
#include "system.h"

int SetRandom(int min, int max) {
	return rand() % (max - (min)+1) + (min);
}