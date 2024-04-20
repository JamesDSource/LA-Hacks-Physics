#include<stdlib.h>
#include"world.h"

LIST(Vec2, PointList)

List_Result ObjectListInit(size_t cap, ObjectList *list) {
	list->len = 0;
	list->cap = cap;
	list->objects = malloc(sizeof(Object)*cap);
	list->positions = malloc(sizeof(Vec2)*cap);
	list->velocities = malloc(sizeof(Vec2)*cap);
	list->materials = malloc(sizeof(ObjectMaterial)*cap);

	if (list->objects == NULL || list->positions == NULL || list->velocities == NULL || list->materials == NULL) {
		return LIST_RESULT_OUT_OF_MEMORY;
	}

	return LIST_RESULT_SUCCESS;
}

void ObjectListFree(ObjectList list) {
	free(list.objects);
	free(list.positions);
	free(list.velocities);
	free(list.materials);
}

List_Result ObjectListAppend(ObjectList *list, Object object, Vec2 position, Vec2 velocity, ObjectMaterial material) {
	size_t len = list->len;
	if (len + 1 >= list->cap) {
		list->cap *= 2;
		list->objects = realloc(list->objects, sizeof(Object)*list->cap);
		list->positions = realloc(list->positions, sizeof(Vec2)*list->cap);
		list->velocities = realloc(list->velocities, sizeof(Vec2)*list->cap);
		list->materials = realloc(list->materials, sizeof(ObjectMaterial)*list->cap);

		if (list->objects == NULL || list->positions == NULL || list->velocities == NULL || list->materials == NULL) {
			return LIST_RESULT_OUT_OF_MEMORY;
		}
	}

	list->objects[len] = object;
	list->positions[len] = position;
	list->velocities[len] = velocity;
	list->materials[len] = material;
	list->len += 1;

	return LIST_RESULT_SUCCESS;
}

List_Result ObjectListUnorderedRemove(ObjectList *list, int idx) {
	if (idx >= list->len) {
		return LIST_RESULT_OUT_OF_BOUNDS;
	}

	int last = list->len - 1;
	list->objects[idx] = list->objects[last];
	list->positions[idx] = list->positions[last];
	list->velocities[idx] = list->velocities[last];
	list->materials[idx] = list->materials[last];
	list->len = last;
	return LIST_RESULT_SUCCESS;
}

World_Result WorldInit(World **world) {
	*world = malloc(sizeof(World));
	if (*world == NULL) {
		return WORLD_RESULT_OUT_OF_MEMORY;
	}

	**world = (World){0};
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
	for (size_t i = 0; i < world->objects.len; ++i) {
		for (size_t j = 0; j < world->objects.len; ++j) {
			if (i == j) {
				continue;
			}
		}
	}
}
