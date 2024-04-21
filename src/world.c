#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
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

List_Result ObjectListAppendCircle(ObjectList *list, Circle circle, Vec2 position, Vec2 velocity, ObjectMaterial material) {
	return ObjectListAppend(list, (Object){ .type = SHAPE_CIRCLE, .shape = (Shape){ .circle = circle } }, position, velocity, material);
}

List_Result ObjectListAppendPolygon(ObjectList *list, Polygon polygon, Vec2 position, Vec2 velocity, ObjectMaterial material) {
	return ObjectListAppend(list, (Object){ .type = SHAPE_POLYGON, .shape = (Shape){ .polygon = polygon } }, position, velocity, material);
}

List_Result ObjectListUnorderedRemove(ObjectList *list, int idx) {
	if (idx < 0 || idx >= list->len) {
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

void ObjectListClear(ObjectList* list) {
	for (size_t i = 0; i < list->len; ++i) {
		if (list->objects[i].type == SHAPE_POLYGON) {
			PointListFree(list->objects[i].shape.polygon.points);
		}
	}
	list->len = 0;
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
	for(size_t i = 0; i < world->objects.len; ++i){
		world->objects.positions[i] = Vec2Add(world->objects.positions[i], world->objects.velocities[i]);
	}

	for (size_t i = 0; i < world->objects.len; ++i) {
		for (size_t j = 0; j < world->objects.len; ++j) {
			if (i == j) {
				continue;
			}
		}
	}
}

static Vec2 circleSupport(Circle circle, Vec2 position, Vec2 dir) {
	return Vec2Add(position, Vec2MultScaler(dir, circle.radius));
}

static Vec2 polygonSupport(Polygon poly, Vec2 position, Vec2 dir) {
	assert(poly.points->len > 2);

	Vec2 *item = &poly.points->items;
	Vec2 closest_point = *item;
	Fixed_FLT largest_dot = Vec2Dot(closest_point, dir);

	for (item++; item < item + poly.points->len; ++item) {
		Fixed_FLT dot = Vec2Dot(*item, dir);
		if (dot > largest_dot) {
			largest_dot = dot;
			closest_point = *item;
		}
	}

	return closest_point;
}

static Vec2 supportPoint(ObjectList *list, size_t a, size_t b, Vec2 dir) {
	Vec2 support_a, support_b;
	switch (list->objects[a].type) {
	case SHAPE_CIRCLE:
		support_a = circleSupport(list->objects[a].shape.circle, list->positions[a], dir);
		break;
	case SHAPE_POLYGON:
		support_a = polygonSupport(list->objects[a].shape.polygon, list->positions[a], dir);
		break;
	}

	switch (list->objects[a].type) {
	case SHAPE_CIRCLE:
		support_b = circleSupport(list->objects[b].shape.circle, list->positions[b], dir);
		break;
	case SHAPE_POLYGON:
		support_b = polygonSupport(list->objects[b].shape.polygon, list->positions[b], dir);
		break;
	}

	return Vec2Add(Vec2Sub(support_a, support_b), Vec2Sub(list->positions[a], list->positions[b]));
}

static Vec2 vec2Cross3(Vec2 a, Fixed_FLT az, Vec2 b, Fixed_FLT bz, Fixed_FLT *out) {
	Vec2 c;
	c.x = FixedSub(FixedMult(a.y, bz), FixedMult(b.y, az));
	c.y = FixedSub(FixedMult(az, b.x), FixedMult(bz, a.x));
	if (out != NULL) {
		*out = FixedSub(FixedMult(a.x, b.y), FixedMult(b.x, a.y));
	}

	return c;
}

static Vec2 vec2TripleCross3(Vec2 a, Fixed_FLT az, Vec2 b, Fixed_FLT bz) {
	Fixed_FLT cz;
	Vec2 c = vec2Cross3(a, az, b, bz, &cz);
	return vec2Cross3(c, cz, a, az, NULL);
}

static void gjkLineCase(Simplex *simplex, Vec2 *dir) {
	assert(simplex->len == 2);

	Vec2 ao = Vec2MultScaler(simplex->points[1], FixedFromInt(-1));
	Vec2 b = simplex->points[0];
	Vec2 ab = Vec2Add(b, ao);

	printf("Line Case");
	if (Vec2Dot(ab, ao) > 0) {
		*dir = Vec2Normalize(vec2TripleCross3(ab, 0, ao, 0));
	}
}

static bool gjkTriangleCase(Simplex *simplex, Vec2 *dir) {
	assert(simplex->len == 3);

	Vec2 a = simplex->points[2];
	Vec2 b = simplex->points[1];
	Vec2 c = simplex->points[0];

	Vec2 ao = Vec2MultScaler(a, -1);
	Vec2 ac = Vec2Sub(c, a);
	Vec2 ab = Vec2Sub(b, a);

	Fixed_FLT abcz;
	Vec2 abc = vec2Cross3(ab, 0, ac, 0, &abcz);

	printf("Triangle Case\n");
	if (Vec2Dot(vec2Cross3(abc, abcz, ac, 0, NULL), ao) > 0) {
		printf("Triangle AC\n");
		if (Vec2Dot(ac, ao) > 0) {
			printf("Triangle AC 2 Points\n");
			simplex->points[1] = a;
			simplex->points[0] = c;
			simplex->len = 2;

			*dir = Vec2Normalize(vec2TripleCross3(ac, 0, ao, 0));
		} else {
			printf("Triangle AC 1 Point\n");
			simplex->points[0] = a;
			simplex->len = 1;

			*dir = Vec2Normalize(ao);
		}

		return false;
	} else if (Vec2Dot(vec2Cross3(ab, 0, abc, abcz, NULL), ao) > 0) {
		printf("Triangle AB\n");
		if (Vec2Dot(ab, ao) > 0) {
			printf("Triangle AB 2 Points\n");
			simplex->points[1] = a;
			simplex->points[0] = b;
			simplex->len = 2;

			*dir = Vec2Normalize(vec2TripleCross3(ab, 0, ao, 0));
		} else {
			printf("Triangle AB 1 Point\n");
			simplex->points[0] = a;
			simplex->len = 1;

			*dir = Vec2Normalize(ao);
		}

		return false;
	}

	return true;
}

bool GJK(ObjectList *list, size_t a, size_t b) {
	Vec2 dir = (Vec2){
		.x = FixedFromInt(1),
		.y = 0,
	};

	Simplex simplex = (Simplex){
		.points[0] = supportPoint(list, a, b, dir),
		.len = 1,
	};
	dir = Vec2Normalize0(Vec2MultScaler(simplex.points[0], -1));

	while (true) {
		Vec2 point = supportPoint(list, a, b, dir);
		if (Vec2Dot(point, dir) < 0) {
			return false;
		}

		simplex.points[simplex.len] = point;
		simplex.len += 1;
		switch (simplex.len) {
		case 2:
			gjkLineCase(&simplex, &dir);
			break;
		case 3:
			if (gjkTriangleCase(&simplex, &dir)) {
				return true;
			}
			break;
		default:
			assert(false);
			break;
		}

	}

	return true;
}

// @returns number of failures
int GJKTest() {
	int failures = 0;
	World *world;
	World_Result world_res = WorldInit(&world);
	if (world_res != WORLD_RESULT_SUCCESS) {
		fprintf(stderr, "WorldInit failed with exit code %d\n", world_res);
		return 1;
	}

	{
		ObjectListAppendCircle(
			&world->objects,
			(Circle){ .radius = FixedFromInt(10) },
			(Vec2){ .x = FixedFromInt(100), .y = FixedFromInt(100) },
			(Vec2){0},
			(ObjectMaterial){0});

		ObjectListAppendCircle(
			&world->objects,
			(Circle){ .radius = FixedFromInt(10) },
			(Vec2){ .x = FixedFromInt(105), .y = FixedFromInt(100) },
			(Vec2){0},
			(ObjectMaterial){0});

		bool res = GJK(&world->objects, 0, 1);
		if (res) {
			printf("Circle test 1 passed\n");
		} else {
			fprintf(stderr, "Circle test 1 failed\n");
			failures++;
		}

		ObjectListClear(&world->objects);
	}

	WorldDeinit(world);
	return failures;
}
