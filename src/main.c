#include <bits/types/struct_timeval.h>
#include<stdio.h>
#include<stdbool.h>
#include<assert.h>
#include <stdlib.h>
#include<time.h>
#include"raylib.h"
#include"world.h"
#include"vis.h"

int main(void) {   
	InitWindow(800, 800, "Physics Demo");

	World *world;
	srand(time(NULL));
	World_Result world_res = WorldInit(&world);
		for (int i = 0; i < 2; ++i) {
			ObjectListAppendCircle(
					&world->objects, 
					(Circle){.radius = FixedFromInt(10)},
					(Vec2){.x = FixedFromInt(rand()%100), .y = FixedFromInt(rand()%100)},
					(Vec2){FixedFromInt(rand()%10 - 5), FixedFromInt(rand()%10 - 5)},
					(ObjectMaterial){0});
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
