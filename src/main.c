#include<stdio.h>
#include<stdbool.h>
#include"world.h"
#include<assert.h>

int main() {
	// InitWindow(1920, 1080, "Physics Demo", NULL, NULL);
	Vec2 v = (Vec2){
		.x = FixedFromInt(1),
		.y = FixedFromInt(2),
	};
	Fixed_FLT length = FixedSqrt(FixedAdd(FixedMult(v.x, v.x), FixedMult(v.y, v.y)));
	printf("%f\n%f\n%f\n", FloatFromFixed(FixedDiv(FixedFromInt(2), FixedFromInt(3))), FloatFromFixed(FixedFromFloat(2)), FloatFromFixed(length));

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
