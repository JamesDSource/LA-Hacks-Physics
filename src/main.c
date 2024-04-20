#include<stdio.h>
#include<stdbool.h>

typedef struct {

} World;

int main() {
	InitWindow(1920, 1080, "Physics Demo", NULL, NULL);

	World *world = WorldInit();

	while (!WindowShouldClose()) {
		WorldTick(world);
		VisDraw(world);
	}

	WorldDeinit(world);
	CloseWindow();
	return 0;
}
