#pragma once
#include <stdlib.h>
#include <time.h>

int SetRandom(int min, int max) {
	return rand() % (max - (min)+1) + (min);
}