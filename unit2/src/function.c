#include "function.h"
int nCompletionStatus = 0;

// this is a helper function
static float add(float x, float y) {
	float z = x + y;
	return z;
}

float add_and_multiply(float x, float y) {
	float z = add(x, y);
	z *= MULTIPLIER;
	return z;
}

