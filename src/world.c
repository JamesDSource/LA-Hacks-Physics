#include<stdlib.h>
#include"world.h"

World_Result WorldInit(World **world) {
	*world = malloc(sizeof(World));
	if (*world == NULL) {
		return WORLD_RESULT_OUT_OF_MEMORY;
	}

	**world = (World){};
	List_Result list_res = ObjectListInit(30, &(**world).objects);
	if (list_res == LIST_RESULT_OUT_OF_MEMORY) {
		return WORLD_RESULT_OUT_OF_MEMORY;
	}

	return WORLD_RESULT_SUCCESS;
}

void WorldDeinit(World *world) {
	ObjectListFree(world->objects);
	free(world);
}

void WorldTick(World *world) {

}
