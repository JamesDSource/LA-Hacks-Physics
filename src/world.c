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

			Vec2 mpv;
			if (GJK(&(world->objects), i, j, &mpv)) {

			}
		}
	}
}

static Vec2 circleSupport(Circle circle, Vec2 dir) {
	return Vec2Mult(dir, (Vec2){circle.radius, circle.radius});
}

static Vec2 polygonSupport(Polygon poly, Vec2 dir) {
	assert(poly.points->len > 2);

	Vec2 *item = &poly.points->items;
	Vec2 closest_point = *item;
	Fixed_FLT largest_dot = Vec2Dot(closest_point, dir);

	for (item++; item < (&(poly.points->items) + poly.points->len); ++item) {
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
		support_a = circleSupport(list->objects[a].shape.circle, dir);
		break;
	case SHAPE_POLYGON:
		support_a = polygonSupport(list->objects[a].shape.polygon, dir);
		break;
	}

	dir = Vec2MultScaler(dir, -1);
	dir = Vec2Normalize(dir);
	switch (list->objects[b].type) {
	case SHAPE_CIRCLE:
		support_b = circleSupport(list->objects[b].shape.circle, dir);
		break;
	case SHAPE_POLYGON:
		support_b = polygonSupport(list->objects[b].shape.polygon, dir);
		break;
	}

	return Vec2Add(Vec2Sub(support_a, support_b), Vec2Sub(list->positions[a], list->positions[b]));
}

static Vec2 tripleProduct(Vec2 a, Vec2 b, Vec2 c) {
	Fixed_FLT firstz = FixedSub(FixedMult(a.x, b.y), FixedMult(b.x, a.y));
	Vec2 result = (Vec2){
		.x = FixedMult(c.y, firstz),
		.y = FixedMult(firstz, c.x),
	};
	return result;
}

static void gjkLineCase(Simplex *simplex, Vec2 *dir) {
	assert(simplex->len == 2);

	Vec2 b = simplex->points[1];
	Vec2 c = simplex->points[0];

	Vec2 cb = Vec2Sub(b, c);
	Vec2 c0 = Vec2MultScaler(c, -1);

	Vec2 triple = tripleProduct(cb, c0, cb);
	*dir = Vec2Normalize(triple);
}

static bool gjkTriangleCase(Simplex *simplex, Vec2 *dir) {
	assert(simplex->len == 3);

	Vec2 a = simplex->points[2];
	Vec2 b = simplex->points[1];
	Vec2 c = simplex->points[0];

	Vec2 a0 = Vec2MultScaler(a, -1);
	Vec2 ac = Vec2Sub(c, a);
	Vec2 ab = Vec2Sub(b, a);

	Vec2 ab_pro = tripleProduct(ac, ab, ab);
	Vec2 ac_pro = tripleProduct(ab, ac, ac);

	if (Vec2Dot(ab_pro, a0) > 0) {
		simplex->points[1] = a;
		simplex->points[0] = b;
		simplex->len = 2;

		*dir = Vec2Normalize(ab_pro);

		return false;
	} else if (Vec2Dot(ac_pro, a0) > 0) {
		simplex->points[1] = a;
		simplex->points[0] = c;
		simplex->len = 2;

		*dir = Vec2Normalize(ac_pro);
		return false;
	}

	return true;
}

static Vec2 vec2Normal(Vec2 x) {
	return (Vec2) {
		.x = x.y,
		.y = -x.x,
	};
}

static Vec2 epa(ObjectList *objects, size_t a, size_t b, Simplex simplex) {
	PointList *polytope;
	PointListInit(simplex.len*2, &polytope);
	for (size_t i = 0; i < simplex.len; ++i) {
		PointListAppend(&polytope, simplex.points[i]);
	}

	size_t min_index = 0;
	Vec2 min_normal = {0};
	Fixed_FLT min_dist = FLT_MAX;
	while(min_dist == FLT_MAX) {
		Vec2 *items = &polytope->items;
		for (size_t i = 0; i < polytope->len; ++i) {
			size_t j = i+1;
			if (j == polytope->len) {
				j = 0;
			}

			Vec2 ij = vec2Normal(Vec2Sub(items[j], items[i]));
			Vec2 normal = Vec2Normalize0(ij);
			if (normal.x == 0 && normal.y == 0) {
				continue;
			}

			Fixed_FLT dist = Vec2Dot(normal, items[i]);
			if (dist < 0) {
				dist = -dist;
				normal = Vec2MultScaler(normal, -1);
			}

			if (dist < min_dist) {
				min_dist = dist;
				min_normal = normal;
				min_index = j;
			}
		}

		Vec2 support = supportPoint(objects, a, b, min_normal);
		Fixed_FLT support_dist = Vec2Dot(min_normal, support);

		if (FixedAbs(FixedSub(support_dist, min_dist)) > FixedFromFloat(0.001)) {
			min_dist = FLT_MAX;
			List_Result insert_res = PointListInsertAt(&polytope, support, min_index);
			assert(insert_res == LIST_RESULT_SUCCESS);
		}
	}

	PointListFree(polytope);
	Fixed_FLT mult = FixedAdd(min_dist, FixedFromFloat(0.001));
	return Vec2Mult(min_normal, (Vec2){mult});
}

bool GJK(ObjectList *list, size_t a, size_t b, Vec2 *mpv) {
	Vec2 dir = Vec2Normalize0(Vec2Sub(list->positions[b], list->positions[a]));
	if (dir.x == 0 && dir.y == 0) {
		dir.y = FixedFromInt(1);
	}

	Simplex simplex = (Simplex){
		.points[0] = supportPoint(list, a, b, dir),
		.len = 1,
	};
	dir = Vec2MultScaler(dir, -1);

	while (true) {
		Vec2 point = supportPoint(list, a, b, dir);
		simplex.points[simplex.len] = point;
		simplex.len += 1;

		if (Vec2Dot(point, dir) < 0) {
			return false;
		}

		switch (simplex.len) {
		case 2:
			gjkLineCase(&simplex, &dir);
			break;
		case 3:
			if (gjkTriangleCase(&simplex, &dir)) {
				if (mpv != NULL && false) {
					*mpv = epa(list, a, b, simplex);
				}
				return true;
			}
			break;
		default:
			assert(false);
			break;
		}

	}

	return false;
}

// @returns number of failures
int GJKTest(void) {
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

		bool res = GJK(&world->objects, 0, 1, NULL);
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
