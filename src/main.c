#include<stdio.h>
#include<stdbool.h>
#include<assert.h>
#include<time.h>
#include"raylib.h"
#include"world.h"
#include"vis.h"

int main(void) {   
	InitWindow(800, 800, "Physics Demo");

	World *world;
	World_Result world_res = WorldInit(&world);
		for (int i = 0; i < 30; ++i) {
			ObjectListAppendCircle(&world->objects, (Circle){.radius = FixedFromInt(20)}, (Vec2){.x = FixedFromInt(i*30)}, (Vec2){FixedFromInt(i%10 - 5), FixedFromInt(i)}, (ObjectMaterial){0});
		}
	if (world_res != WORLD_RESULT_SUCCESS) {
		fprintf(stderr, "WorldInit failed with error code %d\n", world_res);
	}

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		WorldTick(world);

		BeginDrawing();
		ClearBackground(RAYWHITE);
		VisualizeWorld(world);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
