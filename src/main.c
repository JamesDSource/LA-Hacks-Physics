#include<stdio.h>
#include<stdbool.h>
#include"world.h"
#include<assert.h>

// Add the fixed math implementation. Do only once
#define _FIXED_MATH_IMPL_
#include"fixed_math.h"

int main() {
	// InitWindow(1920, 1080, "Physics Demo", NULL, NULL);

	World *world;
	World_Result world_res = WorldInit(&world);
	assert(world_res == WORLD_RESULT_SUCCESS);

	// while (!WindowShouldClose()) {
	// 	VisDraw(world)
	// }

	WorldDeinit(world);
	// CloseWindow();
	return 0;
}
